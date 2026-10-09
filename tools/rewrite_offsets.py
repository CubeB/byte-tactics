"""Rewrite `*(T*)((char*)x + 0xN)` onto a named member of x's view.

    uv run tools/rewrite_offsets.py src/map/line_of_sight.cpp
    uv run tools/rewrite_offsets.py --dry-run src/game/game_load.cpp

Phase 4 of docs/cleanup-roadmap.md: "A script then rewrites the offset casts
that land on a named member." For each byte-offset access the tool finds x's
declared type in scope (the declaration nearest the use that is in the same
function or class, resolved as tools/rename_fields.py resolves it) and the
type's view in the file (the compiler's own layout of it, from the /Z7 records
tools/gametypes.py reads). When the view declares a member at the offset whose
type is the cast's, the access becomes `x->member` (`x.member`, `x->arr[i]`,
`&x->member`). A member that is an array of T takes the index the expression
carries: `(char*)x + 0x14b * i + 0x1f`, with the view's array of 0x14b-byte
elements, becomes `x->arr[i].member`.

A file built with /Gi (a `// FLAGS: /Gi` line) is read the same way: its views
come from a /Z7 copy with the /Gi line stripped, compiled with the original
file's directory on the include path so its relative includes still resolve.
Only that copy is read, for the member offsets; the rewrite is always applied to
the original file.

What is left alone is counted, because the offset does not land on a member
(padding, or a view that does not split the range), x is a `char*`/`void*` or
a scalar with no view, or the access is not one offset from the operand
(a nested cast, a second variable, a subtraction).

Like tools/trycasts.py (#6225), one shape of rewrite (`x`, member path, cast
type) is a group: the group is applied, the file's functions are run through
tools/checkall.py, and the edit is kept only when every one still prints
MATCH; otherwise it is put back. The file is restored to its last
known-matching state when the run is interrupted (SIGINT, SIGTERM or an
error), so a killed run leaves a file that still compiles and matches.
"""

import argparse
import re
import signal
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import gametypes as gt  # noqa: E402
from rename_fields import Source  # noqa: E402
from sources import annotations, relative  # noqa: E402
from trycasts import (Interrupted, checkall, code_mask, restore,  # noqa: E402
                      tidy, write_atomic)

IDENT = r"[A-Za-z_]\w*"
CHAR_CAST = re.compile(r"\(\s*char\s*\*+\s*\)")
CONST = re.compile(r"^(?:0[xX][0-9a-fA-F]+|\d+)$")
# The cast a `(char*)x` sits in, with the `(` before it: `*(T*)(` or `(T*)(`.
DEREF_CAST = re.compile(r"\*\s*\(\s*([A-Za-z_][^()]*?)\s*\)\s*\(\s*$")
PLAIN_CAST = re.compile(r"\(\s*([A-Za-z_][^()]*?)\s*\)\s*\(\s*$")
# What may appear in the terms of an offset expression.
TERM = re.compile(r"^[A-Za-z0-9_\s()\[\].>:*+-]+$")

# The primitives a cast can spell, with the synonyms source writes for them.
SYNONYM = {
    "unsigned": "unsigned int", "signed": "int", "short int": "short",
    "signed short": "short", "signed short int": "short",
    "unsigned short int": "unsigned short", "long int": "long",
    "signed long": "long", "signed long int": "long",
    "unsigned long int": "unsigned long", "long long": "__int64",
    "signed long long": "__int64", "unsigned long long": "unsigned __int64",
}
PRIMS = set(gt.PRIM_SIZE)
# Words the declaration pattern can pick up that are not type names. The
# pattern already swallows the type-ish keywords (`struct`, `unsigned`, ...).
NOT_TYPES = {
    "return", "case", "sizeof", "delete", "new", "throw", "goto", "else", "do", "while",
    "if", "switch", "for", "default", "using", "typedef", "template", "friend", "operator",
    "catch", "break", "continue", "static_cast", "dynamic_cast", "const_cast",
    "reinterpret_cast", "typename", "namespace", "public", "private", "protected", "virtual",
    "export", "explicit", "mutable", "inline", "asm", "__asm", "__try", "__finally", "__except",
}


