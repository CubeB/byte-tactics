"""Measure how far the source tree is from linking into one executable.

    uv run tools/linkcheck.py              # summary and the linkability line
    uv run tools/linkcheck.py --verbose    # also every symbol behind each count
    uv run tools/linkcheck.py --json build/linkcheck.json   # the counts, for tracking

Every file under src/ is compiled the way tools/progress.py compiles it (its
cache in build/progress is reused, so only files changed since the last run are
compiled), and the objects' COFF symbol tables are read the way LINK would read
them. Three things are measured:

  undefined symbols     referenced by some object and defined by none, sorted
                        into game functions, game globals, Win32 imports,
                        CRT/library and other; then what is left once the
                        libraries the objects ask for (LIBCMT, LIBCPMT,
                        OLDNAMES) and the import libraries of the DLLs the
                        original imports have supplied theirs
  duplicate definitions one name defined in more than one object: COMDAT folds
                        (inline and template code, string literals, vtables:
                        the linker keeps one) apart from real conflicts
                        (LNK2005)
  global types          for every global address, the types the source
                        declares for it, read from the mangled names of the
                        data symbols (`?DAT_00512344@@3PAUFoo@@A` is a `Foo*`)
                        and, for arrays, from the declarations themselves

Nothing here changes how a function compiles; docs/linking.md explains the
numbers and what to do about them.
"""

import argparse
import csv
import hashlib
import json
import os
import re
import struct
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
from pathlib import Path

from check import DEFAULT_FLAGS, ROOT, base_name, load_symbols
from sources import link_order, source_files

SRC = ROOT / "src"
LIBDIR = ROOT / "toolchain/msvc5-sp3/LIB"
PROGRESS = ROOT / "data/progress.csv"
FUNCTIONS = ROOT / "data/functions.csv"
CACHE = ROOT / "build/progress"

# The import libraries for the DLLs the original imports. WIN32.dll in the
# exe's import table is WINMM.dll renamed by a later patch; smackw32.dll has no
# import library in the toolchain, and the game imports it (and DPLAYX.dll) by
# ordinal.
IMPORT_LIBS = ("KERNEL32", "USER32", "GDI32", "ADVAPI32", "DDRAW", "DSOUND", "DPLAY", "SHELL32",
               "WINMM")
CRT_LIBS = ("LIBCMT", "LIBCPMT", "OLDNAMES")

IMAGE_SCN_LNK_COMDAT = 0x1000
IMAGE_SCN_CNT_CODE = 0x20
SEL_NODUPLICATES = 1  # a COMDAT that must not be defined twice (any other selection folds)
REL_I386_DIR32 = 0x06
PLACEHOLDER = re.compile(r"(?:DAT|FUN|PTR|LAB)_([0-9a-f]{8})$")


# --- reading objects ------------------------------------------------------------

@dataclass
class Definition:
    name: str
    code: bool         # in a code section
    section: str       # .text, .data, .bss, .rdata, ...
    comdat: int        # COMDAT selection (0: not a COMDAT)
    size: int          # of the section holding it, or a common symbol's size
    fingerprint: str   # contents of a COMDAT section, to tell identical folds apart


@dataclass
class ObjectInfo:
    src: str                                        # path relative to the root
    defs: dict[str, Definition] = field(default_factory=dict)
    refs: dict[str, bool] = field(default_factory=dict)       # undefined name -> is a function
    libs: list[str] = field(default_factory=list)              # -defaultlib: directives
    addends: dict[str, tuple[int, int]] = field(default_factory=dict)  # data symbol -> (min, max) offset used


