"""Write include/ta_protos.h: prototypes of the game's own free functions.

    uv run tools/protos.py              # writes include/ta_protos.h
    uv run tools/protos.py --stats      # also lists what was left out and why
    uv run tools/protos.py --verify     # also compiles every prototype as a definition
                                        # and checks it decorates to the name it came from

Cavedog's sources declared the game's functions in headers that every file
included; those headers are lost. This rebuilds the part we know. A function's
full signature is its decorated name: data/symbols.csv holds the exe's names
(a few of them decorated), and data/progress.csv the decorated name each
function's file under src/ compiles to (matched or partial), including a few
functions with no callers that symbols.csv lacks.
Each free function (`?name@@Y...`) is demangled into a prototype, in address
order, and every struct, class and union a prototype names is declared ahead
of them (`struct Unit;`), with the class key its decoration uses.

Left out, because a forward declaration cannot express them: member functions
(they need the class body, which each file defines itself), and prototypes
that name a template (std::vector and the like), an enum or a nested type.

docs/c2-regalloc.md ("Symbol ids", "A prototypes header") says why a header
like this matters and what it measured: the front end numbers every declaration
in the file, and some register and operand ties follow those numbers. No file
includes it yet. A file that does must not declare a callee differently: a
prototype that differs only in its return type or calling convention is an
error, and one with other parameter types is an overload that can make a call
ambiguous (a literal 0, a function name assigned to a void*).
"""

import argparse
import csv
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SYMBOLS = ROOT / "data" / "symbols.csv"
PROGRESS = ROOT / "data" / "progress.csv"
OUT = ROOT / "include" / "ta_protos.h"

PRIMITIVES = {
    "C": "signed char", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short",
    "H": "int", "I": "unsigned int", "J": "long", "K": "unsigned long", "M": "float",
    "N": "double", "O": "long double", "X": "void",
}
EXTENDED = {"_N": "bool", "_J": "__int64", "_K": "unsigned __int64"}
KEYS = {"U": "struct", "V": "class", "T": "union"}
CONVENTIONS = {"A": "__cdecl", "G": "__stdcall", "I": "__fastcall"}
CV = {"A": "", "B": "const", "C": "volatile", "D": "const volatile"}


class Unsupported(Exception):
    """A decoration this prototype header cannot declare (a reason, not a bug)."""


