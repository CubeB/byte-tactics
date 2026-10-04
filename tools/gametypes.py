"""Write include/ta_types.h: one definition of each game type, merged from the files' views.

    uv run tools/gametypes.py              # writes include/ta_types.h
    uv run tools/gametypes.py --stats      # also lists the joins refused and the types left out
    uv run tools/gametypes.py --verify     # also checks every size and field offset by compiling
    uv run tools/gametypes.py --explain Unit   # the views of a type and what joined them

(Not tools/types.py: a file of that name in tools/ would shadow Python's own
`types` module for every tool there.)

Every file under src/unsorted/ declares the structs and classes it uses: a
partial view of each, with the real offsets of the fields its functions touch.
Cavedog's headers, which declared each type once, are lost. This rebuilds the
part the views know.

1. Every file is compiled again with /Z7 (without its /Gi, which /Z7 does not
   allow and which changes no layout), and the CodeView records give each view
   exactly as VC5 laid it out: size, fields with offsets, bitfields, anonymous
   unions, base classes, the vtable pointer, member functions and static
   members. The object's symbol table gives the decorated name of every
   function and global the file defines or calls, with its types.
2. Views are joined into game types, strongest evidence first:
   - the same global or function declared in two files: the decorated name
     gives the type in each position, so `extern Game_00449bb0* g_game` and
     `extern Game* g_game` are views of one type. A function's own file gives
     its real signature and each caller is joined to it (return types only
     there, since callers often guess them); a member function's decoration
     names its class, which every file must spell the same;
   - inside a joined type, the struct, pointer or array at the same offset of
     two views (embedded structs only when they are as large);
   - the same name (`Unit`, or `Unit_0041b2e0`, a file's own view of a Unit):
     a type joins the largest type of its name when they share fields (two
     over the same bytes, or one of the same name) and do not disagree, or
     disagree somewhere but share at least half of the smaller one's fields;
   - a name a file only refers to (`struct Unit;`) is the one type with views
     of that exact name, when there is exactly one.
   A join is refused when the two disagree about the type itself: a size a
   view that embeds the type or indexes an array of it gives, a field past
   that size, or a union against a struct; and, for views of different names,
   any field of one crossing a field of the other. When the evidence that a
   type is another outweighs its own views (a view joined by one prototype
   that disagrees with the type the prototype's own file has), it is left out:
   declared, so pointers to it keep a name, but not defined. Names the
   toolchain's headers define (a file's own copy of DPNAME) are not game types.
3. Each type's fields are its views' fields by byte range. A range is decided
   by how many views declare a field over exactly those bytes: a field that
   crosses another loses to it if fewer views have it, and when as many have
   each, neither is kept (the bytes stay unknown). A struct or array keeps the
   fields inside it unless more views have a field inside it than have it (a
   view that groups a run of fields into a struct of its own); a char array
   with other fields inside is a block one view did not split. The type and
   name are the ones most views give the chosen range; bitfields are merged by
   their bits; alternatives one view declares over the same bytes are its
   anonymous union. A struct held by value must be as large as the header lays
   it out, or the range stays unknown.
4. Member functions come from the class bodies (the form most views give) and
   from the decorated names in data/progress.csv of the members the files
   define (members, constructors, destructors and operators), which win where
   both have one. A member is declared virtual only where the class (or a base)
   has a vtable pointer in the views; a base class stays only where the header
   lays it out as large as the derived views have it.

Each type takes the name its views most often give it, preferring one without
an address suffix; names are unique. Gaps between known fields are
`char unknown_<offset>[n]`, as the files write them, and every type is laid out
under `#pragma pack(1)`, so the offsets are the views' own (--verify checks
each one). Types come in address order of the first file that uses them, after
the types they hold by value. The header includes the system headers its types
need. Nothing is invented: each type, field and member is in some file.

docs/c2-regalloc.md ("A game types header") gives what the header adds in
symbol ids and where that puts the functions waiting on a symbol-id window.
"""

import argparse
import bisect
import csv
import hashlib
import json
import os
import re
import struct
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from check import DEFAULT_FLAGS, ROOT, compile_source  # noqa: E402
from coff import parse_object  # noqa: E402
from ghidratypes import (  # noqa: E402
    FWDREF, LF_ARGLIST, LF_ARRAY, LF_BCLASS, LF_BITFIELD, LF_CLASS, LF_ENUM, LF_ENUMERATE,
    LF_MEMBER, LF_METHOD, LF_MFUNCTION, LF_MODIFIER, LF_NESTTYPE, LF_ONEMETHOD, LF_POINTER,
    LF_PROCEDURE, LF_STMEMBER, LF_STRUCTURE, LF_UNION, LF_VFUNCTAB, PRIMS, TypeTable,
    numeric, pstring,
)

SRC = ROOT / "src" / "unsorted"
PROGRESS = ROOT / "data" / "progress.csv"
OUT = ROOT / "include" / "ta_types.h"
CACHE = ROOT / "build" / "typeshdr"
FLAGS = DEFAULT_FLAGS + " /Z7"
LF_METHODLIST = 0x1206
COMPOSITES = (LF_CLASS, LF_STRUCTURE, LF_UNION)
KEY_OF_LEAF = {LF_CLASS: "class", LF_STRUCTURE: "struct", LF_UNION: "union"}
DEFINED = re.compile(r"^\s*(?:typedef\s+)?(struct|class|union|enum)\s+([A-Za-z_]\w*)\s*(?::[^{;]*)?\{", re.M)
ANON = re.compile(r"(^|::)(__unnamed|<unnamed-tag>)$")
CV_CALLS = {0x00: "__cdecl", 0x04: "__fastcall", 0x07: "__stdcall", 0x0b: "__thiscall"}


# --- step 1: the views, read out of CodeView ---------------------------------------