def read_object(src: str, data: bytes) -> ObjectInfo:
    """The external symbols of one COFF object (coff.py skips the auxiliary
    records, which hold each COMDAT section's selection)."""
    info = ObjectInfo(src)
    _, nsects, _, symptr, nsyms, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
    strtab = data[symptr + nsyms * 18:]

    def name_of(raw: bytes) -> str:
        if raw[:4] == b"\0\0\0\0":
            (off,) = struct.unpack_from("<I", raw, 4)
            return strtab[off:strtab.index(b"\0", off)].decode("latin-1")
        return raw[:8].split(b"\0", 1)[0].decode("latin-1")

    sections = []
    for s in range(nsects):
        hdr = data[20 + opthdr + s * 40: 20 + opthdr + s * 40 + 40]
        sname = hdr[:8].split(b"\0", 1)[0].decode("latin-1")
        size, rawptr, relptr, _, nrel, _, chars = struct.unpack_from("<IIIIHHI", hdr, 16)
        sections.append((sname, size, rawptr, relptr, nrel, chars))
        if sname == ".drectve" and rawptr:
            text = data[rawptr:rawptr + size].decode("latin-1")
            info.libs += [m.upper() for m in re.findall(r"-defaultlib:(\S+)", text, re.I)]

    names: dict[int, str] = {}
    selection: dict[int, int] = {}
    externals = []
    i = 0
    while i < nsyms:
        raw = data[symptr + i * 18: symptr + i * 18 + 18]
        value, secnum, typ, sclass, naux = struct.unpack_from("<IhHBB", raw, 8)
        name = name_of(raw)
        names[i] = name
        if sclass == 3 and naux and secnum > 0 and value == 0 and name.startswith("."):
            aux = data[symptr + (i + 1) * 18: symptr + (i + 2) * 18]
            if sections[secnum - 1][5] & IMAGE_SCN_LNK_COMDAT:
                selection[secnum] = aux[14]
        elif sclass == 2:
            externals.append((name, value, secnum, typ))
        i += 1 + naux

    def fingerprint(secnum: int) -> str:
        sname, size, rawptr, relptr, nrel, _ = sections[secnum - 1]
        body = bytearray(data[rawptr:rawptr + size]) if rawptr else bytearray(size)
        targets = []
        for r in range(nrel):
            off, symidx, rtype = struct.unpack_from("<IIH", data, relptr + r * 10)
            target = names.get(symidx, "?")
            # File-local labels ($SG..., section symbols) are named per object.
            targets.append(f"{off}:{rtype}:{'<local>' if target.startswith(('$', '.')) else target}")
            if rtype == REL_I386_DIR32:
                continue  # the addend stays: it says which part of the target is meant
            body[off:off + 4] = b"\0\0\0\0"
        return hashlib.sha1(bytes(body) + "|".join(targets).encode()).hexdigest()[:16]

    data_symbols = set()
    for name, value, secnum, typ in externals:
        if secnum > 0:
            sname, size, _, _, _, chars = sections[secnum - 1]
            sel = selection.get(secnum, 0)
            info.defs[name] = Definition(name, bool(chars & IMAGE_SCN_CNT_CODE), sname, sel, size,
                                         fingerprint(secnum) if sel else "")
            if not chars & IMAGE_SCN_CNT_CODE:
                data_symbols.add(name)
        elif secnum == 0 and value:
            # A communal ("common") symbol: an uninitialised definition of `value` bytes.
            info.defs[name] = Definition(name, False, ".bss", 0, value, "common")
            data_symbols.add(name)
        elif secnum == 0:
            is_func = (typ & 0x30) == 0x20
            info.refs[name] = is_func
            if not is_func:
                data_symbols.add(name)

    # Which part of each global the code uses: a reference to DAT_x + 8 is
    # written as a relocation against DAT_x with 8 stored in the field.
    for sname, size, rawptr, relptr, nrel, _ in sections:
        if not rawptr:
            continue
        for r in range(nrel):
            off, symidx, rtype = struct.unpack_from("<IIH", data, relptr + r * 10)
            target = names.get(symidx)
            if rtype != REL_I386_DIR32 or target not in data_symbols or off + 4 > size:
                continue
            (addend,) = struct.unpack_from("<i", data, rawptr + off)
            lo, hi = info.addends.get(target, (addend, addend))
            info.addends[target] = (min(lo, addend), max(hi, addend))
    return info


def include_hash() -> str:
    """The same hash tools/progress.py folds into its cache keys."""
    return hashlib.sha256(b"".join(
        p.read_bytes() for p in sorted((ROOT / "include").rglob("*")) if p.is_file())).hexdigest()


def load_objects(quiet: bool = False) -> tuple[list[ObjectInfo], list[str]]:
    """Every source file's object, compiled through tools/progress.py's cache.
    Returns the objects and the files that failed to compile."""
    from progress import compile_cached  # imported here: it pulls in the issue tooling

    # In link order (tools/sources.py): the counts and the type each global
    # gets do not depend on where the files live.
    sources = link_order(source_files())
    ihash = include_hash()

    def cached(src: Path) -> bool:
        key = hashlib.sha256(src.read_bytes() + ihash.encode() + DEFAULT_FLAGS.encode()).hexdigest()[:16]
        stamp = CACHE / src.relative_to(SRC).with_suffix(".key")
        return stamp.exists() and stamp.read_text() == key and stamp.with_suffix(".obj").exists()

    stale = [s for s in sources if not cached(s)]
    if stale and not quiet:
        print(f"compiling {len(stale)} file(s) not in build/progress ...", file=sys.stderr)
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        compiled = dict(zip(sources, pool.map(lambda s: compile_cached(s, ihash), sources)))
    objects, failed = [], []
    for src in sources:
        obj, _ = compiled[src]
        if obj is None:
            failed.append(str(src.relative_to(ROOT)))
            continue
        objects.append(read_object(str(src.relative_to(ROOT)), obj.read_bytes()))
    return objects, failed


def archive_symbols(path: Path) -> list[str]:
    """The public symbols a .lib offers, from its first linker member."""
    data = path.read_bytes()
    if data[:8] != b"!<arch>\n" or data[8:24].rstrip() != b"/":
        return []
    size = int(data[56:66])
    body = data[68:68 + size]
    (count,) = struct.unpack_from(">I", body, 0)
    return [n.decode("latin-1") for n in body[4 + 4 * count:].split(b"\0")[:count]]


def library_symbols(wanted: set[str]) -> dict[str, str]:
    """Symbol -> library, for the CRT libraries the objects ask for and the
    import libraries of the DLLs the original imports."""
    out: dict[str, str] = {}
    for lib in sorted(set(CRT_LIBS) | wanted) + list(IMPORT_LIBS):
        path = LIBDIR / f"{lib}.LIB"
        if path.exists():
            for name in archive_symbols(path):
                out.setdefault(name, lib)
    return out


# --- MSVC type names --------------------------------------------------------------
#
# Enough of the Visual C++ 5 name mangling to read the type of a global:
# `?name@scope@@<storage><type><cv>`. Types are tuples:
#   ("prim", "int")  ("rec", "struct", "Foo")  ("ptr", pointee_cv, pointee, own_cv)
#   ("ref", cv, inner)  ("arr", [dims], inner)  ("fn", callconv, ret, [args])  ("raw", text)
# An array variable mangles exactly like a pointer (`char x[]` and `char* x`
# are both `PAD`), so arrays come from the declarations (see source_arrays).