# --- types -------------------------------------------------------------------------

def parse_cast(text: str) -> tuple | None:
    """The type a cast spells in gametypes' tuple form, or None. Star counts
    are returned separately, since the tuple holds the base type."""
    text = tidy(text)
    stars = 0
    while text.endswith("*"):
        stars += 1
        text = tidy(text[:-1])
    text = tidy(re.sub(r"\b(?:const|volatile)\b", " ", text))
    text = tidy(re.sub(r"^(?:struct|class|union|enum)\s+", "", text))
    text = SYNONYM.get(text, text)
    if text in PRIMS:
        return ("prim", text), stars
    if re.fullmatch(IDENT + r"(?:\s*::\s*" + IDENT + r")*", text):
        return ("named", "struct", gt.parse_cxx_name(text)), stars
    return None


def type_name(t: tuple | None) -> str | None:
    """The plain name of a named or enum type, or None."""
    if not t:
        return None
    if t[0] == "named":
        parts = t[2]
    elif t[0] == "enum":
        parts = t[1]
    else:
        return None
    return parts[0] if len(parts) == 1 and isinstance(parts[0], str) else None


def describe(t: tuple | None) -> str:
    """A type in source's own shape, for the summary."""
    if not t:
        return "unknown"
    k = t[0]
    if k == "prim":
        return t[1]
    if k == "ptr":
        return describe(t[1]) + "*"
    if k == "array":
        return describe(t[2])
    if k in ("named", "enum"):
        return type_name(t) or "named"
    return "a struct"


def same_type(a: tuple | None, b: tuple | None, wildcard: bool = False) -> bool:
    """Whether two types name the same type. cv is dropped on both sides and
    a struct and a class with one name are the same; `wildcard` accepts any."""
    if wildcard:
        return True
    if a is None or b is None:
        return False
    if a[0] != b[0]:
        if a[0] in ("named", "enum") and b[0] in ("named", "enum"):
            return type_name(a) == type_name(b)
        return False
    k = a[0]
    if k == "prim":
        return a[1] == b[1]
    if k in ("ptr", "ref"):
        return same_type(a[1], b[1])
    if k == "array":
        return a[1] == b[1] and same_type(a[2], b[2])
    if k in ("named", "enum"):
        return type_name(a) == type_name(b)
    if k == "anon":
        return a[1] is b[1]
    return a == b


class Views:
    """A file's views, converted to the tuple type form, with sizes."""

    def __init__(self, raw: dict[str, dict]):
        self.raw = raw
        self.conv: dict[str, dict] = {}

    def get(self, name: str) -> dict | None:
        view = self.raw.get(name)
        if view is None:
            return None
        if name not in self.conv:
            self.conv[name] = gt.convert_view(view)
        return self.conv[name]

    def find(self, name: str | None) -> dict | None:
        """The view named `name`, also under its address-suffixed spelling."""
        return self.get(name) or self.get(gt.base_name(name)) if name else None

    def size_of(self, t: tuple | None) -> int | None:
        if not t:
            return None
        k = t[0]
        if k == "prim":
            return gt.PRIM_SIZE.get(t[1])
        if k in ("ptr", "ref"):
            return 4
        if k == "array":
            size = self.size_of(t[2])
            return None if size is None else size * t[1]
        if k in ("named", "enum"):
            view = self.find(type_name(t))
            return view["size"] if view else None
        if k == "anon":
            return t[1]["size"]
        return None

    def members(self, view: dict) -> list[tuple[int, str, tuple, dict]]:
        """(offset, name, type, field) of the view's members, with anonymous
        unions and structs flattened in (their members are the view's own)."""
        out = []
        for f in view["fields"]:
            if f["type"][0] == "anon":
                for off, name, t, inner in self.members(f["type"][1]):
                    out.append((f["off"] + off, name, t, inner))
            else:
                out.append((f["off"], f["name"], f["type"], f))
        return out

    def member_named(self, t: tuple | None, name: str) -> tuple | None:
        """The type of the member `name` of a value of type `t`."""
        if not t or t[0] != "named":
            return None
        view = self.find(type_name(t))
        if view is None:
            return None
        for _, mname, mt, f in self.members(view):
            if mname == name and not f.get("bits"):
                return mt
        return None

    def path(self, view: dict, c: int, k: int, e: str | None,
             want: tuple | None) -> tuple[list[tuple[str, str]], tuple] | None:
        """The member path whose address is +c+k*e in `view`, when the
        expression's type is `want`. Steps are ('m', name) and ('x', index);
        a member at exactly the offset is preferred over an array element."""
        members = self.members(view)
        # Padding members (`unknown_<offset>`) are blocks of unnamed bytes, not
        # members the roadmap is naming: leave accesses into them as they are.
        members = [m for m in members if not m[1].startswith("unknown")]
        if k == 0:
            for off, name, t, f in members:
                if f.get("bits") or off != c:
                    continue
                if t[0] == "array" and same_type(t[2], want, want is None):
                    return [("m", name), ("x", "0")], t[2]
                if same_type(t, want, want is None):
                    return [("m", name)], t
        for off, name, t, f in members:
            if f.get("bits") or t[0] != "array":
                continue
            elem, n = t[2], t[1]
            esz = self.size_of(elem)
            if esz is None or n <= 0 or not (off <= c < off + esz * n):
                continue
            if k:
                if esz != k:
                    continue
                index: str = e or "0"
            else:
                j, c = divmod(c - off, esz)
                index = str(j)
            head: list[tuple[str, str]] = [("m", name), ("x", index)]
            if c == 0:
                if same_type(elem, want, want is None):
                    return head, elem
                continue
            inner_view = self.find(type_name(elem))
            if inner_view is None:
                continue
            inner = self.path(inner_view, c, 0, None, want)
            if inner:
                return head + inner[0], inner[1]
        return None