class Demangler:
    """The part of Microsoft's decoration scheme that free functions here use."""

    def __init__(self, text: str):
        self.s = text
        self.i = 0
        self.names: list[str] = []   # name back-references 0-9
        self.args: list[tuple] = []  # argument back-references 0-9
        self.types: dict[str, str] = {}

    def peek(self, n: int = 1) -> str:
        return self.s[self.i:self.i + n]

    def take(self, n: int = 1) -> str:
        part = self.s[self.i:self.i + n]
        if len(part) < n:
            raise ValueError("truncated")
        self.i += n
        return part

    def fragment(self) -> str:
        c = self.peek()
        if c.isdigit():
            self.take()
            return self.names[int(c)]
        if c == "?":
            raise Unsupported("names a template")
        end = self.s.index("@", self.i)
        name = self.s[self.i:end]
        self.i = end + 1
        if len(self.names) < 10:
            self.names.append(name)
        return name

    def qualified(self) -> list[str]:
        parts = []
        while self.peek() != "@":
            parts.append(self.fragment())
        self.take()
        return parts[::-1]

    def number(self) -> int:
        c = self.take()
        if c.isdigit():
            return int(c) + 1
        value = 0
        while c != "@":
            value = value * 16 + ord(c) - ord("A")
            c = self.take()
        return value

    def type(self, slot: bool = False) -> tuple:
        start = self.i
        c = self.peek()
        if c.isdigit() and slot:
            self.take()
            return self.args[int(c)]
        t = self.type_body()
        if slot and self.i - start > 1 and len(self.args) < 10:
            self.args.append(t)
        return t

    def type_body(self) -> tuple:
        c = self.take()
        if c in PRIMITIVES:
            return ("prim", PRIMITIVES[c])
        if c == "_":
            code = c + self.take()
            if code not in EXTENDED:
                raise Unsupported(f"type {code}")
            return ("prim", EXTENDED[code])
        if c in "PQRS":
            const_ptr = c in "QS"
            if self.peek() == "6":
                self.take()
                return ("ptr", self.function(), const_ptr)
            if self.peek() not in CV:
                raise Unsupported("pointer to member")
            cv = CV[self.take()]
            pointee = self.type_body()
            if cv and pointee[0] != "ptr":  # a pointer pointee carries its own const (Q)
                pointee = ("cv", cv, pointee)
            return ("ptr", pointee, const_ptr)
        if c == "A":
            cv = CV[self.take()]
            target = self.type_body()
            if cv and target[0] != "ptr":
                target = ("cv", cv, target)
            return ("ref", target)
        if c in KEYS:
            parts = self.qualified()
            if len(parts) != 1:
                raise Unsupported("nested type")
            self.types.setdefault(parts[0], KEYS[c])
            return ("named", parts[0])
        if c == "W":
            raise Unsupported("enum")
        if c == "Y":
            dims = [self.number() for _ in range(self.number())]
            return ("array", dims, self.type_body())
        if c == "?":
            raise Unsupported("names a template")
        raise Unsupported(f"type {c}")

    def returns(self) -> tuple:
        if self.peek() == "?":
            self.take()
            cv = CV[self.take()]
            t = self.type_body()
            return ("cv", cv, t) if cv else t
        return self.type_body()

    def function(self) -> tuple:
        cc = CONVENTIONS.get(self.take())
        if cc is None:
            raise Unsupported("calling convention")
        ret = self.returns()
        params: list[tuple] = []
        varargs = False
        if self.peek() == "X":
            self.take()
        else:
            while self.peek() not in ("@", "Z"):
                params.append(self.type(slot=True))
            varargs = self.take() == "Z"
        if self.take() != "Z":
            raise ValueError("no Z after the parameters")
        return ("func", cc, ret, params, varargs)

    def symbol(self) -> tuple[str, tuple]:
        if self.take() != "?":
            raise Unsupported("C name")
        if self.peek() == "?":
            raise Unsupported("constructor, destructor or operator")
        if self.peek() == "$":
            raise Unsupported("function template")
        parts = self.qualified()
        if len(parts) != 1:
            raise Unsupported("member function")
        if self.take() != "Y":
            raise Unsupported("not a free function")
        f = self.function()
        if self.i != len(self.s):
            raise ValueError("trailing characters")
        return parts[0], f


def render(t: tuple, inner: str = "") -> str:
    """C declarator for type t around the declarator text `inner`."""
    kind = t[0]
    if kind in ("prim", "named"):
        return f"{t[1]} {inner}".rstrip()
    if kind == "cv":
        return f"{t[1]} {render(t[2], inner)}"
    if kind == "array":
        return render(t[2], inner + "".join(f"[{d}]" for d in t[1]))
    if kind == "func":
        _, cc, ret, params, varargs = t
        args = [render(p) for p in params] + (["..."] if varargs else [])
        return render(ret, f"{inner}({', '.join(args) or 'void'})")
    mark = "*" if kind == "ptr" else "&"
    if kind == "ptr" and t[2]:
        mark += " const"
    target = t[1]
    if target[0] == "func":
        return render(target, f"({target[1]} {mark}{inner})")
    if target[0] == "array":
        return render(target, f"({mark}{inner})")
    return f"{render(target)}{mark}" + (f" {inner}" if inner else "")


def prototype(name: str, f: tuple) -> str:
    """The declaration of free function `name` of type f (/Gz makes __stdcall the default)."""
    cc = f[1]
    return render(f, name if cc == "__stdcall" else f"{cc} {name}") + ";"