PRIMS = {"C": "signed char", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short",
         "H": "int", "I": "unsigned int", "J": "long", "K": "unsigned long", "M": "float",
         "N": "double", "O": "long double", "X": "void", "_J": "__int64", "_K": "unsigned __int64",
         "_N": "bool", "_W": "wchar_t"}
PRIM_SIZE = {"signed char": 1, "char": 1, "unsigned char": 1, "short": 2, "unsigned short": 2,
             "int": 4, "unsigned int": 4, "long": 4, "unsigned long": 4, "float": 4, "double": 8,
             "long double": 8, "__int64": 8, "unsigned __int64": 8, "bool": 1, "wchar_t": 2}
CVS = {"A": "", "B": "const", "C": "volatile", "D": "const volatile"}
CALLCONV = {"A": "__cdecl", "E": "__thiscall", "G": "__stdcall", "I": "__fastcall"}
RECORDS = {"U": "struct", "V": "class", "T": "union"}


class Demangle:
    def __init__(self, text: str):
        self.s, self.i = text, 0
        self.names: list[str] = []
        self.args: list[tuple] = []

    def take(self) -> str:
        c = self.s[self.i]
        self.i += 1
        return c

    def number(self) -> int:
        neg = self.s[self.i] == "?"
        if neg:
            self.i += 1
        c = self.take()
        if c.isdigit():
            n = int(c) + 1
        else:
            n = 0
            while c != "@":
                n = n * 16 + ord(c) - ord("A")
                c = self.take()
        return -n if neg else n

    def fragment(self) -> str:
        c = self.s[self.i]
        if c.isdigit():
            self.i += 1
            return self.names[int(c)]
        if self.s.startswith("?$", self.i):
            self.i += 2
            end = self.s.index("@", self.i)
            tname = self.s[self.i:end]
            self.i = end + 1
            outer = self.names, self.args
            self.names, self.args = [tname], []
            targs = []
            while self.s[self.i] != "@":
                targs.append(self.arg())
            self.i += 1
            self.names, self.args = outer
            inside = ", ".join(declare(t) for t in targs)
            frag = f"{tname}<{inside}{' ' if inside.endswith('>') else ''}>"  # `> >` for C++98
            self.names.append(frag)
            return frag
        end = self.s.index("@", self.i)
        frag = self.s[self.i:end]
        self.i = end + 1
        self.names.append(frag)
        return frag

    def qualified(self) -> str:
        parts = [self.fragment()]
        while self.s[self.i] != "@":
            parts.append(self.fragment())
        self.i += 1
        return "::".join(reversed(parts))

    def arg(self) -> tuple:
        if self.s[self.i].isdigit():
            return self.args[int(self.take())]
        start = self.i
        t = self.type()
        if self.i - start > 1:
            self.args.append(t)
        return t

    def type(self) -> tuple:
        c = self.take()
        if c == "_":
            return ("prim", PRIMS["_" + self.take()])
        if c in PRIMS:
            return ("prim", PRIMS[c])
        if c in RECORDS:
            return ("rec", RECORDS[c], self.qualified())
        if c == "W":
            self.take()  # the enum's underlying type
            return ("rec", "enum", self.qualified())
        if c in "PQRS":
            own = {"P": "", "Q": "const", "R": "volatile", "S": "const volatile"}[c]
            if self.s[self.i] == "6":
                self.i += 1
                return ("ptr", "", self.function(), own)
            if self.s[self.i] == "8":
                raise ValueError("pointer to member")
            cv = CVS[self.take()]
            if self.s[self.i] == "Y":
                self.i += 1
                dims = [self.number() for _ in range(self.number())]
                return ("ptr", cv, ("arr", dims, self.type()), own)
            return ("ptr", cv, self.type(), own)
        if c == "A":
            return ("ref", CVS[self.take()], self.type())
        if c.isdigit():
            return self.args[int(c)]
        raise ValueError(f"type code {c!r}")

    def function(self) -> tuple:
        cc = CALLCONV.get(self.take(), "")
        ret = self.type()
        args = []
        while self.s[self.i] not in "@Z":
            if self.s[self.i] == "X" and not args:
                self.i += 1
                break
            args.append(self.arg())
        if self.s[self.i] == "@":
            self.i += 1
        if self.i < len(self.s) and self.s[self.i] == "Z":
            self.i += 1
        return ("fn", cc, ret, args)


MEMBER_STATIC = set("CDKLST")
MEMBER_VIRTUAL = set("EFMNUV")