# --- declarations ------------------------------------------------------------------

def decl_of(src: Source, name: str, pos: int) -> tuple[str, int] | None:
    """The declared type of `name` nearest before pos that is in scope, as
    (type, stars). A member of the enclosing class, a local or parameter of
    the same function, or a global counts; a sibling's member does not."""
    pattern = re.compile(
        r"(?:(?<=[;{}(),:])|^)\s*"
        r"(?:(?:const|volatile|unsigned|signed|long|short|static|extern|register|auto"
        r"|struct|class|union|enum)\s+)*"
        r"([A-Za-z_]\w*(?:\s*<[^;{}]*>)?(?:\s*::\s*[A-Za-z_]\w*)*)\s*([*&]*)\s*"
        + re.escape(name) + r"\b")
    f = src.func_at(pos)
    best = None
    for m in pattern.finditer(src.code, 0, pos):
        t = m.group(1)
        if t in NOT_TYPES:
            continue
        if m.group(2).count("&") > 1:
            continue  # `flag && name`: a logical and, not a declarator
        d = m.start(1)
        s = src.struct_at(d)
        if s is not None:
            if not (s[1] < pos < s[2]) and not (f is not None and src.class_of(f) == s[0]):
                continue  # a member of a sibling struct
        else:
            df = src.func_at(d)
            if df is not None and (f is None or df[2] != f[2]):
                continue  # a local or parameter of another function
        if best is None or d > best[0]:
            best = (d, t, m.group(2))
    if best is None:
        return None
    return best[1], best[2].count("*")


def chain_steps(text: str, start: int) -> tuple[str, list[tuple[str, str]], int] | None:
    """`x` and its `->member` / `.member` / `[index]` steps from text[start:],
    or None. The third value is where the operand ends."""
    n = len(text)
    i = start
    if i < n and text[i] == "&":
        i += 1
    while i < n and text[i] in " \t":
        i += 1
    m = re.match(IDENT, text[i:])
    if not m:
        return None
    base = m.group(0)
    i += m.end()
    steps: list[tuple[str, str]] = []
    while True:
        j = i
        while j < n and text[j] in " \t":
            j += 1
        if text.startswith("->", j) or (j < n and text[j] == "."):
            op = "->" if text.startswith("->", j) else "."
            k = j + len(op)
            while k < n and text[k] in " \t":
                k += 1
            m = re.match(IDENT, text[k:])
            if not m:
                return None
            steps.append((op, m.group(0)))
            i = k + m.end()
        elif j < n and text[j] == "[":
            depth, k = 0, j
            while k < n:
                if text[k] == "[":
                    depth += 1
                elif text[k] == "]":
                    depth -= 1
                    if depth == 0:
                        break
                k += 1
            if k >= n:
                return None
            steps.append(("[", tidy(text[j + 1:k])))
            i = k + 1
        else:
            break
    return base, steps, i