def collect() -> tuple[dict[str, str], list[tuple[int, str, str, str, tuple]], dict[str, list[str]]]:
    """(types by name with their key, (address, decorated name, function name, prototype,
    parsed type) in address order, what was left out by reason)."""
    # A decorated name in data/symbols.csv, else the one the function's file compiles
    # to. progress.csv also has a few functions symbols.csv lacks (no callers).
    decorated = {int(r["address"], 16): r["symbol"] for r in csv.DictReader(PROGRESS.open())}
    for row in csv.DictReader(SYMBOLS.open()):
        if row["name"].startswith("?"):
            decorated[int(row["address"], 16)] = row["name"]
    types: dict[str, str] = {}
    protos: list[tuple[int, str, str, str, tuple]] = []
    seen: dict[str, tuple] = {}
    skipped: dict[str, list[str]] = {}
    for address, symbol in sorted(decorated.items()):
        if not symbol.startswith("?"):
            continue
        d = Demangler(symbol)
        try:
            fname, f = d.symbol()
        except Unsupported as why:
            skipped.setdefault(str(why), []).append(f"{address:#x} {symbol}")
            continue
        params = tuple(f[3]), f[4]
        if fname in seen and seen[fname][0] == params:
            if seen[fname][1] != f[2]:
                skipped.setdefault("same parameters as an earlier one, other return type", []).append(
                    f"{address:#x} {symbol}")
            continue
        seen.setdefault(fname, (params, f[2]))
        for tname, key in d.types.items():
            types.setdefault(tname, key)
        protos.append((address, symbol, fname, prototype(fname, f), f))
    return types, protos, skipped


def verify(types: dict[str, str], protos: list[tuple[int, str, str, str, tuple]]) -> int:
    """Compile every prototype as a definition and check it decorates back to its name."""
    sys.path.insert(0, str(ROOT / "tools"))
    from check import compile_source
    from coff import parse_object

    work = ROOT / "build" / "protos"
    work.mkdir(parents=True, exist_ok=True)
    src = work / "verify.cpp"
    body = [f"{key} {name} {{}};" for name, key in sorted(types.items())]
    for p in protos:
        ret = p[4][2]
        if ret == ("prim", "void"):
            value = "{}"
        elif ret[0] in ("prim", "ptr"):
            value = "{ return 0; }"
        else:
            value = f"{{ return *({render(ret[1] if ret[0] == 'ref' else ret)}*)0; }}"
        body.append(f"{p[3][:-1]} {value}")
    src.write_text("\n".join(body) + "\n")
    obj, log = compile_source(src, out_dir="protos")
    if obj is None:
        print(log)
        return 1
    # A forward declaration's key (struct or class) decides the decoration, and a
    # few types are a struct in one file and a class in another: compare without it.
    def keyless(name: str) -> str:
        return re.sub(r"[UVT](?=[A-Za-z_]\w*@)", "U", name)

    names = {keyless(s.name) for s in parse_object(obj.read_bytes()).symbols}
    wrong = [p for p in protos if keyless(p[1]) not in names]
    for address, symbol, _, text, _ in wrong:
        print(f"  {address:#x} {symbol}: {text}")
    print(f"verify: {len(protos) - len(wrong)} of {len(protos)} prototypes decorate to their names")
    return 1 if wrong else 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", type=Path, default=OUT)
    ap.add_argument("--stats", action="store_true", help="list the functions left out and why")
    ap.add_argument("--verify", action="store_true",
                    help="compile each prototype as a definition and compare its decorated name")
    args = ap.parse_args()

    types, protos, skipped = collect()
    lines = [
        "// Generated by tools/protos.py from data/symbols.csv and data/progress.csv; do not edit.",
        "// The game's own free functions, as their matched sources declare them, and the",
        "// structs and classes those prototypes name. Member functions, and prototypes that",
        "// name a template, an enum or a nested type, are left out (see the tool).",
        "#ifndef TA_PROTOS_H",
        "#define TA_PROTOS_H",
        "",
    ]
    lines += [f"{key} {name};" for name, key in sorted(types.items())]
    lines += [""] + [p[3] for p in protos] + ["", "#endif", ""]
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text("\n".join(lines))
    print(f"{args.out.relative_to(ROOT) if args.out.is_relative_to(ROOT) else args.out}: "
          f"{len(protos)} prototypes, {len(types)} types")
    for why, items in sorted(skipped.items()):
        print(f"  left out, {why}: {len(items)}")
        if args.stats:
            for item in items:
                print(f"    {item}")
    return verify(types, protos) if args.verify else 0


if __name__ == "__main__":
    sys.exit(main())