def function_signature(sym: str) -> tuple[str, str] | None:
    """(qualified name, signature) of a C++ function symbol, or None. The
    signature covers what the mangled name says about the function besides
    its name: member kind (static, virtual), `this` qualifiers, calling
    convention, return and parameter types."""
    if not sym.startswith("?"):
        return None
    d = Demangle(sym[1:])
    try:
        if d.s.startswith("?"):
            d.i = 1
            code = d.take()
            if code == "_":
                code += d.take()
            if code == "$":
                return None  # a template function's name
            scope = "" if d.s[d.i] == "@" else d.qualified()
            if d.s[d.i] == "@":
                d.i += 1
            name = f"{scope}::operator{code}" if scope else f"operator{code}"
        else:
            name = d.qualified()
        access = d.take()
        if access in "YZ":
            kind, this = "global", ""
        elif access in MEMBER_STATIC:
            kind, this = "static", ""
        else:
            kind = "virtual" if access in MEMBER_VIRTUAL else "member"
            this = CVS[d.take()]
        cc = CALLCONV.get(d.take(), "?")
        if d.s[d.i] == "@":
            d.i += 1
            ret = None
        else:
            if d.s.startswith("?A", d.i):
                d.i += 2
            ret = d.type()
        args = []
        variadic = False
        while d.i < len(d.s) and d.s[d.i] not in "@Z":
            if d.s[d.i] == "X" and not args:
                d.i += 1
                break
            args.append(d.arg())
        if d.s[d.i:d.i + 2] == "ZZ":
            variadic = True
        sig = (f"{kind} {this} {cc} {declare(ret) if ret else ''}"
               f"({', '.join(declare(a) for a in args)}{', ...' if variadic else ''})")
        shape_ = (f"{kind} {this} {cc} {shape(ret) if ret else ''}"
                  f"({', '.join(shape(a) for a in args)}{', ...' if variadic else ''})")
        return name, sig + "\0" + shape_
    except (ValueError, IndexError, KeyError):
        return None


def signature_difference(a: str, b: str) -> str:
    """How two spellings of one function differ: 'struct names' when the
    signatures agree once every struct, class and union name is blanked."""
    fa, fb = function_signature(a), function_signature(b)
    if not fa or not fb:
        return "signature"
    if fa[1].split("\0")[1] == fb[1].split("\0")[1]:
        return "struct names"
    return "signature"


def data_symbol(sym: str) -> tuple[str, tuple | None]:
    """(qualified name, type) of a data symbol; type is None for an
    extern "C" name, which carries none, or one this reader can't parse."""
    if not sym.startswith("?"):
        return sym[1:] if sym.startswith("_") else sym, None
    d = Demangle(sym[1:])
    try:
        name = d.qualified()
        storage = d.take()
        if storage not in "01234":
            return name, None
        t = d.type()
        cv = CVS.get(d.s[d.i], "") if d.i < len(d.s) else ""
        if cv and t[0] in ("prim", "rec"):
            t = ("cv", cv, t)
        return name, t
    except (ValueError, IndexError):
        return base_name(sym), None


def _join(tokens: list[str]) -> str:
    out = ""
    for tok in tokens:
        if not tok:
            continue
        prev = out[-1:] if out else ""
        word = tok[0].isalnum() or tok[0] == "_"
        if out and ((word and (prev.isalnum() or prev in "_*>")) or (tok == "(" and (prev.isalnum() or prev == ">"))
                    or (tok == "*" and prev == ",")):
            out += " "
        out += tok
    return out


def declare(t: tuple | None, name: str = "") -> str:
    """C declaration of `name` with type t (an abstract declarator without a name)."""
    return _join(_declare(t, [name] if name else []))


def _declare(t, inner: list[str], cv: str = "") -> list[str]:
    if t is None:
        return ["?"] + inner
    k = t[0]
    if k == "cv":
        return _declare(t[2], inner, " ".join(x for x in (t[1], cv) if x))
    if k == "prim":
        return ([cv] if cv else []) + [t[1]] + inner
    if k == "rec":
        return ([cv] if cv else []) + [t[2]] + inner
    if k == "raw":
        return [t[1]] + inner
    if k in ("ptr", "ref"):
        star = "*" if k == "ptr" else "&"
        own = " ".join(x for x in ((t[3] if k == "ptr" else ""), cv) if x)
        pointee = t[2]
        pcv = t[1]
        mid = [star] + ([own] if own else []) + inner
        if pointee[0] in ("fn", "arr"):
            return _declare(pointee, ["("] + mid + [")"], pcv)
        return _declare(pointee, mid, pcv)
    if k == "arr":
        dims = "".join(f"[{d}]" if d else "[]" for d in t[1])
        return _declare(t[2], inner + [dims], cv)
    if k == "fn":
        _, cc, ret, args = t
        if inner and inner[0] == "(":
            inner = ["(", cc] + inner[1:] if cc else inner
        arglist = ", ".join(declare(a) for a in args) or "void"
        return _declare(ret, inner + [f"({arglist})"])
    return ["?"] + inner


def type_size(t: tuple | None) -> int | None:
    if t is None:
        return None
    k = t[0]
    if k == "cv":
        return type_size(t[2])
    if k == "prim":
        return PRIM_SIZE.get(t[1])
    if k in ("ptr", "ref"):
        return 4
    if k == "arr":
        inner = type_size(t[2])
        if inner is None or not t[1] or not all(t[1]):
            return None
        n = inner
        for d in t[1]:
            n *= d
        return n
    return None


def shape(t: tuple | None, loose: bool = False) -> str:
    """The type with every struct, class, union and enum name blanked; with
    `loose`, also without signedness and const/volatile."""
    if t is None:
        return "?"
    k = t[0]
    cv = (lambda q: "") if loose else (lambda q: q)
    if k == "rec":
        return "<record>"
    if k == "prim" and loose:
        return {"long": "int", "unsigned long": "int"}.get(t[1], t[1]).replace("unsigned ", "").replace("signed ", "")
    if k == "cv":
        return f"{cv(t[1])} {shape(t[2], loose)}"
    if k == "ptr":
        return f"{cv(t[1])} {shape(t[2], loose)}*"
    if k == "ref":
        return f"{cv(t[1])} {shape(t[2], loose)}&"
    if k == "arr":
        return f"{shape(t[2], loose)}[]"  # extents may legitimately be left out
    if k == "fn":
        return f"{t[1]} {shape(t[2], loose)}({', '.join(shape(a, loose) for a in t[3])})"
    return declare(t)