def wrap(t: tuple, stars: int) -> tuple:
    for _ in range(stars):
        t = ("ptr", t)
    return t


def deref(t: tuple) -> tuple | None:
    if t is None:
        return None
    if t[0] == "ptr":
        return t[1]
    if t[0] == "array":
        return t[2]
    return None


def resolve(src: Source, views: Views, base: str, steps: list[tuple[str, str]],
            pos: int) -> tuple | None:
    """The type of the operand `base` with its member chain applied, or None."""
    if base == "this":
        f = src.func_at(pos)
        cls = src.class_of(f) if f else None
        if cls is None:
            return None
        t = ("ptr", ("named", "struct", gt.parse_cxx_name(cls)))
    else:
        decl = decl_of(src, base, pos)
        if decl is None:
            return None
        t = wrap(("named", "struct", gt.parse_cxx_name(decl[0])), decl[1])
    for op, name in steps:
        if op == "[":
            t = deref(t)
        elif op == "->":
            t = views.member_named(deref(t), name)
        else:
            t = views.member_named(t, name)
        if t is None:
            return None
    return t


# --- the accesses ------------------------------------------------------------------

class Access:
    """One byte-offset access the tool can rewrite."""

    def __init__(self, start: int, end: int, kind: str, operand: str,
                 steps: list[tuple[str, str]], source: str):
        self.start = start
        self.end = end
        self.kind = kind
        self.operand = operand
        self.steps = steps
        self.source = source

    def render(self) -> str:
        text = self.operand[1:].lstrip() if self.operand.startswith("&") else self.operand
        sep = "." if self.operand.startswith("&") else "->"
        for kind, value in self.steps:
            text += ("[" + value + "]") if kind == "x" else (sep + value)
            sep = "."
        if self.kind == "bare":
            return "(char*)&" + text
        if self.kind == "castptr":
            return "&" + text
        return text

    def key(self) -> tuple:
        return (self.kind, self.operand.startswith("&"), self.operand, tuple(self.steps))


def split_top(text: str, sep: str) -> list[str]:
    """`text` cut at the single character `sep` outside parentheses and brackets."""
    out, depth, cur = [], 0, ""
    for c in text:
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        if depth == 0 and c == sep:
            out.append(cur)
            cur = ""
        else:
            cur += c
    out.append(cur)
    return out


def offset_terms(rest: str) -> tuple[int, int, str | None] | None:
    """`rest` as c + k*e, or None when it holds more than one variable. A
    term is a constant, `c*e`, `e*c` or a plain expression."""
    pieces: list[tuple[int, str]] = []
    depth = 0
    cur = ""
    sign = 1
    i = 0
    while i < len(rest):
        c = rest[i]
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        if depth == 0 and c in "+-" and not rest.startswith("->", i):
            if cur.strip():
                pieces.append((sign, cur))
            sign = 1 if c == "+" else -1
            cur = ""
        else:
            cur += c
        i += 1
    if cur.strip():
        pieces.append((sign, cur))
    c_total = 0
    var: tuple[int, str] | None = None
    for sign, term in pieces:
        term = tidy(term)
        if not term or not TERM.match(term):
            return None
        if CONST.match(term):
            c_total += sign * int(term, 0)
            continue
        parts = split_top(term, "*")
        if len(parts) == 2 and CONST.match(tidy(parts[0])) and not CONST.match(tidy(parts[1])):
            k, e = int(tidy(parts[0]), 0), tidy(parts[1])
        elif len(parts) == 2 and CONST.match(tidy(parts[1])) and not CONST.match(tidy(parts[0])):
            k, e = int(tidy(parts[1]), 0), tidy(parts[0])
        elif len(parts) == 1:
            k, e = 1, term
        else:
            return None
        k *= sign
        if var is None:
            var = (k, e)
        elif var[1] == e:
            var = (var[0] + k, e)
        else:
            return None
    return c_total, (var[0] if var else 0), (var[1] if var else None)