class Views(TypeTable):
    """One object's composites as views: {name: view}, anonymous ones inlined."""

    def __init__(self, data: bytes, wanted: set[str]):
        super().__init__(data)
        self.wanted = wanted
        self.views: dict[str, dict] = {}
        self.busy: set[str] = set()

    def is_anon(self, name: str) -> bool:
        return name == "" or bool(ANON.search(name))

    def expr(self, ti: int, depth: int = 0):
        if ti < 0x1000:
            name = PRIMS.get(ti & 0xff, "undefined")
            if ti == 0x03:
                return "void"
            return {"ptr": name} if (ti >> 8) & 7 else name
        if depth > 60 or ti not in self.records:
            return "undefined"
        leaf, d = self.records[ti]
        if leaf == LF_MODIFIER:
            return self.expr(struct.unpack_from("<I", d)[0], depth + 1)
        if leaf == LF_POINTER:
            utype, attr = struct.unpack_from("<II", d)
            mode = (attr >> 5) & 7
            if mode in (2, 3):
                return {"memptr": 1}
            target = self.expr(utype, depth + 1)
            return {"ref&": target} if mode == 1 else {"ptr": target}
        if leaf == LF_ARRAY:
            elem = struct.unpack_from("<I", d)[0]
            size, _ = numeric(d, 8)
            esize = self.size_of(elem)
            return {"arr": self.expr(elem, depth + 1), "n": size // esize if esize else 0}
        if leaf in COMPOSITES or leaf == LF_ENUM:
            full = self.resolve(ti)
            name = self.composite_name(full)
            if leaf != LF_ENUM and self.is_anon(name):
                return {"anon": self.view(full, depth + 1)}
            if leaf == LF_ENUM:
                self.enum(full)
                return {"enum": name}
            if name in self.wanted and name not in self.views and name not in self.busy:
                self.busy.add(name)
                v = self.view(full, depth + 1)
                self.busy.discard(name)
                if v is not None:
                    self.views[name] = v
            return {"ref": name, "key": KEY_OF_LEAF[self.records[full][0]]}
        if leaf in (LF_PROCEDURE, LF_MFUNCTION):
            return {"fn": self.signature(ti, depth + 1)}
        if leaf == LF_BITFIELD:
            return self.expr(struct.unpack_from("<I", d)[0], depth + 1)
        return "undefined"

    def enum(self, ti: int) -> None:
        name = self.composite_name(ti)
        if name in self.views or name not in self.wanted:
            return
        leaf, d = self.records[ti]
        if struct.unpack_from("<H", d, 2)[0] & FWDREF:
            return
        utype, flist = struct.unpack_from("<II", d, 4)
        values = []
        for sub, s, pos in self.fieldlist(flist):
            if sub == LF_ENUMERATE:
                value, p = numeric(s, pos + 2)
                values.append([pstring(s, p)[0], value])
        self.views[name] = {"kind": "enum", "size": self.size_of(utype), "values": values}

    def method_sig(self, ti: int) -> dict | None:
        if ti not in self.records or self.records[ti][0] != LF_MFUNCTION:
            return None
        return self.signature(ti)

    def view(self, ti: int, depth: int = 0) -> dict | None:
        leaf, d = self.records[ti]
        count, prop = struct.unpack_from("<HH", d)
        if prop & FWDREF:
            return None
        flist = struct.unpack_from("<I", d, 4)[0]
        size, _ = numeric(d, 16 if leaf != LF_UNION else 8)
        v = {"kind": KEY_OF_LEAF[leaf], "size": size, "fields": [], "bases": [], "vfptr": False,
             "methods": [], "statics": []}
        for sub, s, pos in self.fieldlist(flist):
            if sub == LF_BCLASS:
                attr, base = struct.unpack_from("<HI", s, pos)
                off, _ = numeric(s, pos + 6)
                v["bases"].append({"off": off, "type": self.expr(base, depth + 1), "access": attr & 3})
            elif sub == LF_VFUNCTAB:
                v["vfptr"] = True
            elif sub == LF_MEMBER:
                attr, mt = struct.unpack_from("<HI", s, pos)
                off, p = numeric(s, pos + 6)
                field = {"off": off, "name": pstring(s, p)[0], "type": self.expr(mt, depth + 1),
                         "access": attr & 3}
                bl, bd = self.records.get(mt, (0, b""))
                if bl == LF_BITFIELD:
                    field["bits"] = [bd[5], bd[4]]  # position, length
                    field["width"] = self.size_of(struct.unpack_from("<I", bd)[0])
                    field["size"] = field["width"]
                else:
                    field["size"] = self.size_of(mt)
                v["fields"].append(field)
            elif sub == LF_STMEMBER:
                attr, mt = struct.unpack_from("<HI", s, pos)
                v["statics"].append({"name": pstring(s, pos + 6)[0], "type": self.expr(mt, depth + 1),
                                     "access": attr & 3})
            elif sub == LF_ONEMETHOD:
                attr, mt = struct.unpack_from("<HI", s, pos)
                p = pos + 6 + (4 if (attr >> 2) & 7 in (4, 6) else 0)
                self.add_method(v, pstring(s, p)[0], attr, mt)
            elif sub == LF_METHOD:
                n, ml = struct.unpack_from("<HI", s, pos)
                name = pstring(s, pos + 6)[0]
                ml_leaf, md = self.records.get(ml, (0, b""))
                if ml_leaf != LF_METHODLIST:
                    continue
                q = 0
                for _ in range(n):
                    if q + 8 > len(md):
                        break
                    attr, _, mt = struct.unpack_from("<HHI", md, q)
                    q += 8 + (4 if (attr >> 2) & 7 in (4, 6) else 0)
                    self.add_method(v, name, attr, mt)
            elif sub == LF_NESTTYPE:
                v.setdefault("nested", []).append(pstring(s, pos + 6)[0])
        return v

    def add_method(self, v: dict, name: str, attr: int, mt: int) -> None:
        if attr & 0x100:  # compiler generated
            return
        sig = self.method_sig(mt)
        if sig is None:
            return
        sig.pop("this", None)
        v["methods"].append({"name": name, "access": attr & 3, "mprop": (attr >> 2) & 7, "sig": sig})

    def signature(self, ti: int, depth: int = 0) -> dict:
        leaf, d = self.records[ti]
        if leaf == LF_MFUNCTION:
            rv, _, this, call, _, _, args = struct.unpack_from("<IIIBBHI", d)
        else:
            rv, call, _, _, args = struct.unpack_from("<IBBHI", d)
            this = 0
        params, varargs = [], False
        al, ad = self.records.get(args, (0, b"\0\0\0\0"))
        if al == LF_ARGLIST:
            for i in range(struct.unpack_from("<I", ad)[0]):
                pt = struct.unpack_from("<I", ad, 4 + 4 * i)[0]
                if pt == 0:
                    varargs = True
                else:
                    params.append(self.expr(pt, depth + 1))
        sig = {"conv": CV_CALLS.get(call, "__cdecl"), "ret": self.expr(rv, depth + 1),
               "params": params, "varargs": varargs, "static": leaf == LF_MFUNCTION and this == 0}
        return sig

    def all_views(self) -> dict[str, dict]:
        for ti, (leaf, d) in self.records.items():
            if leaf in COMPOSITES or leaf == LF_ENUM:
                try:
                    name = self.composite_name(ti)
                except (struct.error, IndexError):
                    continue
                if name in self.wanted and name not in self.views:
                    self.expr(ti)
        return self.views


# --- decorated names ------------------------------------------------------------------

PRIMITIVES = {
    "C": "signed char", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short",
    "H": "int", "I": "unsigned int", "J": "long", "K": "unsigned long", "M": "float",
    "N": "double", "O": "long double", "X": "void",
}
EXTENDED = {"_N": "bool", "_J": "__int64", "_K": "unsigned __int64", "_W": "wchar_t"}
KEYS = {"U": "struct", "V": "class", "T": "union"}
CONVENTIONS = {"A": "__cdecl", "E": "__thiscall", "G": "__stdcall", "I": "__fastcall"}
CV = {"A": "", "B": "const", "C": "volatile", "D": "const volatile"}
OPERATORS = {
    "0": "ctor", "1": "dtor", "2": "operator new", "3": "operator delete", "4": "operator=",
    "5": "operator>>", "6": "operator<<", "7": "operator!", "8": "operator==", "9": "operator!=",
    "A": "operator[]", "B": "operator cast", "C": "operator->", "D": "operator*", "E": "operator++",
    "F": "operator--", "G": "operator-", "H": "operator+", "I": "operator&", "J": "operator->*",
    "K": "operator/", "L": "operator%", "M": "operator<", "N": "operator<=", "O": "operator>",
    "P": "operator>=", "Q": "operator,", "R": "operator()", "S": "operator~", "T": "operator^",
    "U": "operator|", "V": "operator&&", "W": "operator||", "X": "operator*=", "Y": "operator+=",
    "Z": "operator-=", "_0": "operator/=", "_1": "operator%=", "_2": "operator>>=",
    "_3": "operator<<=", "_4": "operator&=", "_5": "operator|=", "_6": "operator^=",
    "_U": "operator new[]", "_V": "operator delete[]",
}


class Undecorate(Exception):
    pass


class Decoded:
    """A decorated name parsed into its parts.

    Types are tuples: ("prim", name), ("ptr", t, const_ptr), ("ref", t), ("cv", cv, t),
    ("named", key, qualified name), ("enum", qualified name), ("array", dims, t),
    ("func", conv, ret, params, varargs) and ("memfn", class, func). A qualified name
    is a tuple of parts, outermost first; a template part is ("tmpl", name, args).
    """

    def __init__(self, text: str):
        self.s, self.i = text, 0
        self.names: list = []
        self.args: list = []
        self.kind = self.name = self.sig = None
        self.access = self.storage = None
        self.parse()

    def peek(self, n: int = 1) -> str:
        return self.s[self.i:self.i + n]

    def take(self, n: int = 1) -> str:
        part = self.s[self.i:self.i + n]
        if len(part) < n:
            raise Undecorate("truncated")
        self.i += n
        return part

    def fragment(self):
        c = self.peek()
        if c.isdigit():
            self.take()
            if int(c) >= len(self.names):
                raise Undecorate("name back-reference")
            return self.names[int(c)]
        if self.peek(2) == "?$":
            self.take(2)
            saved = self.names, self.args
            self.names, self.args = [], []
            name = self.plain()
            self.names.append(name)
            args = []
            while self.peek() != "@":
                args.append(self.template_arg())
            self.take()
            self.names, self.args = saved
            part = ("tmpl", name, tuple(args))
            if len(self.names) < 10:
                self.names.append(part)
            return part
        if c == "?":
            raise Undecorate("nested special name")
        name = self.plain()
        if len(self.names) < 10:
            self.names.append(name)
        return name

    def plain(self) -> str:
        end = self.s.index("@", self.i)
        name = self.s[self.i:end]
        self.i = end + 1
        return name

    def template_arg(self):
        if self.peek() == "$":
            self.take()
            c = self.take()
            if c == "0":
                return ("int", self.number())
            raise Undecorate("template argument")
        return self.type(slot=True)

    def qualified(self) -> tuple:
        parts = []
        while self.peek() != "@":
            parts.append(self.fragment())
        self.take()
        return tuple(parts[::-1])

    def number(self) -> int:
        c = self.take()
        neg = c == "?"
        if neg:
            c = self.take()
        if c.isdigit():
            value = int(c) + 1
        else:
            value = 0
            while c != "@":
                value = value * 16 + ord(c) - ord("A")
                c = self.take()
        return -value if neg else value

    def type(self, slot: bool = False):
        start = self.i
        c = self.peek()
        if c.isdigit() and slot:
            self.take()
            if int(c) >= len(self.args):
                raise Undecorate("argument back-reference")
            return self.args[int(c)]
        t = self.type_body()
        if slot and self.i - start > 1 and len(self.args) < 10:
            self.args.append(t)
        return t

    def type_body(self):
        c = self.take()
        if c in PRIMITIVES:
            return ("prim", PRIMITIVES[c])
        if c == "_":
            code = c + self.take()
            if code not in EXTENDED:
                raise Undecorate(f"type {code}")
            return ("prim", EXTENDED[code])
        if c in "PQRS":
            const_ptr = c in "QS"
            if self.peek() == "6":
                self.take()
                return ("ptr", self.function(), const_ptr)
            if self.peek() == "8":
                self.take()
                cls = self.qualified()
                self.take()  # this cv
                return ("ptr", ("memfn", cls, self.function()), const_ptr)
            if self.peek() not in CV:
                raise Undecorate("pointer to member data")
            cv = CV[self.take()]
            pointee = self.type_body()
            if cv:
                pointee = ("cv", cv, pointee)
            return ("ptr", pointee, const_ptr)
        if c == "A":
            cv = CV[self.take()]
            target = self.type_body()
            if cv:
                target = ("cv", cv, target)
            return ("ref", target)
        if c in KEYS:
            return ("named", KEYS[c], self.qualified())
        if c == "W":
            self.take()
            return ("enum", self.qualified())
        if c == "Y":
            dims = [self.number() for _ in range(self.number())]
            return ("array", tuple(dims), self.type_body())
        if c == "?":
            cv = CV[self.take()]
            t = self.type_body()
            return ("cv", cv, t) if cv else t
        raise Undecorate(f"type {c}")

    def function(self):
        cc = CONVENTIONS.get(self.take())
        if cc is None:
            raise Undecorate("calling convention")
        if self.peek() == "@":
            self.take()
            ret = None  # constructor or destructor
        else:
            ret = self.type_body()
        params: list = []
        varargs = False
        if self.peek() == "X":
            self.take()
        else:
            while self.peek() not in ("@", "Z"):
                params.append(self.type(slot=True))
            varargs = self.take() == "Z"
        if self.take() != "Z":
            raise Undecorate("no Z after the parameters")
        return ("func", cc, ret, tuple(params), varargs)

    def parse(self) -> None:
        if self.take() != "?":
            raise Undecorate("C name")
        if self.peek() == "?":
            self.take()
            code = self.take()
            if code == "_":
                code += self.take()
            if code not in OPERATORS:
                raise Undecorate(f"special name {code}")
            if code == "B":
                raise Undecorate("conversion operator")
            first = OPERATORS[code]
        elif self.peek() == "$":
            raise Undecorate("function template")
        else:
            first = None
        scope = self.qualified() if first else None
        if first is None:
            parts = self.qualified()
            first, scope = parts[-1], parts[:-1]
        if first == "ctor":
            first = scope[-1] if scope else first
        elif first == "dtor":
            first = "~" + (scope[-1] if isinstance(scope[-1], str) else scope[-1][1]) if scope else first
        self.name = scope + (first,)
        code = self.take()
        if code in "23":
            self.kind = "data"
            self.storage = "static member" if code == "2" else "global"
            self.sig = self.type_body()
            c = self.take()
            if c in "QRST":  # a pointer to member: its class follows
                self.qualified()
            return
        if code == "Y":
            self.kind = "function"
            self.storage = "free"
            self.sig = self.function()
            return
        if code not in "ABCDEFIJKLMNQRSTUV":
            raise Undecorate(f"symbol kind {code}")
        self.kind = "method"
        self.access = {"A": "private", "B": "private", "C": "private", "D": "private",
                       "E": "private", "F": "private", "I": "protected", "J": "protected",
                       "K": "protected", "L": "protected", "M": "protected", "N": "protected",
                       "Q": "public", "R": "public", "S": "public", "T": "public",
                       "U": "public", "V": "public"}[code]
        self.storage = "static" if code in "CDKLST" else "virtual" if code in "EFMNUV" else "member"
        this_cv = "" if self.storage == "static" else CV.get(self.take(), "")
        self.sig = self.function()
        self.this_cv = this_cv


# --- one type language for views and decorations --------------------------------------

CV_PRIMS = {
    "char": "char", "uchar": "unsigned char", "short": "short", "ushort": "unsigned short",
    "long": "long", "ulong": "unsigned long", "longlong": "__int64", "ulonglong": "unsigned __int64",
    "int": "int", "uint": "unsigned int", "bool": "bool", "float": "float", "double": "double",
    "longdouble": "long double", "wchar": "wchar_t", "void": "void",
}
PRIM_SIZE = {
    "char": 1, "signed char": 1, "unsigned char": 1, "bool": 1, "short": 2, "unsigned short": 2,
    "wchar_t": 2, "int": 4, "unsigned int": 4, "long": 4, "unsigned long": 4, "float": 4,
    "double": 8, "long double": 8, "__int64": 8, "unsigned __int64": 8,
}
CXX_PRIMS = sorted(set(PRIMITIVES.values()) | set(EXTENDED.values()) | {"unsigned char", "signed char"},
                   key=len, reverse=True)


def from_view(e):
    """A view's type expression (from Views) in the tuple form Decoded uses, cv dropped."""
    if isinstance(e, str):
        return ("prim", CV_PRIMS.get(e, e)) if e != "undefined" else ("undefined",)
    if "ptr" in e:
        return ("ptr", from_view(e["ptr"]))
    if "ref&" in e:
        return ("ref", from_view(e["ref&"]))
    if "arr" in e:
        return ("array", e["n"], from_view(e["arr"]))
    if "ref" in e:
        return ("named", e.get("key", "struct"), parse_cxx_name(e["ref"]))
    if "enum" in e:
        return ("enum", parse_cxx_name(e["enum"]))
    if "anon" in e:
        return ("anon", convert_view(e["anon"])) if e["anon"] else ("undefined",)
    if "fn" in e:
        f = e["fn"]
        return ("func", f["conv"], from_view(f["ret"]), tuple(from_view(p) for p in f["params"]),
                f["varargs"])
    return ("undefined",)


def convert_view(v: dict) -> dict:
    """A view from Views with every type in the tuple form."""
    out = dict(v)
    out["fields"] = [dict(f, type=from_view(f["type"])) for f in v["fields"]]
    out["bases"] = [dict(b, type=from_view(b["type"])) for b in v.get("bases", [])]
    out["methods"] = [dict(m, sig=from_view({"fn": m["sig"]})) for m in v.get("methods", [])]
    out["statics"] = [dict(s, type=from_view(s["type"])) for s in v.get("statics", [])]
    return out


def from_decoded(t):
    """Decoded's tuples with cv and pointer constness dropped."""
    if t is None:
        return None
    k = t[0]
    if k == "cv":
        return from_decoded(t[2])
    if k == "ptr":
        return ("ptr", from_decoded(t[1]))
    if k == "ref":
        return ("ref", from_decoded(t[1]))
    if k == "array":
        inner = from_decoded(t[2])
        for n in reversed(t[1]):
            inner = ("array", n, inner)
        return inner
    if k == "named":
        return ("named", t[1], tuple(fix_part(p) for p in t[2]))
    if k == "func":
        return ("func", t[1], from_decoded(t[2]), tuple(from_decoded(p) for p in t[3]), t[4])
    if k == "memfn":
        return ("memfn", tuple(fix_part(p) for p in t[1]), from_decoded(t[2]))
    return t


def fix_part(p):
    if isinstance(p, tuple) and p[0] == "tmpl":
        return ("tmpl", p[1], tuple(from_decoded(a) if isinstance(a, tuple) and a[0] != "int" else a
                                    for a in p[2]))
    return p


def split_top(s: str, sep: str) -> list[str]:
    out, depth, cur = [], 0, ""
    i = 0
    while i < len(s):
        if s[i] == "<":
            depth += 1
        elif s[i] == ">":
            depth -= 1
        if depth == 0 and s.startswith(sep, i):
            out.append(cur)
            cur = ""
            i += len(sep)
            continue
        cur += s[i]
        i += 1
    out.append(cur)
    return out


def parse_cxx_name(s: str) -> tuple:
    """'std::vector<Unit *,std::allocator<Unit *> >' as a qualified name of parts."""
    parts = []
    for part in split_top(s.strip(), "::"):
        part = part.strip()
        if "<" in part and part.endswith(">"):
            name, args = part[:part.index("<")], part[part.index("<") + 1:-1]
            parts.append(("tmpl", name.strip(), tuple(parse_cxx_type(a) for a in split_top(args, ","))))
        else:
            parts.append(part)
    return tuple(parts)


def parse_cxx_type(s: str):
    s = s.strip()
    if re.fullmatch(r"-?\d+", s):
        return ("int", int(s))
    if s.endswith("*") or s.endswith("&"):
        inner = parse_cxx_type(s[:-1])
        return ("ptr", inner) if s.endswith("*") else ("ref", inner)
    for word in ("const", "volatile"):
        if s.endswith(" " + word):
            return parse_cxx_type(s[:-len(word) - 1])
        if s.startswith(word + " "):
            return parse_cxx_type(s[len(word) + 1:])
    if s in CXX_PRIMS:
        return ("prim", s)
    for word in ("struct ", "class ", "union "):
        if s.startswith(word):
            return ("named", word.strip(), parse_cxx_name(s[len(word):]))
    return ("named", "class" if "<" in s else "struct", parse_cxx_name(s))


def plain_name(t) -> str | None:
    """The tag name of a named type with one plain part, else None."""
    if t and t[0] == "named" and len(t[2]) == 1 and isinstance(t[2][0], str):
        return t[2][0]
    return None


def sep(inner: str) -> str:
    """The space between a type and its declarator: `int x`, `int* x`, `int[3]`."""
    return "" if not inner or inner[0] in "*&[" else " "


class UnionFind:
    def __init__(self):
        self.parent: dict = {}

    def find(self, x):
        self.parent.setdefault(x, x)
        root = x
        while self.parent[root] != root:
            root = self.parent[root]
        while self.parent[x] != root:
            self.parent[x], x = root, self.parent[x]
        return root

    def union(self, a, b) -> bool:
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False
        if rb < ra:
            ra, rb = rb, ra
        self.parent[rb] = ra
        return True


# --- step 2: which views are one type -------------------------------------------------

FILLER = re.compile(r"^(_*(unknown|unk|pad|padding|gap|filler|reserved|skip|rest|tail|spare)"
                    r"(_?[0-9a-fA-F]+)?_?[0-9a-fA-F]*|_*u[0-9a-fA-F]*|_+)$", re.I)


def is_filler(field: dict) -> bool:
    """A gap the file writes as bytes it does not know (`char unknown_10[8]`)."""
    t = field["type"]
    if "bits" in field or t[0] != "array":
        return False
    inner = t[2]
    while inner[0] == "array":
        inner = inner[2]
    return inner in (("prim", "char"), ("prim", "unsigned char"), ("prim", "signed char")) \
        and bool(FILLER.match(field["name"]))


SUFFIX = re.compile(r"_(?:0x)?[0-9a-fA-F]{6,8}$")
HUBS = 12  # declarations of one symbol each other declaration is joined to
OFFSET_NAME = re.compile(r"^[A-Za-z]*_?(0x)?[0-9a-fA-F]+$|^(unknown|unk|field|pad|gap|data|value)\w*$")


def base_name(name: str) -> str:
    """A view's name without the address suffix files add (`Unit_0041b2e0` is a Unit)."""
    if name.startswith("Class_"):
        return name
    return SUFFIX.sub("", name)


def is_bytes(t) -> bool:
    """An array of chars: a block of bytes, a string or a buffer."""
    if t[0] != "array":
        return False
    while t[0] == "array":
        t = t[2]
    return t in (("prim", "char"), ("prim", "unsigned char"), ("prim", "signed char"))


class Group:
    """The views joined so far as one type: their fields as byte ranges, to test a join."""

    def __init__(self, node: tuple, view: dict, definite: int | None):
        self.nodes = [node]
        self.atoms: dict[tuple, str] = {}
        self.names: dict[tuple, set] = {}
        self.definite = definite
        self.extent = 0
        self.kind = Counter()
        self.add_view(view)

    def add_view(self, view: dict) -> None:
        for a, kind, name in view_atoms(view):
            self.atoms.setdefault(a, kind)
            self.names.setdefault(a, set()).add(name)
            self.extent = max(self.extent, a[1])
        self.kind[view["kind"]] += 1

    def agrees(self, other: "Group") -> bool:
        """Whether two views share fields: two over the same bytes, or one of the
        same name (not one made of its offset) over the same bytes."""
        same = [a for a in other.atoms if a in self.atoms]
        if len(same) >= 2:
            return True
        return any(not OFFSET_NAME.match(n) for a in same for n in self.names[a] & other.names[a])

    def agrees_mostly(self, other: "Group") -> bool:
        """Whether at least two fields, and half of the smaller one's, are over the
        same bytes in both."""
        same = sum(1 for a in other.atoms if a in self.atoms)
        return same >= 2 and 2 * same >= min(len(self.atoms), len(other.atoms))

    def hard_conflict(self, other: "Group") -> str | None:
        """Why the two cannot be one type whatever their names, or None."""
        if self.definite is not None and other.definite is not None and self.definite != other.definite:
            return f"sizes {self.definite:#x} and {other.definite:#x}"
        for d, g in ((self.definite, other), (other.definite, self)):
            if d is not None and g.extent > d:
                return f"a field ends at {g.extent:#x}, past the size {d:#x}"
        if self.kind["union"] and (other.kind["struct"] or other.kind["class"]) \
                or other.kind["union"] and (self.kind["struct"] or self.kind["class"]):
            return "a union and a struct"
        return None

    def field_conflict(self, other: "Group") -> str | None:
        """The first field of one that crosses a field of the other, or None."""
        small, large = (self, other) if len(self.atoms) <= len(other.atoms) else (other, self)
        mine = sorted(large.atoms.items())
        starts = [a[0] for a, _ in mine]
        longest = max((a[1] - a[0] for a, _ in mine), default=0)
        for (lo, hi), kind in small.atoms.items():
            # The atoms that overlap [lo, hi) start before hi and at most `longest` before lo.
            j = bisect.bisect_left(starts, hi)
            i = bisect.bisect_left(starts, lo - longest)
            for a, kb in mine[i:j]:
                if a[1] <= lo or a == (lo, hi):
                    continue
                if not (inside((lo, hi), a, kb) or inside(a, (lo, hi), kind)):
                    return f"[{a[0]:#x},{a[1]:#x}) against [{lo:#x},{hi:#x})"
        return None

    def absorb(self, other: "Group") -> None:
        self.nodes += other.nodes
        for a, kind in other.atoms.items():
            self.atoms.setdefault(a, kind)
            self.names.setdefault(a, set()).update(other.names[a])
        self.extent = max(self.extent, other.extent)
        if self.definite is None:
            self.definite = other.definite
        self.kind.update(other.kind)


def inside(inner: tuple, outer: tuple, kind: str) -> bool:
    """Whether a field over `inner` can lie inside the field over `outer` of this kind."""
    if not (outer[0] <= inner[0] and inner[1] <= outer[1]):
        return False
    if kind == "leaf":
        return False
    if kind.startswith("array:"):
        elem = int(kind[6:])
        rel = inner[0] - outer[0]
        return elem == 0 or rel % elem == 0 and (inner[1] - inner[0]) % elem == 0
    return True  # a struct, union or byte block holds anything


def view_atoms(view: dict) -> list[tuple]:
    out = []
    for f in view["fields"]:
        if is_filler(f) or not f.get("size"):
            continue
        rng = (f["off"], f["off"] + f["size"])
        t = f["type"]
        if "bits" in f:
            kind = "leaf"
        elif t[0] in ("named", "anon"):
            kind = "block"
        elif t[0] == "array":
            inner = t[2]
            while inner[0] == "array":
                inner = inner[2]
            if inner[0] in ("prim", "ptr", "enum", "func") and inner[1:2] not in (("char",), ("unsigned char",)):
                kind = f"array:{f['size'] // max(1, t[1]) if t[2][0] != 'array' else 0}"
            else:
                kind = "block"
        else:
            kind = "leaf"
        out.append((rng, kind, f["name"]))
    if view.get("vfptr"):
        out.append(((0, 4), "leaf", "vftable"))
    return out


class Program:
    """Every file's views, the evidence joining them, and the merged types."""

    def __init__(self, files: dict[str, dict], definers: dict[str, str], declared: set[str],
                 system: set[str] = frozenset()):
        self.files = files
        self.definers = definers
        self.system = system  # names the system headers define: never game types
        self.declared = declared - system  # every name a file declares with struct, class or union
        self.game = set()
        self.views: dict[tuple, dict] = {}
        for stem, f in files.items():
            for name, v in f["views"].items():
                if v["kind"] == "enum":
                    continue  # the two enums the files declare are left to them
                if name in system:
                    continue
                self.game.add(name)
                self.views[(stem, name)] = convert_view(v)
        self.uf = UnionFind()
        self.symbols: dict[tuple, list] = defaultdict(list)
        self.definite: dict[tuple, int] = {}
        for stem, f in files.items():
            for sym in f["defined"] + f["used"]:
                try:
                    d = Decoded(sym)
                except (Undecorate, ValueError, IndexError, KeyError):
                    continue
                own = definers.get(sym) == stem or (sym in f["defined"] and stem not in definers.values())
                self.symbols[self.identity(d)].append((stem, d, sym in f["defined"], own))
                for t in by_value(from_decoded(d.sig)):
                    self.mark_definite(stem, t)
        for (stem, name), v in self.views.items():
            for f in v["fields"]:
                t = f["type"]
                while t[0] == "array":
                    t = t[2]
                self.mark_definite(stem, t)
                if t[0] == "anon":
                    for g in anon_fields(t[1]):
                        u = g["type"]
                        while u[0] == "array":
                            u = u[2]
                        self.mark_definite(stem, u)
            for b in v["bases"]:
                self.mark_definite(stem, b["type"])
        # A name a file only refers to (`struct Unit;`, a pointer in a prototype) is a
        # node without a layout: evidence can still say which type it is.
        self.opaque: set[tuple] = set()
        for stem, f in files.items():
            for sym in f["defined"] + f["used"]:
                for name in re.findall(r"[UVT]([A-Za-z_]\w*)@@", sym):
                    if name in self.game and (stem, name) not in self.views:
                        self.opaque.add((stem, name))
        for (stem, _), v in list(self.views.items()):
            for t in view_types(v):
                name = plain_name(t)
                if name in self.game and (stem, name) not in self.views:
                    self.opaque.add((stem, name))
        self.nodes = set(self.views) | self.opaque
        empty = {"kind": "struct", "size": 0, "fields": [], "bases": [], "vfptr": False,
                 "methods": [], "statics": []}
        self.groups: dict[tuple, Group] = {}
        for node, v in self.views.items():
            self.groups[node] = Group(node, v, self.definite.get(node))
        for node in self.opaque:
            self.groups[node] = Group(node, empty, None)
            self.groups[node].kind.clear()
        self.rejected: list[tuple] = []
        self.refused: set[tuple] = set()
        self.accepted: list[tuple] = []

    def mark_definite(self, stem: str, t) -> None:
        name = plain_name(t)
        if name and (stem, name) in self.views:
            self.definite[(stem, name)] = self.views[(stem, name)]["size"]

    @staticmethod
    def identity(d: Decoded) -> tuple:
        if d.kind == "data":
            return ("data", d.name)
        f = d.sig
        return (d.kind, d.name, len(f[3]), f[4], d.storage == "static")

    def node(self, stem: str, t) -> tuple | None:
        name = plain_name(t)
        if name is None or (stem, name) not in self.nodes:
            return None
        return (stem, name)

    def pairs(self, sa: str, ta, sb: str, tb, out: list, returns: bool = True) -> None:
        """The (node, node) pairs in matching positions of ta (file sa) and tb (file sb)."""
        if ta is None or tb is None or ta[0] != tb[0]:
            return
        k = ta[0]
        if k == "named":
            a, b = self.node(sa, ta), self.node(sb, tb)
            if a and b:
                out.append((a, b))
            elif len(ta[2]) == len(tb[2]):
                for pa, pb in zip(ta[2], tb[2]):
                    if isinstance(pa, tuple) and isinstance(pb, tuple) and pa[1] == pb[1] \
                            and len(pa[2]) == len(pb[2]):
                        for xa, xb in zip(pa[2], pb[2]):
                            self.pairs(sa, xa, sb, xb, out)
        elif k in ("ptr", "ref"):
            self.pairs(sa, ta[1], sb, tb[1], out)
        elif k == "array":
            if ta[1] == tb[1]:
                self.pairs(sa, ta[2], sb, tb[2], out)
        elif k == "func":
            if returns:
                self.pairs(sa, ta[2], sb, tb[2], out)
            if len(ta[3]) == len(tb[3]):
                for xa, xb in zip(ta[3], tb[3]):
                    self.pairs(sa, xa, sb, xb, out)
        elif k == "memfn":
            self.pairs(sa, ("named", "class", ta[1]), sb, ("named", "class", tb[1]), out)
            self.pairs(sa, ta[2], sb, tb[2], out)

    def symbol_edges(self) -> dict[tuple, Counter]:
        """Evidence from the decorated names: {(node, node): {reason: count}}.

        A function's own file (where it is defined) gives its real signature, and each
        caller's declaration is joined to it: parameters and the return type. Callers of
        a function no file defines are joined to each other on parameters only, since a
        caller's return type is often a guess. A global's declarations are all equal."""
        edges: dict[tuple, Counter] = defaultdict(Counter)
        for ident, entries in self.symbols.items():
            if len(entries) < 2:
                continue
            entries = sorted(entries, key=lambda e: e[0])
            label = "::".join(p if isinstance(p, str) else p[1] for p in ident[1])
            owners = [e for e in entries if e[3]] or [e for e in entries if e[2]]
            # Each declaration is joined to a few references, not just one, so that one
            # view that disagrees does not cut the others off.
            if ident[0] == "data":
                refs, why, returns = entries[:HUBS], f"global {label}", True
            elif owners:
                refs, why, returns = owners[:HUBS], f"{label} as its own file declares it", True
            else:
                refs, why, returns = entries[:HUBS], f"{label} in two callers", False
            for sa, da, _, _ in refs:
                for sb, db, _, _ in entries:
                    if sb == sa:
                        continue
                    out: list = []
                    self.pairs(sa, from_decoded(da.sig), sb, from_decoded(db.sig), out, returns)
                    if da.kind == "method":
                        self.pairs(sa, ("named", "class", tuple(fix_part(p) for p in da.name[:-1])),
                                   sb, ("named", "class", tuple(fix_part(p) for p in db.name[:-1])), out)
                    for a, b in out:
                        if a != b:
                            edges[(a, b) if a < b else (b, a)][why] += 1
        return edges

    def field_edges(self) -> dict[tuple, Counter]:
        """Inside each joined type, the types at the same offset of two of its views."""
        edges: dict[tuple, Counter] = defaultdict(Counter)
        for root, g in self.groups.items():
            if len(g.nodes) < 2:
                continue
            by_off = defaultdict(list)
            for node in g.nodes:
                v = self.views.get(node)
                if v is None:
                    continue
                for f in v["fields"]:
                    if "bits" not in f and not is_filler(f):
                        by_off[f["off"]].append((node[0], f["type"], f["size"]))
                for b in v["bases"]:
                    by_off[("base", b["off"])].append((node[0], b["type"], None))
            for off, entries in by_off.items():
                for i in range(len(entries)):
                    for j in range(i + 1, min(len(entries), i + 8)):
                        out: list = []
                        sa, ta, za = entries[i]
                        sb, tb, zb = entries[j]
                        self.field_pairs(sa, ta, za, sb, tb, zb, out)
                        for a, b in out:
                            if a != b:
                                edges[(a, b) if a < b else (b, a)]["fields at one offset"] += 1
        return edges

    def field_pairs(self, sa, ta, za, sb, tb, zb, out) -> None:
        if ta[0] != tb[0]:
            return
        if ta[0] == "named":
            # Two structs embedded at one offset are one type only if they are as large.
            if za is not None and za == zb:
                self.pairs(sa, ta, sb, tb, out)
        elif ta[0] == "array":
            if ta[1] == tb[1] and za == zb and za:
                n = max(ta[1], 1)
                self.field_pairs(sa, ta[2], za // n, sb, tb[2], zb // n, out)
        elif ta[0] in ("ptr", "ref"):
            self.pairs(sa, ta, sb, tb, out)
        elif ta[0] == "anon":
            fa = {f["off"]: f for f in ta[1]["fields"] if "bits" not in f}
            for f in tb[1]["fields"]:
                g = fa.get(f["off"])
                if g is not None and "bits" not in f:
                    self.field_pairs(sa, g["type"], g["size"], sb, f["type"], f["size"], out)

    def apply(self, edges: dict[tuple, Counter], strict: bool = False) -> int:
        """Join along the edges, best supported first, unless the views conflict."""
        n = 0
        for (a, b), why in sorted(edges.items(), key=lambda kv: (-sum(kv[1].values()), kv[0])):
            ra, rb = self.uf.find(a), self.uf.find(b)
            if ra == rb or (a, b) in self.refused:
                continue  # a conflict, once found, stays: groups only grow
            ga, gb = self.groups[ra], self.groups[rb]
            # Views the files give the same name may disagree about a field (the merged
            # type then leaves those bytes unknown); views of different names that
            # disagree are taken to be different types.
            reason = ga.hard_conflict(gb)
            if not reason and not strict and base_name(a[1]) != base_name(b[1]):
                reason = ga.field_conflict(gb)
            elif not reason and strict and not (a[1] == b[1] and a[1].startswith("Class_")):
                # Views joined by name alone must share fields; if they also
                # disagree somewhere, they must share most of the smaller one's.
                conflict = ga.field_conflict(gb)
                if not ga.agrees(gb):
                    continue  # nothing says they are one type (yet: the types may grow)
                if conflict and not ga.agrees_mostly(gb):
                    reason = conflict
            if reason:
                self.rejected.append((a, b, dict(why), reason))
                self.refused.add((a, b))
                continue
            self.merge(ra, rb)
            self.accepted.append((a, b, dict(why)))
            n += 1
        return n

    def name_edges(self) -> dict[tuple, Counter]:
        """Views of one name in several files (`Unit`, or `Unit_0041b2e0`, a file's own
        view of a Unit), where nothing else joined them. Weighted by the size of the
        larger type, so views join the type most files share first."""
        # A type's name is the one most of its views have (without the suffix); each
        # type joins the largest type of its name, its home.
        home: dict[str, tuple] = {}
        named: dict[str, list] = defaultdict(list)
        for root, g in self.groups.items():
            bases = Counter(base_name(n[1]) for n in g.nodes if n in self.views)
            if not bases:
                continue
            top = min(bases, key=lambda b: (-bases[b], b))
            named[top].append(root)
            size = bases[top]
            if top not in home or (size, ) > (home[top][1], ):
                home[top] = (root, size)
        edges: dict[tuple, Counter] = defaultdict(Counter)
        for base, roots in named.items():
            hub = home[base][0]
            a = min(n for n in self.groups[hub].nodes if n in self.views and base_name(n[1]) == base)
            for r in roots:
                if r == hub:
                    continue
                b = min(n for n in self.groups[r].nodes if n in self.views and base_name(n[1]) == base)
                weight = max(len(self.groups[hub].nodes), len(self.groups[r].nodes))
                edges[(a, b) if a < b else (b, a)]["the same name"] = weight
        return edges

    def join_opaque(self) -> None:
        """A name a file only refers to is the type with views of that exact name, when
        exactly one type has them."""
        holders = defaultdict(set)
        for node in self.views:
            holders[node[1]].add(self.uf.find(node))
        for node in sorted(self.opaque):
            root = self.uf.find(node)
            if any(n in self.views for n in self.groups[root].nodes):
                continue
            targets = {self.uf.find(t) for t in holders.get(node[1], set())}
            if len(targets) == 1:
                self.merge(root, next(iter(targets)))

    def merge(self, ra: tuple, rb: tuple) -> None:
        """Join the types at roots ra and rb."""
        ga, gb = self.groups[ra], self.groups[rb]
        self.uf.union(ra, rb)
        root = self.uf.find(ra)
        keep, gone = (ga, rb) if root == ra else (gb, ra)
        keep.absorb(self.groups[gone])
        del self.groups[gone]
        self.groups[root] = keep

    def build(self) -> None:
        self.apply(self.symbol_edges())
        while self.apply(self.field_edges()):
            pass
        while self.apply(self.name_edges(), strict=True):
            while self.apply(self.field_edges()):
                pass
        self.join_opaque()
        self.settle_disputes()
        self.name_types()

    def settle_disputes(self) -> None:
        """A type the evidence says is another, whose views disagree with that one's, is
        left out (only declared, so pointers to it keep a name): when the evidence for
        the join outweighs the views it has of its own."""
        weight: Counter = Counter()
        for a, b, why, reason in self.rejected:
            if set(why) == {"the same name"}:
                continue  # a name alone does not say two views are one type
            ra, rb = self.uf.find(a), self.uf.find(b)
            if ra != rb:
                weight[(ra, rb) if ra < rb else (rb, ra)] += sum(why.values())
        self.left_out: set[tuple] = set()
        self.disputes: list[tuple] = []

        def size(r):
            return sum(1 for n in self.groups[r].nodes if n in self.views)
        for (ra, rb), w in sorted(weight.items(), key=lambda kv: -kv[1]):
            small, big = (ra, rb) if (size(ra), ra) < (size(rb), rb) else (rb, ra)
            if small in self.left_out or big in self.left_out or size(small) == 0 or size(small) > w:
                continue
            self.left_out.add(small)
            self.disputes.append((small, big, w))
        self.forward: set[tuple] = set()
        self.defined_members: dict[tuple, list] = defaultdict(list)
        for sym, stem in self.definers.items():
            try:
                d = Decoded(sym)
            except (Undecorate, ValueError, IndexError, KeyError):
                continue
            if d.kind != "method" or len(d.name) != 2 or not isinstance(d.name[0], str):
                continue
            node = (stem, d.name[0])
            if node in self.nodes:
                self.defined_members[self.uf.find(node)].append((stem, d))

    def layouts(self) -> list["Layout"]:
        self.sizes = {}
        for root, name in self.canon.items():
            if root in self.left_out:
                continue
            g = self.groups[root]
            sizes = Counter(self.views[n]["size"] for n in g.nodes if n in self.views)
            self.sizes[name] = g.definite or max(sizes, key=lambda s: (sizes[s], s))
        out = []
        for root, name in sorted(self.canon.items(), key=lambda kv: kv[1]):
            if root in self.left_out:
                continue
            lay = Layout(self, root, name)
            lay.build()
            out.append(lay)
        return out

    # --- names --------------------------------------------------------------------

    def name_types(self) -> None:
        """Each type with a layout gets the name its views most often use, preferring
        one without an address suffix; the names are unique."""
        self.canon: dict[tuple, str] = {}
        taken: set[str] = set()
        roots = [r for r, g in self.groups.items() if any(n in self.views for n in g.nodes)]
        roots.sort(key=lambda r: (-sum(1 for n in self.groups[r].nodes if n in self.views), r))
        for root in roots:
            names = Counter(n for s, n in self.groups[root].nodes if (s, n) in self.views)
            ranked = sorted(names, key=lambda n: (n.startswith("Class_") or bool(SUFFIX.search(n)),
                                                  -names[n], n))
            pick = next((n for n in ranked if n not in taken), None)
            if pick is None:
                base, k = ranked[0], 2
                while f"{base}_{k}" in taken:
                    k += 1
                pick = f"{base}_{k}"
            taken.add(pick)
            self.canon[root] = pick
        self.taken = taken
        # The types a union may not hold: with a constructor, destructor or vtable.
        self.nontrivial = set()
        for root, name in self.canon.items():
            for node in self.groups[root].nodes:
                v = self.views.get(node)
                if v and (v["vfptr"] or v["bases"] or any(m["name"] in (node[1], "~" + node[1], "operator=")
                                                          for m in v["methods"])):
                    self.nontrivial.add(name)
                    break

    def has_constructor(self, stem: str, t) -> bool:
        while t[0] == "array":
            t = t[2]
        if t[0] == "anon":
            return any(self.has_constructor(stem, f["type"]) for f in t[1]["fields"])
        if t[0] != "named":
            return False
        if t[2] and t[2][0] == "std":
            return True
        name = plain_name(t)
        return name is not None and self.resolve(stem, name) in self.nontrivial

    def resolve(self, stem: str, name: str) -> str | None:
        """The header's name for the type a file calls `name`, or None if it has no views."""
        node = (stem, name)
        if node not in self.nodes:
            return None
        return self.canon.get(self.uf.find(node))

    def defined(self, stem: str, name: str) -> bool:
        """Whether the header defines the type a file calls `name` (not left out)."""
        node = (stem, name)
        return node in self.nodes and self.uf.find(node) in self.canon \
            and self.uf.find(node) not in self.left_out

    # --- types as C++ ---------------------------------------------------------------

    def type_name(self, stem: str, t, member: bool = False) -> str:
        """The header's name of a named type; `member` when a field holds it by value,
        which needs the type defined."""
        if t[0] == "enum":
            return "int"
        qn = t[2]
        if len(qn) == 1 and isinstance(qn[0], str):
            name = qn[0]
            canon = self.resolve(stem, name) if name in self.game else None
            if canon is not None:
                if not self.defined(stem, name):
                    if member:
                        raise Unrepresentable(f"{canon}, left out")
                    self.forward.add((canon, t[1]))
                return canon
            if member and (name in self.game or name in self.declared):
                raise Unrepresentable(f"{name}, never defined")
            if name in self.game or name in self.declared:
                self.forward.add((name, t[1]))
            return name
        # Only the standard containers, with their default arguments, are spelled out;
        # a game type's nested types and templates are not in the header.
        if qn[0] != "std" or len(qn) != 2 or not isinstance(qn[1], tuple):
            if qn == ("std", "string"):
                return "std::string"
            raise Unrepresentable("::".join(p if isinstance(p, str) else p[1] for p in qn))
        tname, args = qn[1][1], list(qn[1][2])
        if tname == "basic_string":
            if args and args[0] == ("prim", "char"):
                return "std::string"
            raise Unrepresentable("std::basic_string")
        keep = STD_DEFAULTS.get(tname)
        if keep is None:
            raise Unrepresentable(f"std::{tname}")
        if len(args) > keep:
            if not self.defaults(tname, args):
                raise Unrepresentable(f"std::{tname} with other arguments")
            args = args[:keep]
        rendered = [str(a[1]) if a[0] == "int" else self.decl(stem, a) for a in args]
        text = ", ".join(rendered)
        return f"std::{tname}<{text}{' ' if text.endswith('>') else ''}>"

    def defaults(self, tname: str, args: list) -> bool:
        """Whether the arguments after the kept ones are the template's defaults."""
        def is_std(a, name, inner):
            return a[0] == "named" and len(a[2]) == 2 and a[2][0] == "std" and isinstance(a[2][1], tuple) \
                and a[2][1][1] == name and list(a[2][1][2]) == [inner]
        if tname in ("vector", "list", "deque"):
            return len(args) == 2 and is_std(args[1], "allocator", args[0])
        if tname in ("set", "multiset"):
            return len(args) == 3 and is_std(args[1], "less", args[0]) and is_std(args[2], "allocator", args[0])
        if tname in ("map", "multimap"):
            return len(args) == 4 and is_std(args[2], "less", args[0]) and is_std(args[3], "allocator", args[1])
        return False

    def decl(self, stem: str, t, inner: str = "", member: bool = False) -> str:
        """The C++ declarator of type t (from file `stem`) around `inner`; `member` for a
        field's own type, which a pointer or a function type does not pass on."""
        k = t[0]
        if k == "prim":
            return f"{t[1]}{sep(inner)}{inner}"
        if k in ("named", "enum"):
            return f"{self.type_name(stem, t, member)}{sep(inner)}{inner}"
        if k == "array":
            return self.decl(stem, t[2], f"{inner}[{t[1]}]", member)
        if k in ("ptr", "ref"):
            mark = "*" if k == "ptr" else "&"
            target = t[1]
            if target[0] == "func":
                _, conv, ret, params, varargs = target
                args = [self.decl(stem, p) for p in params] + (["..."] if varargs else [])
                return self.decl(stem, ret, f"({conv} {mark}{inner})({', '.join(args) or 'void'})")
            if target[0] == "array":
                return self.decl(stem, target, f"({mark}{inner})")
            if target[0] == "anon":
                raise Unrepresentable("pointer to an unnamed struct")
            return self.decl(stem, target, mark + (" " + inner if inner and inner[0] not in "*&" else inner))
        if k == "anon":
            body = self.anon_body(stem, t[1])
            return f"{t[1]['kind']} {{ {body} }}{sep(inner)}{inner}"
        raise Unrepresentable(k)

    def anon_body(self, stem: str, view: dict) -> str:
        """An unnamed struct or union's fields, one line, as the view declares them."""
        out = []
        for f in view["fields"]:
            if "bits" in f:
                out.append(f"{self.decl(stem, f['type'], f['name'], True)} : {f['bits'][1]};")
            else:
                out.append(f"{self.decl(stem, f['type'], f['name'], True)};")
        return " ".join(out)

    def type_key(self, stem: str, t) -> str:
        return self.decl(stem, t, member=True)

    def value_size(self, stem: str, t) -> int | None:
        """The size of a field of type t as the header lays it out, when known."""
        k = t[0]
        if k == "prim":
            return PRIM_SIZE.get(t[1])
        if k in ("ptr", "ref", "enum"):
            return 4
        if k == "array":
            inner = self.value_size(stem, t[2])
            return None if inner is None else inner * t[1]
        if k == "anon":
            return t[1]["size"]
        if k == "named":
            name = plain_name(t)
            canon = self.resolve(stem, name) if name else None
            return self.sizes.get(canon) if canon else None
        return None

    def safe_key(self, stem: str, t) -> str | None:
        try:
            return self.decl(stem, t, member=True)
        except Unrepresentable:
            return None

    @staticmethod
    def field_kind(f: dict) -> str:
        t = f["type"]
        if "bits" in f:
            return "leaf"
        if t[0] in ("named", "anon", "array"):
            return "block"
        return "leaf"

    # --- members --------------------------------------------------------------------

    def methods_of(self, layout: "Layout") -> list[dict]:
        """Each member function: the class bodies' declarations (the form most views
        give), and the decorated names of the members the files define, which win where
        both have one. A dict of its text (without `static` or `virtual`) and flags."""
        root = layout.root
        found: dict[tuple, Counter] = {}
        names = {n for _, n in self.groups[root].nodes}
        for node in layout.nodes:
            stem, cname = node
            v = self.views[node]
            for order, m in enumerate(v["methods"]):
                name = m["name"]
                if name == cname:
                    name = layout.name
                elif name == "~" + cname:
                    name = "~" + layout.name
                elif name.startswith("~") or name in names:
                    continue  # a constructor of another view's name
                sig = m["sig"]
                key = (name, len(sig[3]), sig[4])
                try:
                    text = self.method_text(stem, name, sig, m["mprop"] == 2, ctor=name.lstrip("~") == layout.name)
                except Unrepresentable:
                    continue
                flags = (m["mprop"] == 2, m["mprop"] in (1, 4, 5, 6), m["mprop"] in (5, 6))
                found.setdefault(key, Counter())[(text, flags, order)] += 1
        chosen = {}
        for key, votes in found.items():
            text, flags, order = max(votes, key=lambda k: (votes[k], k[1], -k[2], k[0]))
            chosen[key] = {"text": text, "static": flags[0], "virtual": flags[1], "pure": flags[2],
                           "order": order}
        for stem, d in self.defined_members.get(root, []):
            name = d.name[-1]
            cls = d.name[-2]
            if name == cls:
                name = layout.name
            elif name == "~" + cls:
                name = "~" + layout.name
            f = from_decoded(d.sig)
            key = (name, len(f[3]), f[4])
            static = d.storage == "static"
            try:
                text = self.method_text(stem, name, f, static, ctor=name.lstrip("~") == layout.name)
            except Unrepresentable:
                continue
            old = chosen.get(key)
            chosen[key] = {"text": text, "static": static,
                           "virtual": d.storage == "virtual" or bool(old and old["virtual"]),
                           "pure": False, "order": old["order"] if old else 10000}
        for m in chosen.values():
            m["ctor"] = m["text"].startswith((layout.name + "(", "~"))
        return sorted(chosen.values(), key=lambda m: (not m["ctor"], not m["virtual"], m["order"], m["text"]))

    def method_text(self, stem: str, name: str, sig, static: bool, ctor: bool) -> str:
        _, conv, ret, params, varargs = sig
        args = [self.decl(stem, p) for p in params] + (["..."] if varargs else [])
        # A member's default is __thiscall; a static member's, under /Gz, __stdcall.
        default = "__stdcall" if static else "__thiscall"
        cc = "" if conv == default else f"{conv} "
        head = f"{cc}{name}({', '.join(args) or 'void'})"
        if ctor or ret is None:
            return head
        return self.decl(stem, ret, head)


# --- step 3: one layout per type --------------------------------------------------------

class Unrepresentable(Exception):
    """A type the header cannot spell (a pointer to member, a game template)."""


STD_DEFAULTS = {
    # template: number of leading arguments to keep when the rest are the defaults
    "vector": 1, "list": 1, "deque": 1, "set": 1, "multiset": 1, "map": 2, "multimap": 2,
    "basic_string": 0, "pair": 2, "allocator": 1, "less": 1,
}


class Layout:
    """The merged layout of one game type, ready to render."""

    def __init__(self, program: "Program", root: tuple, name: str):
        self.p = program
        self.root = root
        self.name = name
        group = program.groups[root]
        self.nodes = [n for n in group.nodes if n in program.views]
        kinds = Counter(program.views[n]["kind"] for n in self.nodes)
        self.key = "union" if kinds["union"] > (kinds["struct"] + kinds["class"]) else \
            "class" if kinds["class"] > kinds["struct"] else "struct"
        # The size: the one a view that embeds the type or indexes an array of it
        # gives, else the one most views give (at least the end of the last field).
        sizes = Counter(program.views[n]["size"] for n in self.nodes)
        self.size = group.definite or max(sizes, key=lambda s: (sizes[s], s))
        self.disputed: list[tuple] = []
        self.members: list[tuple] = []
        self.bases: list[str] = []
        self.virtual = any(program.views[n]["vfptr"] for n in self.nodes)
        self.methods: list[tuple] = []
        self.statics: list[tuple] = []

    # The fields, as components of overlapping byte ranges.
    def atoms(self) -> list[tuple]:
        out = []
        for node in self.nodes:
            v = self.p.views[node]
            for i, f in enumerate(v["fields"]):
                if is_filler(f) or not f.get("size"):
                    continue
                out.append((f["off"], f["off"] + f["size"], node, i))
        return out

    def build(self, use_bases: bool = True) -> None:
        self.members, self.disputed, self.bases = [], [], []
        self.collect_methods()
        # A vtable pointer of its own, when a view has one and a virtual member is known.
        self.virtual = any(self.p.views[n]["vfptr"] for n in self.nodes) and \
            any(m["virtual"] for m in self.methods)
        atoms = sorted(self.atoms())
        if self.virtual:
            atoms = [a for a in atoms if a[0] >= 4]  # the vtable pointer
        self.base_end = 4 if self.virtual else 0
        if use_bases:
            self.base_layout()
        start = self.base_end
        atoms = [a for a in atoms if a[0] >= start]
        comps, cur, end = [], [], -1
        for a in atoms:
            if cur and a[0] >= end:
                comps.append(cur)
                cur = []
            cur.append(a)
            end = max(end, a[1]) if len(cur) > 1 else a[1]
        if cur:
            comps.append(cur)
        for comp in comps:
            union = self.union(comp)
            if union is not None:
                self.members.append(union)
                continue
            for top, atoms in self.resolve(comp):
                member = self.component(top, atoms)
                if member is None:
                    self.disputed.append(top)
                else:
                    self.members.append(member)
        self.members.sort(key=lambda m: m[1])
        self.fill_gaps(start)

    def resolve(self, comp: list[tuple]) -> list[tuple]:
        """The fields to keep from a run of overlapping ones: [(interval, atoms in it)].

        Each byte range is decided by how many views declare a field over exactly
        those bytes. A field crossing another loses to it only if fewer views have
        it; when as many have each, both are left out (the bytes stay unknown). A
        struct or array keeps the fields inside it, unless more views have a field
        inside it than have it (a view that groups a run of fields into one struct
        of its own); a char array with other fields inside it is a block of bytes
        the view did not split, and gives way to them."""
        support: dict[tuple, set] = defaultdict(set)
        kind: dict[tuple, str] = {}
        for a in comp:
            iv = (a[0], a[1])
            support[iv].add(a[2])
            f = self.p.views[a[2]]["fields"][a[3]]
            k = "bits" if "bits" in f else self.p.field_kind(f)
            if k == "block" and is_bytes(f["type"]):
                k = "bytes"
            if kind.get(iv) not in (None, k):
                k = "block" if "block" in (k, kind[iv]) else kind[iv]
            kind[iv] = k
        ivs = list(support)
        lumps = {iv for iv in ivs if kind[iv] == "bytes"
                 and any(j != iv and iv[0] <= j[0] and j[1] <= iv[1] for j in ivs)}
        order = sorted((iv for iv in ivs if iv not in lumps),
                       key=lambda iv: (-len(support[iv]), -(iv[1] - iv[0]), iv))
        kept: list[tuple] = []
        blocked: list[tuple] = []
        for iv in order:
            s = len(support[iv])
            if any(b[0] < iv[1] and iv[0] < b[1] for b in blocked):
                continue
            ties = []
            other = False
            for k in kept:
                if k[1] <= iv[0] or iv[1] <= k[0]:
                    continue
                if k[0] <= iv[0] and iv[1] <= k[1] and kind[k] in ("block", "bytes"):
                    other = True  # inside a struct or array already kept
                elif len(support[k]) > s:
                    other = True  # crosses a field more views have
                else:
                    ties.append(k)  # as many views have each: neither is kept
            if ties:
                for k in ties:
                    kept.remove(k)
                blocked += ties + [iv]
            elif not other:
                kept.append(iv)
        for b in blocked:
            self.disputed.append(b)
        out = []
        for iv in sorted(kept):
            atoms = [a for a in comp if iv[0] <= a[0] and a[1] <= iv[1]]
            out.append((iv, atoms))
        return out

    def fill_gaps(self, start: int) -> None:
        out, pos = [], start
        for m in self.members:
            if m[0] == "gap":
                continue
            if m[1] > pos:
                out.append(("gap", pos, m[1] - pos))
            out.append(m)
            pos = m[1] + member_size(m)
        if self.size > pos:
            out.append(("gap", pos, self.size - pos))
        self.size = max(self.size, pos)
        self.members = out

    def base_layout(self) -> None:
        """The base classes, when every view that has fields there agrees on them."""
        self.base_end = 4 if self.virtual else 0
        seen = Counter()
        for node in self.nodes:
            try:
                bases = tuple((b["off"], self.p.type_key(node[0], b["type"]))
                              for b in self.p.views[node]["bases"])
            except Unrepresentable:
                return
            if bases:
                seen[bases] += 1
        if not seen:
            return
        if len(seen) > 1:
            return
        bases = next(iter(seen))
        names, end = [], 0
        for node in self.nodes:
            for b in self.p.views[node]["bases"]:
                try:
                    names.append((b["off"], self.p.type_name(node[0], b["type"], member=True)))
                except Unrepresentable:
                    return
            if self.p.views[node]["bases"]:
                v = self.p.views[node]
                first = min((f["off"] for f in v["fields"]), default=v["size"])
                end = max(end, first)
                break
        self.bases = [n for _, n in sorted(set(names))]
        if self.name in self.bases:
            # A base the evidence joined to its derived class: the views disagree.
            self.bases = []
            return
        self.base_end = max(end, self.base_end)

    def component(self, top: tuple, atoms: list[tuple]) -> tuple | None:
        """The member over the bytes `top`, from the fields the views declare there."""
        cands = [a for a in atoms if (a[0], a[1]) == top]
        fields = [(a[2], self.p.views[a[2]]["fields"][a[3]]) for a in cands]
        if any("bits" in f for _, f in fields):
            return self.bitfields(top, cands)
        return self.choose(top, fields)

    def choose(self, top: tuple, fields: list[tuple]) -> tuple | None:
        votes = Counter()
        names = defaultdict(Counter)
        for node, f in fields:
            try:
                key = self.p.type_key(node[0], f["type"])
            except Unrepresentable:
                continue
            size = self.p.value_size(node[0], f["type"])
            if size is not None and size != top[1] - top[0]:
                continue  # a struct the header lays out larger or smaller than this view
            votes[key] += 1
            names[key][f["name"]] += 1
        if not votes:
            return None
        key = max(votes, key=lambda k: (votes[k], k))
        node, f = next((n, f) for n, f in fields if self.p.safe_key(n[0], f["type"]) == key)
        name = max(names[key], key=lambda k: (names[key][k], k))
        return ("field", top[0], name, node[0], f["type"], top[1] - top[0])

    def bitfields(self, top: tuple, comp: list[tuple]) -> tuple | None:
        per_view = defaultdict(list)
        plain = []
        for a in comp:
            f = self.p.views[a[2]]["fields"][a[3]]
            if (a[0], a[1]) != top:
                return None
            if "bits" in f:
                per_view[a[2]].append(f)
            else:
                plain.append((a[2], f))
        bits: dict[tuple, Counter] = defaultdict(Counter)
        types = Counter()
        for node, fs in per_view.items():
            for f in fs:
                bits[tuple(f["bits"])][f["name"]] += 1
                types[f["type"][1] if f["type"][0] == "prim" else "unsigned int"] += 1
        spans = sorted(bits)
        for (p1, l1), (p2, l2) in zip(spans, spans[1:]):
            if p2 < p1 + l1:
                # Two views split the word differently: keep the word, not its bits.
                return self.choose(top, plain) if plain else None
        width = top[1] - top[0]
        storage = max(types, key=lambda k: (types[k], k))
        if PRIM_SIZE.get(storage) != width:
            storage = {1: "unsigned char", 2: "unsigned short", 4: "unsigned int"}[width]
        out = []
        for span in spans:
            out.append((span[0], span[1], max(bits[span], key=lambda k: (bits[span][k], k))))
        return ("bits", top[0], storage, out, width)

    def union(self, comp: list[tuple]) -> tuple | None:
        """The fields of one view that overlap at one offset, as an anonymous union."""
        by_view = defaultdict(list)
        for a in comp:
            by_view[a[2]].append(a)
        lo, hi = comp[0][0], max(a[1] for a in comp)
        for node, atoms in sorted(by_view.items(), key=lambda kv: -len(kv[1])):
            span = (min(a[0] for a in atoms), max(a[1] for a in atoms))
            if span != (lo, hi) or len(atoms) < 2:
                continue
            # Every other view's fields must equal one of this view's alternatives.
            mine = {(a[0], a[1]) for a in atoms}
            if any((a[0], a[1]) not in mine for a in comp):
                continue
            alts = []
            for a in sorted(atoms, key=lambda a: (a[0], -a[1])):
                f = self.p.views[node]["fields"][a[3]]
                if "bits" in f or a[0] != lo:
                    return None
                try:
                    self.p.type_key(node[0], f["type"])
                except Unrepresentable:
                    return None
                if self.p.has_constructor(node[0], f["type"]):
                    return None  # not allowed in a union
                alts.append(("field", a[0], f["name"], node[0], f["type"], a[1] - a[0]))
            return ("union", lo, alts, hi - lo)
        return None

    def collect_methods(self) -> None:
        self.methods = self.p.methods_of(self)
        # Static data members, by name, in the type most views give them.
        votes: dict[str, Counter] = defaultdict(Counter)
        for node in self.nodes:
            for s in self.p.views[node]["statics"]:
                try:
                    votes[s["name"]][self.p.decl(node[0], s["type"], s["name"])] += 1
                except Unrepresentable:
                    pass
        self.statics = [max(v, key=lambda k: (v[k], k)) for _, v in sorted(votes.items())]


# --- step 4: the header -------------------------------------------------------------------

def value_deps(p: "Program", stem: str, t, out: set) -> None:
    """The header names type t needs complete (by value, not behind a pointer)."""
    k = t[0]
    if k == "array":
        value_deps(p, stem, t[2], out)
    elif k == "anon":
        for f in t[1]["fields"]:
            value_deps(p, stem, f["type"], out)
    elif k == "named":
        qn = t[2]
        if len(qn) == 1 and isinstance(qn[0], str):
            canon = p.resolve(stem, qn[0])
            if canon:
                out.add(canon)
        for part in qn:
            if isinstance(part, tuple) and part[0] == "tmpl":
                for a in part[2]:
                    if isinstance(a, tuple) and a[0] not in ("int",):
                        value_deps(p, stem, a, out)
    elif k in ("ptr", "ref"):
        # A std container of pointers still needs nothing; a pointer needs a declaration.
        pass


def render_layout(p: "Program", lay: "Layout", polymorphic: bool) -> list[str]:
    """The definition; `polymorphic` when the class or a base has a vtable pointer, else
    members the views declare virtual are declared plainly (the vtable is in a base the
    header lays out without one)."""
    used: set[str] = {lay.name}
    for m in lay.methods:
        found = re.match(r".*?(~?\w+)\(", m["text"])
        if found:
            used.add(found.group(1))

    def unique(name: str, off: int) -> str:
        if not name or name in used:
            name = f"{name or 'field'}_{off:x}"
            while name in used:
                name += "_"
        used.add(name)
        return name

    head = f"{lay.key} {lay.name}"
    if lay.bases:
        head += " : " + ", ".join(f"public {b}" for b in lay.bases)
    views = len(lay.nodes)
    lines = [f"{head} {{  // {lay.size:#x} bytes, {views} view{'s' if views != 1 else ''}"]
    if lay.key == "class":
        lines.append("public:")
    lay.checks = []
    for m in lay.members:
        kind, off = m[0], m[1]
        if kind == "gap":
            name = unique(f"unknown_{off:x}", off)
            lay.checks.append((name, off))
            lines.append(f"    char {name}[{m[2]}];")
        elif kind == "field":
            _, _, name, stem, t, _ = m
            name = unique(name, off)
            lay.checks.append((name, off))
            lines.append(f"    {p.decl(stem, t, name, True)};  // +{off:#x}")
        elif kind == "bits":
            _, _, storage, spans, width = m
            pos = 0
            first = True
            for bpos, blen, name in spans:
                if bpos > pos:
                    lines.append(f"    {storage} : {bpos - pos};" + (f"  // +{off:#x}" if first else ""))
                    first = False
                lines.append(f"    {storage} {unique(name, off)} : {blen};" + (f"  // +{off:#x}" if first else ""))
                first = False
                pos = bpos + blen
            if pos < width * 8:
                lines.append(f"    {storage} : {width * 8 - pos};")  # the rest of the word
        elif kind == "union":
            alts = []
            for _, _, name, stem, t, _ in m[2]:
                name = unique(name, off)
                lay.checks.append((name, off))
                alts.append(p.decl(stem, t, name, True) + ";")
            lines.append(f"    union {{ {' '.join(alts)} }};  // +{off:#x}")
    for m in lay.methods:
        text = m["text"]
        if m["static"]:
            text = "static " + text
        elif m["virtual"] and polymorphic:
            text = "virtual " + text + (" = 0" if m["pure"] else "")
        lines.append(f"    {text};")
    for text in lay.statics:
        name = re.search(r"(\w+)(\[\d+\])*$", text)
        if name and name.group(1) in used:
            continue
        lines.append(f"    static {text};")
    lines.append("};")
    return lines


# The system headers the types need, in the order they are included, and what in the
# rendered types needs each (<windows.h> always: the views use its types throughout).
SYSTEM_HEADERS = [
    ("<windows.h>", r""),
    ("<ddraw.h>", r"\b(IDirectDraw\w*|_DD\w+|DDSURFACEDESC|_DDSURFACEDESC)\b"),
    ("<dsound.h>", r"\b(IDirectSound\w*|_DS\w+)\b"),
    ("<dplay.h>", r"\b(IDirectPlay\w*|DPID|_DP\w+|DPNAME)\b"),
    ("<stdio.h>", r"\b_iobuf\b"),
    ("<vector>", r"\bstd::vector<"),
    ("<list>", r"\bstd::list<"),
    ("<map>", r"\bstd::(map|multimap)<"),
    ("<set>", r"\bstd::(set|multiset)<"),
    ("<deque>", r"\bstd::deque<"),
    ("<string>", r"\bstd::string\b"),
    ("<utility>", r"\bstd::pair<"),
]


def write_header(p: "Program", layouts: list["Layout"]) -> tuple[str, dict]:
    """The header's text, with the types in dependency order, and what was left out."""
    by_name = {lay.name: lay for lay in layouts}
    # A base class stays only if the header lays it out as large as the views' derived
    # classes have it (their own fields start where it ends); else the derived class
    # is laid out from its fields alone.
    for lay in layouts:
        if lay.bases:
            sizes = [by_name[b].size if b in by_name and by_name[b].members else 0 for b in lay.bases]
            if any(b not in by_name for b in lay.bases) or sum(sizes) != lay.base_end - (4 if lay.virtual else 0):
                lay.build(use_bases=False)
    poly: dict[str, bool] = {}

    def polymorphic(name: str, seen: frozenset = frozenset()) -> bool:
        if name not in poly:
            lay = by_name.get(name)
            poly[name] = bool(lay) and name not in seen and (
                lay.virtual or any(polymorphic(b, seen | {name}) for b in lay.bases))
        return poly[name]
    rendered: dict[str, list[str]] = {}
    deps: dict[str, set[str]] = {}
    failed: dict[str, str] = {}
    for lay in layouts:
        try:
            rendered[lay.name] = render_layout(p, lay, polymorphic(lay.name))
        except Unrepresentable as why:
            failed[lay.name] = str(why)
            continue
        d: set[str] = set()
        for m in lay.members:
            if m[0] == "field":
                value_deps(p, m[3], m[4], d)
            elif m[0] == "union":
                for alt in m[2]:
                    value_deps(p, alt[3], alt[4], d)
        for b in lay.bases:
            d.add(b)
        d.discard(lay.name)
        deps[lay.name] = d
    # Address order (the first file using a type), deepest dependencies first.
    first = {lay.name: min(n[0] for n in lay.nodes) for lay in layouts}
    import heapq
    waiting = {n: set(deps[n]) for n in rendered}
    users = defaultdict(set)
    for n, d in waiting.items():
        for x in d:
            users[x].add(n)
    heap = [(first[n], n) for n, d in waiting.items() if not d]
    heapq.heapify(heap)
    order, done = [], set()
    while heap:
        _, n = heapq.heappop(heap)
        order.append(n)
        done.add(n)
        for u in users[n]:
            waiting[u].discard(n)
            if not waiting[u]:
                heapq.heappush(heap, (first[u], u))
    left = set(rendered) - done
    for n in sorted(left):
        failed[n] = "needs a type that is left out: " + ", ".join(sorted(deps[n] - done))
    keys = {lay.name: lay.key for lay in layouts}
    lines = [
        "// Generated by tools/gametypes.py from the views in src/unsorted; do not edit.",
        "// One definition of each game type the files declare, merged from their views",
        "// (see the tool). Gaps are bytes no view names; offsets are the views' own.",
        "#ifndef TA_TYPES_H",
        "#define TA_TYPES_H",
        "",
    ]
    body = "\n".join(line for n in order for line in rendered[n])
    needed = [h for h, pattern in SYSTEM_HEADERS if not pattern or re.search(pattern, body)]
    if "<utility>" in needed and any(h in needed for h in ("<vector>", "<list>", "<map>", "<set>", "<deque>")):
        needed.remove("<utility>")  # the containers include it
    lines += [f"#include {h}" for h in needed]
    lines += ["", "#pragma pack(push, 1)", ""]
    forward = sorted({(n, keys[n]) for n in by_name} | {(n, k) for n, k in p.forward if n not in by_name})
    lines += [f"{k} {n};" for n, k in forward]
    lines.append("")
    for n in order:
        lines += rendered[n]
        lines.append("")
    lines += ["#pragma pack(pop)", "", "#endif", ""]
    stats = {"types": len(order), "left out": failed, "forward": len(forward), "order": order}
    return "\n".join(lines), stats


def verify(layouts: list["Layout"], order: list[str]) -> int:
    """Compile the header with a check of every type's size and every field's offset."""
    by_name = {lay.name: lay for lay in layouts}
    body = ['#include "ta_types.h"', "#include <stddef.h>", ""]
    where = {}
    for i, name in enumerate(order):
        lay = by_name[name]
        where[len(body) + 1] = f"sizeof({name}) == {lay.size:#x}"
        body.append(f"typedef char size_{i}[sizeof({name}) == {lay.size:#x} ? 1 : -1];")
        for j, (field, off) in enumerate(lay.checks):
            where[len(body) + 1] = f"{name}::{field} at {off:#x}"
            body.append(f"typedef char off_{i}_{j}[offsetof({name}, {field}) == {off:#x} ? 1 : -1];")
    work = ROOT / "build" / "typeshdr"
    work.mkdir(parents=True, exist_ok=True)
    src = work / "verify.cpp"
    src.write_text("\n".join(body) + "\n")
    obj, log = compile_source(src, out_dir="typeshdr")
    bad = []
    for line in log.splitlines():
        m = re.search(r"verify\.cpp\((\d+)\) : error", line)
        if m:
            bad.append(where.get(int(m.group(1)), line))
        elif "error" in line:
            bad.append(line)
    for b in bad[:40]:
        print(f"  wrong: {b}")
    checks = sum(1 + len(by_name[n].checks) for n in order)
    print(f"verify: {checks - len(bad)} of {checks} sizes and offsets as the views have them"
          + ("" if obj else " (the check file did not compile)"))
    return 1 if bad or obj is None else 0


def member_size(m: tuple) -> int:
    return m[-1] if m[0] in ("field", "bits", "union") else m[2]


def view_types(view: dict):
    """Every type a view's fields, bases, methods and statics name, nested ones too."""
    def walk(t):
        if t is None:
            return
        yield t
        k = t[0]
        if k in ("ptr", "ref"):
            yield from walk(t[1])
        elif k == "array":
            yield from walk(t[2])
        elif k == "func":
            yield from walk(t[2])
            for p in t[3]:
                yield from walk(p)
        elif k == "anon":
            yield from view_types(t[1])
        elif k == "named":
            for part in t[2]:
                if isinstance(part, tuple) and part[0] == "tmpl":
                    for a in part[2]:
                        if isinstance(a, tuple) and a[0] != "int":
                            yield from walk(a)
    for f in view["fields"]:
        yield from walk(f["type"])
    for b in view["bases"]:
        yield from walk(b["type"])
    for m in view["methods"]:
        yield from walk(m["sig"])
    for s in view["statics"]:
        yield from walk(s["type"])


def anon_fields(view: dict) -> list[dict]:
    out = []
    for f in view["fields"]:
        out.append(f)
        if f["type"][0] == "anon":
            out += anon_fields(f["type"][1])
    return out


def by_value(t, under_pointer: bool = False):
    """The named types a decorated type uses by value (not behind a pointer or reference)."""
    if t is None:
        return
    k = t[0]
    if k == "named":
        if not under_pointer:
            yield t
        for part in t[2]:
            if isinstance(part, tuple) and part[0] == "tmpl":
                for a in part[2]:
                    if isinstance(a, tuple) and a[0] != "int":
                        yield from by_value(a, under_pointer)
    elif k in ("ptr", "ref"):
        yield from by_value(t[1], True)
    elif k == "array":
        yield from by_value(t[2], under_pointer)
    elif k == "func":
        yield from by_value(t[2], under_pointer)
        for p in t[3]:
            yield from by_value(p, under_pointer)
    elif k == "memfn":
        yield from by_value(t[2], True)


def defined_names() -> dict[str, set[str]]:
    """The struct, class, union and enum names each file defines: {file stem: names}."""
    out = {}
    for path in sorted(SRC.glob("*.cpp")):
        out[path.stem] = {m.group(2) for m in DEFINED.finditer(path.read_text(errors="replace"))}
    return out


def system_names() -> set[str]:
    """The struct, class and typedef names the toolchain's headers define. A file that
    declares one of them itself (DPNAME, say) is cloning a system type, not a game type."""
    out = set()
    for path in (ROOT / "toolchain" / "msvc5-sp3" / "INCLUDE").iterdir():
        if not path.is_file():
            continue
        text = path.read_text(errors="replace")
        out |= set(re.findall(r"\b(?:struct|class|union)\s+([A-Za-z_]\w*)\s*[{:]", text))
        out |= set(re.findall(r"}\s*([A-Za-z_]\w*)\s*[,;]", text))
        out |= set(re.findall(r"\btypedef\s+(?:struct|class|union)\s+\w+\s+([A-Za-z_]\w*)\s*[,;]", text))
    return out


def declared_names() -> set[str]:
    """Every name a file declares or uses with a class key (`struct Unit;`, `class Foo {`)."""
    out = set()
    for path in sorted(SRC.glob("*.cpp")):
        out |= set(re.findall(r"\b(?:struct|class|union)\s+([A-Za-z_]\w*)", path.read_text(errors="replace")))
    return out


def compile_cached(src: Path) -> Path | None:
    key = hashlib.sha256(src.read_bytes() + FLAGS.encode()).hexdigest()[:16]
    stamp = CACHE / "obj" / (src.stem + ".key")
    obj = stamp.with_suffix(".obj")
    if stamp.exists() and stamp.read_text() == key and obj.exists():
        return obj
    text = src.read_text(errors="replace")
    if re.search(r"^// FLAGS:", text, re.M):
        # /Z7 cannot be combined with /Gi, which changes no layout: compile a copy without it.
        copy = CACHE / "nogi" / src.name
        copy.parent.mkdir(parents=True, exist_ok=True)
        copy.write_text(re.sub(r"^// FLAGS:.*$", "", text, flags=re.M))
        src = copy
    built, _ = compile_source(src, FLAGS, out_dir="typeshdr")
    if built:
        obj.parent.mkdir(parents=True, exist_ok=True)
        obj.write_bytes(built.read_bytes())
    obj = obj if built else None
    if obj:
        stamp.parent.mkdir(parents=True, exist_ok=True)
        stamp.write_text(key)
    return obj


def extract(src: Path, wanted: set[str]) -> dict | None:
    """One file's views and the decorated names it defines or refers to."""
    cache = CACHE / "views" / (src.stem + ".json")
    key = hashlib.sha256(src.read_bytes() + FLAGS.encode() + Path(__file__).read_bytes()).hexdigest()[:16]
    if cache.exists():
        data = json.loads(cache.read_text())
        if data.get("key") == key:
            return data
    obj = compile_cached(src)
    if obj is None:
        return None
    o = parse_object(obj.read_bytes())
    debug_t = next((s for s in o.sections if s.name == ".debug$T"), None)
    views = Views(debug_t.data, wanted).all_views() if debug_t else {}
    defined, used = set(), set()
    for s in o.symbols:
        if not s.name.startswith("?") or s.name.startswith(("??_C@", "??_7", "??_8", "??_R", "??_E", "??_G")):
            continue
        if s.storage_class == 2 and s.section > 0:
            defined.add(s.name)
        elif s.storage_class == 2:
            used.add(s.name)
    data = {"key": key, "views": views, "defined": sorted(defined), "used": sorted(used - defined)}
    cache.parent.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps(data))
    return data


def extract_all(jobs: int) -> dict[str, dict]:
    names = defined_names()
    files = sorted(SRC.glob("*.cpp"))
    with ThreadPoolExecutor(max_workers=jobs) as pool:
        results = list(pool.map(lambda p: extract(p, names[p.stem]), files))
    return {p.stem: r for p, r in zip(files, results) if r is not None}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", type=Path, default=OUT)
    ap.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 4))
    ap.add_argument("--dump", type=Path, help="write the extracted views as JSON and stop")
    ap.add_argument("--stats", action="store_true", help="list the types left out and the joins refused")
    ap.add_argument("--explain", nargs="*", help="list the views of these types and what joined them")
    ap.add_argument("--verify", action="store_true",
                    help="compile the header with a check of every size and field offset")
    args = ap.parse_args()
    files = extract_all(args.jobs)
    if args.dump:
        args.dump.write_text(json.dumps(files))
        nviews = sum(len(f["views"]) for f in files.values())
        print(f"{len(files)} files, {nviews} views -> {args.dump}")
        return 0
    definers = {r["symbol"]: Path(r["file"]).stem for r in csv.DictReader(PROGRESS.open())}
    p = Program(files, definers, declared_names(), system_names())
    p.build()
    layouts = p.layouts()
    text, stats = write_header(p, layouts)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(text)
    nviews = sum(len(lay.nodes) for lay in layouts)
    disputed = sum(1 for lay in layouts if lay.disputed)
    print(f"{args.out.relative_to(ROOT) if args.out.is_relative_to(ROOT) else args.out}: "
          f"{stats['types']} types from {nviews} views, {stats['forward']} forward declarations")
    print(f"  {len(p.rejected)} joins refused, {disputed} types with bytes the views disagree on")
    print(f"  left out: {len(stats['left out'])}, and {len(p.disputes)} groups of views whose evidence "
          f"says they are another type their fields disagree with")
    for name in args.explain or []:
        lay = next((lay for lay in layouts if lay.name == name), None)
        if lay is None:
            print(f"{name}: not in the header")
            continue
        print(f"{name}: {len(lay.nodes)} views, size {lay.size:#x}, disputed {lay.disputed[:20]}")
        for node in sorted(lay.nodes, key=lambda n: -p.views[n]["size"]):
            v = p.views[node]
            print(f"  {node[0]} {node[1]}: size {v['size']:#x}, {len(v['fields'])} fields"
                  + (" (definite)" if node in p.definite else ""))
        for a, b, why in p.accepted:
            if p.uf.find(a) == lay.root and (len(why) > 0):
                print(f"  joined {a} {b}: {why}")
    if args.stats:
        for small, big, w in p.disputes:
            names = sorted({n for _, n in p.groups[small].nodes})
            print(f"    disputed: {', '.join(names)} -> {p.canon.get(big, big)} (evidence {w})")
        for name, why in sorted(stats["left out"].items()):
            print(f"    {name}: {why}")
        for a, b, why, reason in p.rejected:
            print(f"    not joined: {a[1]} ({a[0]}) and {b[1]} ({b[0]}): {reason}; {dict(why)}")
    if args.verify:
        return verify(layouts, stats["order"])
    return 0


if __name__ == "__main__":
    sys.exit(main())