def records(t: tuple | None) -> list[tuple[str, str]]:
    """The (keyword, name) of every struct, class, union or enum a type names."""
    if t is None:
        return []
    k = t[0]
    if k == "rec":
        return [(t[1], t[2])]
    if k == "cv":
        return records(t[2])
    if k in ("ptr", "ref", "arr"):
        return records(t[2])
    if k == "fn":
        return records(t[2]) + [r for a in t[3] for r in records(a)]
    return []


# --- the declarations themselves -------------------------------------------------

COMMENTS = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
STATEMENT = re.compile(r"[^;{}]*;")


def source_arrays(path: Path) -> dict[str, list[int]]:
    """Name -> array extents (0 for []) for every array a file declares
    `extern` (or `static` inside a class), at file scope or in a class."""
    text = COMMENTS.sub(" ", path.read_text(errors="replace"))
    text = re.sub(r"(?m)^\s*#.*$", ";", text)  # preprocessor lines end no statement
    text = re.sub(r"\b(public|private|protected)\s*:", ";", text)
    text = re.sub(r'extern\s+"C"\s*\{', "{", text)
    out = {}
    for m in STATEMENT.finditer(text):
        stmt = m.group().strip()
        stmt = re.sub(r'^extern\s+"C"\s+', "extern ", stmt)
        if not re.match(r"(extern|static)\b", stmt):
            continue
        if "(" in stmt.split("[", 1)[0] and "(*" not in stmt:
            continue  # a function's declaration
        for decl in re.finditer(r"\b([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)+)(?=[,;=])", stmt):
            dims = []
            for d in re.findall(r"\[([^\]]*)\]", decl.group(2)):
                d = d.strip()
                try:
                    dims.append(int(d, 0) if d else 0)
                except ValueError:
                    dims.append(0)
            out[decl.group(1)] = dims
    return out


def with_arrays(t: tuple | None, dims: list[int] | None) -> tuple | None:
    """A pointer type turned back into the array the file declared."""
    if not dims or t is None or t[0] != "ptr":
        return t
    inner = t[2]
    if inner[0] == "arr" and len(dims) == 1 + len(inner[1]):
        return ("arr", dims[:1] + inner[1], inner[2])
    pointee = ("cv", t[1], inner) if t[1] and inner[0] in ("prim", "rec") else inner
    return ("arr", dims[:1], pointee)


# --- the analysis -----------------------------------------------------------------

def load_known() -> tuple[dict[str, int], dict[int, tuple[str, int]], dict[int, tuple[str, str]]]:
    """data/symbols.csv, data/functions.csv (address -> (kind, size)), and
    data/progress.csv (address -> (file, mangled symbol) of every annotated
    definition)."""
    symbols = load_symbols()
    with FUNCTIONS.open() as fh:
        kinds = {int(r["address"], 16): (r["kind"], int(r["size"] or 0)) for r in csv.DictReader(fh)}
    annotated = {}
    if PROGRESS.exists():
        with PROGRESS.open() as fh:
            for r in csv.DictReader(fh):
                if r["symbol"] and r["status"] in ("matched", "partial"):
                    annotated[int(r["address"], 16)] = (r["file"], r["symbol"])
    return symbols, kinds, annotated


def address_of(sym: str, symbols: dict[str, int]) -> int | None:
    """The address a symbol names: its DAT_/FUN_ placeholder, or data/symbols.csv."""
    if sym.startswith("__imp_"):
        return None
    name = base_name(sym)
    if "::" in name and name in symbols:
        # A method named after the base class's virtual it overrides
        # (Class_0044f010::FUN_0044ef40 is at 0x44f150): its definition's
        # address, which data/symbols.csv learns from the match.
        return symbols[name]
    m = PLACEHOLDER.search(name.split("::")[-1])
    if m:
        return int(m.group(1), 16)
    if name in symbols:
        return symbols[name]
    if not sym.startswith("?"):
        return symbols.get(sym)
    return None


@dataclass
class Report:
    objects: int = 0
    failed: list[str] = field(default_factory=list)
    referenced: int = 0
    undefined: dict[str, dict[str, list]] = field(default_factory=dict)   # category -> sub -> [lines]
    unresolved: int = 0
    duplicates: dict[str, dict[str, list]] = field(default_factory=dict)
    real_duplicates: int = 0
    globals: dict[str, list] = field(default_factory=dict)
    global_count: int = 0
    libraries_overridden: list = field(default_factory=list)