def rest_after(text: str, i: int) -> tuple[str, int]:
    """The offset expression after an operand: its terms, up to the
    enclosing parenthesis, `;` or `,`, with the index just past it."""
    rest, depth, j = "", 0, i
    while j < len(text):
        c = text[j]
        if c in "([":
            depth += 1
        elif c in ")]":
            if depth == 0:
                break
            depth -= 1
        if depth == 0 and c in ";,?=<>!|^%/" and not (c == ">" and j > 0 and text[j - 1] == "-"):
            break
        rest += c
        j += 1
    return rest, j


def scan(src: Source, views: Views, text: str) -> list[Access]:
    """Every access in the file that can be rewritten."""
    mask = code_mask(text)
    out: list[Access] = []
    for m in CHAR_CAST.finditer(text):
        if not mask[m.start()]:
            continue
        pre = text[:m.start()]
        dm = DEREF_CAST.search(pre)
        pm = PLAIN_CAST.search(pre)
        kind = "bare"
        want = None
        if dm or pm:
            parsed = parse_cast((dm or pm).group(1))
            if parsed is None or parsed[1] < 1:
                continue
            base_t, stars = parsed
            kind = "deref" if dm else "castptr"
            want = base_t if stars == 1 else wrap(base_t, stars - 1)
        chain = chain_steps(text, m.end())
        if chain is None:
            continue
        base, steps, i = chain
        operand = tidy(text[m.end():i])
        rest, j = rest_after(text, i)
        if not tidy(rest).startswith(("+", "-")):
            continue
        terms = offset_terms(rest)
        if terms is None:
            continue
        c, k, e = terms
        resolved = resolve(src, views, base, steps, m.start())
        if resolved is None:
            continue
        obj = resolved if operand.startswith("&") else deref(resolved)
        if obj is None:
            continue
        if obj[0] == "array":
            obj = obj[2]
        name = type_name(obj)
        view = views.find(name)
        if view is None:
            continue
        # The cast is replaced with it: `*(T*)(...)`, `(T*)(...)` or the bare
        # `(char*)x + off` cast.
        if dm:
            start = dm.start()
        elif pm:
            start = pm.start()
        else:
            start = m.start()
        end = j + 1 if (dm or pm) else j
        if kind == "castptr" and re.match(r"^\s*\)\s*->", text[j:]):
            kind = "arrow"
            want = wrap(want, 1)
        path = views.path(view, c, k, e, want)
        if path is None:
            continue
        out.append(Access(start, end, kind, operand, path[0], text[start:end]))
    return out


def splice(text: str, access: Access) -> str:
    return text[:access.start] + access.render() + text[access.end:]


def reasons_of(text: str, src: Source, views: Views) -> dict[str, int]:
    """Why the remaining `(char*)` accesses stay, for the summary."""
    out: dict[str, int] = {}
    spans = [(a.start, a.end) for a in scan(src, views, text)]

    def note(reason: str) -> None:
        out[reason] = out.get(reason, 0) + 1

    def rewritten(pos: int) -> bool:
        return any(a <= pos < b for a, b in spans)

    mask = code_mask(text)
    for m in CHAR_CAST.finditer(text):
        if not mask[m.start()] or rewritten(m.start()):
            continue
        pre = text[:m.start()]
        dm = DEREF_CAST.search(pre)
        pm = PLAIN_CAST.search(pre)
        if dm or pm:
            parsed = parse_cast((dm or pm).group(1))
            if parsed is None or parsed[1] < 1:
                note("the cast type is not a pointer type")
                continue
        chain = chain_steps(text, m.end())
        if chain is None:
            note("the operand is not a plain variable chain")
            continue
        base, steps, i = chain
        rest, _ = rest_after(text, i)
        if not tidy(rest).startswith(("+", "-")):
            note("no offset from the operand")
            continue
        if offset_terms(rest) is None:
            note("the offset is not one constant plus one variable")
            continue
        resolved = resolve(src, views, base, steps, m.start())
        if resolved is None:
            note("the operand's type is not known in this file")
            continue
        operand = tidy(text[m.end():i])
        obj = resolved if operand.startswith("&") else deref(resolved)
        if obj is None:
            note("the operand is not a pointer to a view")
            continue
        if obj[0] == "array":
            obj = obj[2]
        name = type_name(obj)
        view = views.find(name)
        if view is None:
            note(f"the operand points to {describe(obj)}, which has no view here")
            continue
        note("no member at the offset")
    return out