def analyse(objects: list[ObjectInfo]) -> tuple[Report, dict]:
    symbols, kinds, annotated = load_known()
    rep = Report(objects=len(objects))
    defs: dict[str, list[tuple[str, Definition]]] = defaultdict(list)
    refs: dict[str, list[str]] = defaultdict(list)
    is_func: dict[str, bool] = {}
    wanted_libs = set()
    for o in objects:
        wanted_libs.update(o.libs)
        for name, d in o.defs.items():
            defs[name].append((o.src, d))
        for name, f in o.refs.items():
            refs[name].append(o.src)
            is_func[name] = f
    libs = library_symbols(wanted_libs)

    # Definitions by address, to say whether an undefined name is a spelling
    # problem (the function exists under another name) or a missing body.
    defined_at: dict[int, set[str]] = defaultdict(set)
    for addr, (_, sym) in annotated.items():
        if sym in defs:
            defined_at[addr].add(sym)
    for name, ds in defs.items():
        if ds[0][1].code:
            a = address_of(name, symbols)
            if a is not None:
                defined_at[a].add(name)

    from check import Original
    orig = Original()
    imports_by_name = {e.name.decode() for d in orig.pe.DIRECTORY_ENTRY_IMPORT for e in d.imports if e.name}
    import_slots = {e.address for d in orig.pe.DIRECTORY_ENTRY_IMPORT for e in d.imports}
    # Code with no FPO record (functions.csv kind "gap"): hand-written
    # assembly, or functions the FPO table leaves out. Nothing under src/
    # defines it, and some of it has several entry points.
    gaps = sorted((a, a + size) for a, (kind, size) in kinds.items() if kind == "gap")

    def in_gap(addr: int) -> bool:
        return any(lo <= addr < hi for lo, hi in gaps)

    def import_thunk(addr: int) -> bool:
        """`jmp [__imp_X]`: the linker's stub for an imported function."""
        raw = orig.read(addr, 6)
        return len(raw) == 6 and raw[:2] == b"\xff\x25" and struct.unpack_from("<I", raw, 2)[0] in import_slots

    library_base: dict[str, str] = {}
    for n, lib in libs.items():
        library_base.setdefault(base_name(n[6:] if n.startswith("__imp_") else n), lib)

    undefined = {n: r for n, r in refs.items() if n not in defs}
    rep.referenced = len(refs)
    cats: dict[str, dict[str, list]] = {c: defaultdict(list) for c in
                                         ("game functions", "game globals", "Win32 imports", "CRT/library", "other")}
    unresolved = 0
    for name in sorted(undefined):
        files = undefined[name]
        func = is_func.get(name, False)
        bname = base_name(name[6:] if name.startswith("__imp_") else name)
        line = f"{name}  ({len(files)} file{'s' if len(files) != 1 else ''}, e.g. {files[0]})"
        lib = libs.get(name)
        if lib in IMPORT_LIBS:
            cats["Win32 imports"][f"in {lib}.LIB"].append(line)
            continue
        if lib:
            cats["CRT/library"][f"in {lib}.LIB"].append(line)
            continue
        unresolved += 1
        if bname in library_base:
            # The library has the name, but not this spelling: a C++-mangled
            # declaration of a C function, or another calling convention.
            other = library_base[bname]
            cat = "Win32 imports" if other in IMPORT_LIBS else "CRT/library"
            cats[cat][f"spelt differently from {other}.LIB (extern \"C\", calling convention)"].append(line)
            continue
        addr = address_of(name, symbols)
        if name.startswith("__imp_") or bname in imports_by_name or (addr is not None and import_thunk(addr)):
            cats["Win32 imports"]["no import library in the toolchain (smackw32, DPLAYX by ordinal)"].append(line)
            continue
        if func:
            kind = kinds.get(addr, ("", 0))[0] if addr is not None else ""
            if kind == "game":
                # Prefer a definition under the same name, to tell a
                # signature difference from a different name.
                there = sorted(defined_at.get(addr, ()), key=lambda x: (base_name(x) != bname, x))
                if not there:
                    cats["game functions"]["no definition at that address"].append(line)
                elif base_name(there[0]) == bname:
                    sub = ("same name, signature differs only in struct names"
                           if signature_difference(name, there[0]) == "struct names"
                           else "same name, other signature")
                    cats["game functions"][sub].append(f"{line}\n            defined as {there[0]}")
                else:
                    cats["game functions"]["defined under another name"].append(
                        f"{line}\n            defined as {there[0]}")
            elif kind == "gap" or (addr is not None and in_gap(addr)):
                cats["game functions"]["in code with no FPO record (no source yet)"].append(f"{line}  [{addr:#x}]")
            elif kind == "library":
                cats["CRT/library"]["library code in the game region, not in the toolchain libraries"].append(line)
            elif addr is not None:
                cats["other"]["function name at an address that is no function start"].append(f"{line}  [{addr:#x}]")
            else:
                cats["other"][other_kind(name)].append(line)
            continue
        # Data.
        if addr is not None and addr >= 0x4FC000 and not name.startswith("??_7"):
            sub = ("named DAT_<address>" if PLACEHOLDER.search(bname.split("::")[-1])
                   else "real name (address from data/symbols.csv)")
            cats["game globals"][sub].append(f"{line}  [{addr:#x}]")
        else:
            cats["other"][other_kind(name)].append(line)
    rep.undefined = cats
    rep.unresolved = unresolved

    # Duplicate definitions.
    dups: dict[str, dict[str, list]] = {"COMDAT folds": defaultdict(list), "real conflicts": defaultdict(list)}
    for name, ds in sorted(defs.items()):
        if len(ds) < 2:
            continue
        files = [f for f, _ in ds]
        listed = f"({len(ds)} files: {', '.join(files[:4])}{', ...' if len(files) > 4 else ''})"
        # Communal symbols (`int x;` in C style) merge with each other and
        # with one real definition.
        real = [d for _, d in ds if d.fingerprint != "common"]
        strong = [d for d in real if d.comdat in (0, SEL_NODUPLICATES)]
        if len(real) <= 1:
            continue
        if not strong:
            prints = Counter(d.fingerprint for d in real)
            kind = fold_kind(name, real[0])
            same = "identical" if len(prints) == 1 else "contents differ, the linker keeps one"
            variants = f", {len(prints)} variants" if len(prints) > 1 else ""
            dups["COMDAT folds"][f"{kind}: {same}"].append(f"{name}  {listed[:-1]}{variants})")
            continue
        addr = address_of(name, symbols)
        d0 = ds[0][1]
        if d0.code and addr is not None and kinds.get(addr, ("", 0))[0] == "game":
            own = annotated.get(addr, ("", ""))[0]
            kind = "game function defined in more than one file"
            extra = f"  [{addr:#x}, annotated in {own or 'no file'}]"
        elif d0.code:
            kind, extra = "other code", ""
        elif addr is not None:
            kind, extra = "global data defined in more than one file", f"  [{addr:#x}]"
        else:
            kind, extra = "other data", ""
        dups["real conflicts"][kind].append(f"{name}  {listed}{extra}")
        rep.real_duplicates += 1
    rep.duplicates = dups

    # Definitions a library also offers: LNK2005 if that library member is pulled in.
    rep.libraries_overridden = sorted(f"{n}  ({libs[n]}.LIB; defined in {ds[0][0]})" for n, ds in defs.items()
                                      if n in libs and libs[n] not in IMPORT_LIBS
                                      and any(d.comdat in (0, SEL_NODUPLICATES) for _, d in ds))

    # Global types.
    table = global_table(objects, symbols)
    rep.global_count = len(table)
    gl: dict[str, list] = defaultdict(list)
    for addr, g in sorted(table.items()):
        verdict = g["verdict"]
        if verdict == "one type":
            gl[verdict].append(addr)
            continue
        views = "; ".join(f"{t} ({n})" for t, n in g["types"].most_common())
        gl[verdict].append(f"{addr:#x} {g['name']}: {views}")
    rep.globals = gl
    return rep, table