def rewrite_file(path: Path, dry_run: bool) -> tuple[int, int, dict[str, int]]:
    src = Source(path)
    text = path.read_text(encoding="latin-1")
    addresses = [a for a, _ in annotations(path)]
    if not addresses:
        print(f"{relative(path)}: no functions to check")
        return 0, 0, {}
    data = gt.extract(path, gt.file_id(path), wanted_names(src))
    if data is None:
        print(f"{relative(path)}: the file does not compile with /Z7; left alone")
        return 0, 0, {}
    views = Views(data["views"])
    first = scan(src, views, text)
    if dry_run:
        for a in first:
            print(f"{relative(path)}  {a.source}  ->  {a.render()}")
        return 0, 0, reasons_of(text, src, views)
    good = text
    rewritten = groups = 0
    failed: set[tuple] = set()
    while True:
        accesses = scan(src, views, good)
        keys = []
        for a in accesses:
            if a.key() not in keys and a.key() not in failed:
                keys.append(a.key())
        if not keys:
            break
        key = keys[0]
        group = sorted((a for a in accesses if a.key() == key),
                       key=lambda a: a.start, reverse=True)
        candidate = good
        for a in group:
            candidate = splice(candidate, a)
        write_atomic(path, candidate)
        matched_all, matched, output = checkall(addresses)
        if matched_all:
            good = candidate
            rewritten += len(group)
            groups += 1
            print(f"{relative(path)}  {group[0].render()}  x{len(group)}  REWRITTEN  "
                  f"({matched} of {len(addresses)} MATCH)")
        else:
            failed.add(key)
            why = "compile failed" if "compile failed" in output else \
                f"{matched} of {len(addresses)} MATCH"
            print(f"{relative(path)}  {group[0].render()}  x{len(group)}  kept  ({why})")
            restore(path, good)
    restore(path, good)
    return rewritten, groups, reasons_of(good, src, views)


def wanted_names(src: Source) -> set[str]:
    """The type names the file defines or the accesses name, so the views
    extraction keeps them (and their address-suffixed spellings)."""
    wanted = {m.group(2) for m in gt.DEFINED.finditer(src.code)}
    wanted |= set(re.findall(r"\b(?:struct|class|union)\s+(" + IDENT[1:-1] + r")", src.code))
    for m in CHAR_CAST.finditer(src.code):
        chain = chain_steps(src.code, m.end())
        if chain is None:
            continue
        decl = decl_of(src, chain[0], m.start())
        if decl:
            wanted.add(decl[0])
            wanted.add(gt.base_name(decl[0]))
    return wanted


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+", type=Path, help="source files to rewrite")
    ap.add_argument("--dry-run", action="store_true", help="print the rewrites, change nothing")
    args = ap.parse_args()

    def stop(signum, frame):
        raise Interrupted(signum)

    signal.signal(signal.SIGINT, stop)
    signal.signal(signal.SIGTERM, stop)
    total = 0
    try:
        for path in args.files:
            if not path.is_file():
                sys.exit(f"no such file: {path}")
        for path in args.files:
            rewritten, groups, reasons = rewrite_file(path, args.dry_run)
            total += rewritten
            if not args.dry_run:
                print(f"{relative(path)}: {rewritten} accesses rewritten in {groups} shapes")
            for reason, n in sorted(reasons.items(), key=lambda r: (-r[1], r[0])):
                print(f"    {n} left: {reason}")
        if not args.dry_run:
            print(f"total: {total} accesses rewritten")
    except (Interrupted, KeyboardInterrupt):
        print("\ninterrupted: the file in hand was restored to its last matching state",
              file=sys.stderr)
        sys.exit(130)


if __name__ == "__main__":
    main()