def global_table(objects: list[ObjectInfo], symbols: dict[str, int] | None = None) -> dict[int, dict]:
    """Every global the source refers to by address, with each file's view of it.

    address -> {name, spellings: Counter(symbol), types: Counter(declared type),
    typed: {declared type: type tuple}, files: set, addends: (lo, hi), verdict}"""
    symbols = load_symbols() if symbols is None else symbols
    table: dict[int, dict] = {}
    for o in objects:
        names = [n for n in list(o.refs) + list(o.defs)
                 if (n in o.refs and not o.refs[n]) or (n in o.defs and not o.defs[n].code)]
        arrays = None
        for sym in names:
            if sym.startswith(("__imp_", "??_7", "??_C@", "__real@", "$", "??_R", "__CT", "__TI")):
                continue
            addr = address_of(sym, symbols)
            if addr is None or not 0x4FC000 <= addr < 0x52D000:
                continue
            if arrays is None:
                arrays = source_arrays(ROOT / o.src)
            qname, t = data_symbol(sym)
            t = with_arrays(t, arrays.get(qname.split("::")[-1]))
            key = declare(t) if t is not None else ('extern "C" (no type)' if not sym.startswith("?") else "?")
            g = table.setdefault(addr, {"spellings": Counter(), "types": Counter(), "typed": {},
                                        "files": set(), "addends": None, "defined_in": []})
            g["spellings"][sym] += 1
            g["types"][key] += 1
            g["typed"].setdefault(key, t)
            g["files"].add(o.src)
            if sym in o.defs:
                g["defined_in"].append(o.src)
            if sym in o.addends:
                lo, hi = o.addends[sym]
                if g["addends"]:
                    lo, hi = min(lo, g["addends"][0]), max(hi, g["addends"][1])
                g["addends"] = (lo, hi)
    by_addr = {a: n for n, a in symbols.items()}
    for addr, g in table.items():
        held = by_addr.get(addr)
        g["name"] = held if held and not held.startswith("??_7") else f"DAT_{addr:08x}"
        g["verdict"] = verdict(g)
    return table


def verdict(g: dict) -> str:
    typed = {k: t for k, t in g["typed"].items() if t is not None}
    untyped = len(g["typed"]) - len(typed)
    keys = set(typed)
    # T[] and T[N] are one type: the extent may be left out of a declaration.
    arrays = {k for k in keys if typed[k][0] == "arr" and not all(typed[k][1])}
    for k in arrays:
        t = typed[k]
        if any(o != k and typed[o][0] == "arr" and declare(typed[o][2]) == declare(t[2]) for o in keys):
            keys.discard(k)
    if len(keys) <= 1:
        return "one type" if not (untyped and keys) else "one type, plus extern \"C\" references"
    if len({shape(typed[k]) for k in keys}) == 1:
        return "conflicting: struct names only"
    if len({shape(typed[k], loose=True) for k in keys}) == 1:
        return "conflicting: signedness or const"
    bare = {k: typed[k][2] if typed[k][0] == "cv" else typed[k] for k in keys}
    if ({t[0] for t in bare.values()} <= {"ptr", "arr"}
            and len({shape(t[2], loose=True) for t in bare.values()}) == 1):
        return "conflicting: array or pointer"
    return "conflicting: shape"


def other_kind(name: str) -> str:
    if name.startswith("??_7"):
        return "vtables"
    if name.startswith(("??_G", "??_E")):
        return "deleting destructors"
    if "std@@" in name:
        return "std:: members"
    if name.startswith("??"):
        return "constructors, destructors and operators"
    return "names with no known address"


def fold_kind(name: str, d: Definition) -> str:
    if name.startswith("??_C@"):
        return "string literals"
    if name.startswith("__real@"):
        return "floating-point constants"
    if name.startswith("??_7"):
        return "vtables"
    if name.startswith(("__CT", "__TI", "__CTA", "??_R")):
        return "exception tables"
    if d.code:
        return "inline and template functions"
    return "other data"


# --- output -------------------------------------------------------------------------

def count(d: dict[str, list]) -> int:
    return sum(len(v) for v in d.values())


WIDTH = 76  # label column


def row(indent: int, label: str, n: int, extra: str = "") -> str:
    return f"{' ' * indent}{label:{WIDTH - indent}s} {n:6,}{extra}"


def print_report(rep: Report, verbose: bool) -> None:
    total_undef = sum(count(v) for v in rep.undefined.values())
    print(f"linkcheck: {rep.objects:,} objects" + (f", {len(rep.failed)} failed to compile" if rep.failed else ""))
    print(f"\nUndefined symbols: {total_undef:,} of the {rep.referenced:,} external names the objects reference "
          "are defined by no object.")
    for cat, subs in rep.undefined.items():
        print(row(2, cat, count(subs)))
        if cat == "game globals":
            addrs = {re.search(r"\[(0x[0-9a-f]+)\]$", it).group(1) for items in subs.values() for it in items}
            print(f"    (names for {len(addrs):,} addresses: no global data is defined anywhere yet)")
        for sub, items in sorted(subs.items(), key=lambda kv: -len(kv[1])):
            print(row(4, sub, len(items)))
            if verbose:
                for it in items:
                    print(f"        {it}")
    resolved_by_libs = total_undef - rep.unresolved
    print(f"  {resolved_by_libs:,} of them come from the toolchain's libraries; {rep.unresolved:,} would be "
          "unresolved externals (LNK2001).")

    dup_total = count(rep.duplicates["COMDAT folds"]) + count(rep.duplicates["real conflicts"])
    print(f"\nDuplicate definitions: {dup_total:,} names are defined in more than one object.")
    for cat, subs in rep.duplicates.items():
        note = " (no link error: the linker keeps one copy)" if cat == "COMDAT folds" else " (LNK2005)"
        print(row(2, cat + note, count(subs)))
        for sub, items in sorted(subs.items(), key=lambda kv: -len(kv[1])):
            print(row(4, sub, len(items)))
            if verbose:
                for it in items:
                    print(f"        {it}")
    if rep.libraries_overridden:
        print(row(2, "defined by an object and by a CRT library (LNK2005 if that member links)",
                  len(rep.libraries_overridden)))
        if verbose:
            for it in rep.libraries_overridden:
                print(f"        {it}")

    conflicting = sum(len(v) for k, v in rep.globals.items() if k.startswith("conflicting"))
    print(f"\nGlobal types: {rep.global_count:,} globals are referenced by address. For each, the types the "
          "source declares\nfor it (from the data symbols' mangled names, and arrays from the declarations):")
    for verdict_, items in sorted(rep.globals.items(), key=lambda kv: (kv[0].startswith("conflicting"), -len(kv[1]))):
        print(row(2, verdict_, len(items)))
        if verbose and verdict_.startswith(("conflicting", "one type,")):
            for it in items:
                print(f"        {it}" if isinstance(it, str) else f"        {it:#x}")
    print()
    print(summary_line(rep, total_undef, conflicting))


def summary_line(rep: Report, total_undef: int, conflicting: int) -> str:
    resolved = rep.referenced - rep.unresolved
    pct = 100 * resolved / rep.referenced if rep.referenced else 100.0
    errors = rep.unresolved + rep.real_duplicates
    return (f"linkability: {pct:.1f}% of {rep.referenced:,} referenced names resolve; "
            f"{errors:,} link errors ({rep.unresolved:,} unresolved, {rep.real_duplicates:,} duplicate); "
            f"{conflicting:,} of {rep.global_count:,} globals have conflicting types")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--verbose", "-v", action="store_true", help="list every symbol behind each count")
    ap.add_argument("--json", type=Path, help="also write the counts to this file")
    args = ap.parse_args()

    objects, failed = load_objects()
    rep, _ = analyse(objects)
    rep.failed = failed
    print_report(rep, args.verbose)
    if failed:
        print("\nfailed to compile:\n  " + "\n  ".join(failed))
    if args.json:
        total_undef = sum(count(v) for v in rep.undefined.values())
        conflicting = sum(len(v) for k, v in rep.globals.items() if k.startswith("conflicting"))
        out = {
            "objects": rep.objects,
            "referenced": rep.referenced,
            "undefined": {c: {s: len(v) for s, v in subs.items()} for c, subs in rep.undefined.items()},
            "unresolved": rep.unresolved,
            "duplicates": {c: {s: len(v) for s, v in subs.items()} for c, subs in rep.duplicates.items()},
            "real_duplicates": rep.real_duplicates,
            "globals": {k: len(v) for k, v in rep.globals.items()},
            "summary": summary_line(rep, total_undef, conflicting),
        }
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(out, indent=2) + "\n")


if __name__ == "__main__":
    main()
