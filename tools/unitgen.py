"""Emit one merged unit source from the unit map and the files in src/.

    uv run tools/unitgen.py Class_004b0610
    uv run tools/unitgen.py Class_004b0610 -o build/units/Class_004b0610.cpp

The unit's members live in several files, and each file declares its own view of
the classes they use. This emits one candidate file:

  - every type the unit's files declare is emitted once, with the views of that
    name merged: the base list from the view that has one, fields from the
    reference file's view (that is the layout its matched functions were built
    against) plus any field the other views declare that a member body actually
    names, and methods unioned by name;
  - the reference file (the one carrying the most fields) keeps its order, so
    `#pragma pack` regions and the order of virtual slots survive;
  - every other file's remaining declarations are inserted above the first
    place that needs them, where the merged class can see them;
  - every member's definition is copied from the file that holds it, in address
    order.

The exception is a type name the unit's files do not declare at all: those are
not pulled in (see docs/consolidation.md), and the generator says so.

Every line comes from an existing file; nothing is invented. Where two views
disagree about a field the choice is a heuristic, so the unit's conflicts from
the map are printed and the candidate is not a commit. Merging changes the
compiler state and the inline budget, so every function has to be re-checked
with tools/check.py once the file compiles.

Writes under build/ by default, which git ignores.
"""

import argparse
import re
from pathlib import Path

from check import ROOT
from unitmap import build

ANNOTATION = re.compile(r"^\s*//\s*FUNCTION:\s*(0x[0-9a-fA-F]+)")
BLOCK = re.compile(r"^\s*(struct|class|union|enum)\s+([A-Za-z_]\w*)\s*(:[^{;]*)?\s*\{")
FORWARD = re.compile(r"^\s*(struct|class|union|enum)\s+([A-Za-z_]\w*)\s*;\s*$")
FIELD = re.compile(r"^\s*((?:const\s+|unsigned\s+|signed\s+|static\s+)*)"
                   r"([A-Za-z_][\w:]*[\w:<>]*\s*[*&]?)\s+([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*;")
SCALARS = ("int", "unsigned", "char", "short", "long", "bool", "float", "double", "void", "__int64")


def read(rel: str) -> str:
    return (ROOT / rel).read_text(errors="replace")


def prelude_of(text: str) -> list[str]:
    lines = text.splitlines()
    for i, line in enumerate(lines):
        if ANNOTATION.match(line):
            return lines[:i]
    return lines


def find_block(lines: list[str], name: str) -> tuple[int, int] | None:
    """(start, end) inclusive of the `class/struct <name> { ... };` block."""
    for i, line in enumerate(lines):
        m = BLOCK.match(line)
        if not (m and m.group(2) == name and m.group(1) in ("class", "struct", "union", "enum")):
            continue
        depth = 0
        for j in range(i, len(lines)):
            depth += lines[j].count("{") - lines[j].count("}")
            if depth <= 0:
                return i, j
    return None


def expand_block(lines: list[str], i: int) -> list[str]:
    """The whole block at `i`, one-liners split into a header, body and close."""
    end = i
    depth = 0
    while end < len(lines):
        depth += lines[end].count("{") - lines[end].count("}")
        if depth <= 0:
            break
        end += 1
    if end > i:
        return lines[i:end + 1]
    text = lines[i]
    head, _, rest = text.partition("{")
    body, _, _ = rest.rpartition("}")
    items = [part.strip() for part in body.split(";") if part.strip()]
    return [head.rstrip() + " {"] + ["    " + item + ";" for item in items] + ["};"]


def split_prelude(lines: list[str]) -> list[tuple[str, str | None, list[str]]]:
    """('block', name, lines) for definitions, ('text', None, [line]) for the rest."""
    out, i = [], 0
    while i < len(lines):
        m = BLOCK.match(lines[i])
        if not m:
            out.append(("text", None, [lines[i]]))
            i += 1
            continue
        end, depth = i, 0
        while end < len(lines):
            depth += lines[end].count("{") - lines[end].count("}")
            if depth <= 0:
                break
            end += 1
        out.append(("block", m.group(2), expand_block(lines, i)))
        i = end + 1
    return out


KEYWORDS = {
    "auto", "bool", "break", "case", "char", "class", "const", "continue", "default",
    "delete", "do", "double", "else", "enum", "explicit", "extern", "float", "for",
    "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new",
    "operator", "private", "protected", "public", "register", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "template", "this", "throw",
    "try", "typedef", "typename", "union", "unsigned", "using", "virtual", "void",
    "volatile", "while", "wchar_t", "__cdecl", "__stdcall", "__fastcall", "__int64",
}


def declared_name(line: str) -> str | None:
    """The name a prelude line declares, or None when it declares nothing (or the
    name cannot be told apart from a type keyword)."""
    code = line.split("//")[0].rstrip()
    if not code or code.startswith("#include") or code.startswith("#pragma"):
        return None
    if code.startswith("#define"):
        m = re.match(r"#define\s+(\w+)", code)
        return m.group(1) if m else None
    m = FORWARD.match(code)
    if m:
        return m.group(2)
    if code.startswith("typedef"):
        m = re.search(r"\(\s*[^()]*?\*+\s*([A-Za-z_]\w*)\s*\)", code)
        if not m:
            m = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;\s*$", code)
        name = m.group(1) if m else None
        return name if name not in KEYWORDS else None
    code = re.sub(r"__declspec\s*\([^()]*\)", "", code)
    code = re.sub(r'"[^"]*"', "", code)
    code = code.split("=")[0]
    if "(" in code:
        m = re.match(r"^[^()]*?([A-Za-z_]\w*)\s*\(", code)
    else:
        m = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;?\s*$", code)
    name = m.group(1) if m else None
    return name if name not in KEYWORDS else None


def fields_of(body: list[str]) -> dict[object, dict]:
    """key (offset, or name when no offset) -> the field and the line it is on."""
    out = {}
    for line in body:
        code = line.split("//")[0]
        if "(" in code:
            continue
        m = FIELD.match(code)
        if not m:
            continue
        type_ = (m.group(1) + m.group(2)).strip()
        if type_ in ("public", "private", "protected"):
            continue
        off = re.search(r"^\s*\+?\s*0x([0-9a-fA-F]+)", line.partition("//")[2].strip())
        offset = int(off.group(1), 16) if off else None
        key = offset if offset is not None else m.group(3)
        out[key] = {"offset": offset, "type": type_, "name": m.group(3),
                    "array": m.group(4) or "", "line": line}
    return out


def methods_of(body: list[str]) -> list[tuple[str, str]]:
    """(name, line) for each line of a class body that introduces a function,
    whether it ends in `;` (a declaration) or in `)`/`{` (an inline definition)."""
    out = []
    for line in body:
        code = line.split("//")[0].strip()
        if "(" not in code:
            continue
        head = code.split("(")[0].split()
        if not head:
            continue
        name = head[-1].lstrip("*&")
        if re.match(r"^[A-Za-z_~]", name):
            out.append((name, line))
    return out


def specificity(type_: str, array: str) -> int:
    """A crude ranking for choosing between two views of one field. void* loses
    to a named pointer; a scalar loses to a named type. Deliberately simple: the
    choice is reported as a conflict and the reviewer decides."""
    t = type_.replace(" ", "")
    base = t.rstrip("*&")
    named = bool(re.match(r"^[A-Za-z_]\w*$", base)) and (base[0].isupper() or "_" in base)
    if t == "void*":
        return 0
    if t.endswith(("*", "&")):
        return 5 if named else 1
    if array:
        return 4 if named else 2
    return 3 if named else 1 if base in SCALARS else 2


def body_tokens(order: list[str]) -> set[str]:
    """Every identifier that appears in the unit's member definitions, which is
    what a merged declaration has to provide a name for."""
    tokens: set[str] = set()
    for f in order:
        lines = read(f).splitlines()
        for i, line in enumerate(lines):
            if ANNOTATION.match(line):
                tokens |= set(re.findall(r"[A-Za-z_]\w*", "\n".join(lines[i:])))
                break
    return tokens


def merge_views(views: list[dict], reference: str, needed: set[str]) -> tuple[str, list[str]]:
    """One declaration from every file's view of the same name.

    The base list comes from the view that has one (a packed layout struct has
    none). Fields come from the reference file's view when it declares any (that
    is the layout the reference file's matched functions were compiled against),
    else from the richest view, and the other views fill in fields the chosen one
    lacks, matched by offset when one is given and by position otherwise. Methods
    are unioned by name, so a view that left a method out does not remove it."""
    header = views[0]["header"]
    for v in views:
        m = BLOCK.match(v["header"])
        if m and m.group(3):
            header = v["header"]
            break
    ref_views = [v for v in views if v["file"] == reference and fields_of(v["body"])]
    if ref_views:
        best = ref_views[0]
    else:
        best = max(views, key=lambda v: (len(fields_of(v["body"])), len(v["body"])))

    chosen = list(fields_of(best["body"]).values())
    by_offset = {f["offset"]: f for f in chosen if f["offset"] is not None}
    others = [v for v in views if v is not best]
    for v in others:
        for index, field in enumerate(fields_of(v["body"]).values()):
            if field["offset"] is not None:
                match = by_offset.get(field["offset"])
                if match is None:
                    match = next((f for f in chosen if f["name"] == field["name"]), None)
            elif index < len(chosen) and chosen[index]["offset"] is None:
                match = chosen[index]
            else:
                match = next((f for f in chosen if f["name"] == field["name"]), None)
            if match is not None:
                if field["name"] == match["name"] and \
                        specificity(field["type"], field["array"]) > \
                        specificity(match["type"], match["array"]):
                    chosen[chosen.index(match)] = field
                elif field["name"] in needed and match["name"] not in needed:
                    chosen[chosen.index(match)] = field
                elif field["name"] in needed and field["name"] != match["name"]:
                    # Two views of one offset that both bodies name: keep both,
                    # the reviewer decides which one is the real field.
                    chosen.append(field)
                continue
            if field["name"] not in needed:
                continue
            chosen.append(field)
            if field["offset"] is not None:
                by_offset[field["offset"]] = field

    body, methods, seen = [], set(), 0
    for line in best["body"]:
        if fields_of([line]):
            if seen < len(chosen):
                body.append(chosen[seen]["line"])
                seen += 1
            continue
        for name, _ in methods_of([line]):
            methods.add(name)
        body.append(line)
    while seen < len(chosen):
        body.append(chosen[seen]["line"])
        seen += 1
    for v in others:
        for name, line in methods_of(v["body"]):
            if name not in methods:
                methods.add(name)
                body.append(line)
    return header, body


def annotations(lines: list[str]) -> list[tuple[int, str]]:
    return [(i, m.group(1)) for i, line in enumerate(lines) if (m := ANNOTATION.match(line))]


def brace_end(lines: list[str], start: int) -> int:
    depth, seen = 0, False
    for i in range(start, len(lines)):
        for ch in lines[i].split("//")[0]:
            if ch == "{":
                depth, seen = depth + 1, True
            elif ch == "}":
                depth -= 1
        if seen and depth <= 0:
            return i
    return len(lines) - 1


def member_span(lines: list[str], anns: list[tuple[int, str]], address: str) -> list[str]:
    """The definition at `address`: its annotation, any body-less annotation
    before it, and the definition itself."""
    for i, addr in anns:
        if addr != address:
            continue
        end, j = i, i
        while True:
            rest = [l for l in lines[j + 1:] if l.strip() and not l.strip().startswith("//")]
            if rest and ("(" in rest[0] or "=" in rest[0]):
                return lines[i:brace_end(lines, j + 1) + 1]
            later = [a for a in anns if a[0] > j]
            if not later:
                break
            j = later[0][0]
            end = j
        return lines[i:end + 1]
    return []


def collect_views(order: list[str]) -> dict[str, list[dict]]:
    """name -> one view per file that declares it, in `order`."""
    views: dict[str, list[dict]] = {}
    for f in order:
        for kind, name, ls in split_prelude(prelude_of(read(f))):
            if kind != "block":
                continue
            views.setdefault(name, []).append(
                {"file": f, "header": ls[0], "body": ls[1:-1]})
    return views


def rewrite_prelude(lines: list[str], end: int,
                    merged: dict[str, tuple[str, list[str]]]) -> tuple[list[str], set[str], set[str]]:
    """Replace each block in lines[:end] by its merged view. The reference file's
    other lines are kept verbatim (it compiles on its own). Returns the lines,
    the names a block defines, and every name a line declares."""
    out, defined, declared = [], set(), set()
    i = 0
    while i < end:
        m = BLOCK.match(lines[i])
        if m:
            name = m.group(2)
            j = i
            depth = 0
            while j < end:
                depth += lines[j].count("{") - lines[j].count("}")
                if depth <= 0:
                    break
                j += 1
            default = expand_block(lines, i)
            header, body = merged.get(name, (default[0], default[1:-1]))
            out += [header] + body + ["};"]
            defined.add(name)
            declared.add(name)
            i = j + 1
            continue
        name = declared_name(lines[i])
        if name is not None:
            declared.add(name)
        out.append(lines[i])
        i += 1
    return out + lines[end:], defined, declared


def gather_extra(order: list[str], defined: set[str], declared: set[str], seen_text: set[str],
                 merged: dict[str, tuple[str, list[str]]]) -> tuple[list[str], list[str]]:
    """The declarations the other files add, with the same name dedup. A block is
    skipped only when the name is already defined (a forward declaration in the
    reference does not replace a definition here)."""
    extra, pulled = [], []
    for f in order:
        for kind, name, ls in split_prelude(prelude_of(read(f))):
            if kind == "block":
                if name in defined:
                    continue
                header, body = merged.get(name, (ls[0], ls[1:-1]))
                extra += [""] + [header] + body + ["};"]
                defined.add(name)
                pulled.append(name)
            else:
                line = ls[0]
                if not line.strip() or line.strip().startswith("//"):
                    continue
                nm = declared_name(line)
                if nm is not None:
                    if nm in defined or nm in declared:
                        continue
                    declared.add(nm)
                elif line.strip() in seen_text:
                    continue
                seen_text.add(line.strip())
                extra.append(line)
        extra.append("")
    return extra, pulled


def first_use(lines: list[str], names: list[str]) -> int | None:
    """The first line before the first annotation that names a pulled type, not
    counting comments; never before the last #include."""
    wanted = set(names)
    floor = 0
    for i, line in enumerate(lines):
        if ANNOTATION.match(line):
            break
        if line.strip().startswith("#include"):
            floor = i + 1
    for i, line in enumerate(lines):
        if ANNOTATION.match(line):
            break
        if i < floor:
            continue
        if wanted & set(re.findall(r"[A-Za-z_]\w*", line.split("//")[0])):
            return i
    return None


def enclosing_start(lines: list[str], at: int) -> int:
    """The start of the outermost block containing `at`, pulled back over a
    `#pragma pack(push, ...)` that encloses it."""
    best = at
    for i in range(at + 1):
        if not BLOCK.match(lines[i]):
            continue
        depth, j = 0, i
        while j < len(lines):
            depth += lines[j].count("{") - lines[j].count("}")
            if depth <= 0:
                break
            j += 1
        if i <= at <= j and i < best:
            best = i
    if best > 0 and lines[best - 1].strip().startswith("#pragma pack(push"):
        best -= 1
    return best


def generate(unit: str, entry: dict) -> tuple[str, str, list[str], list[str], list[str], list[str]]:
    files, members = entry["files"], entry["members"]

    def weight(f: str) -> tuple[int, int]:
        return (sum(1 for e in entry["fields"] if e["file"] == f),
                sum(1 for m in members if m["file"] == f))

    reference = max(files, key=weight)
    order = [reference] + [f for f in files if f != reference]

    views = collect_views(order)
    needed = body_tokens(order)
    merged = {name: merge_views(v, reference, needed) for name, v in views.items()}

    ref_lines = read(reference).splitlines()
    pre_end = len(ref_lines)
    for i, line in enumerate(ref_lines):
        if ANNOTATION.match(line):
            pre_end = i
            break

    out, defined, declared = rewrite_prelude(ref_lines, pre_end, merged)

    # The merged class uses types declared by the other files, so those have to
    # sit above it, not below.
    seen_text = {l.strip() for l in out if l.strip()}
    extra, pulled = gather_extra(order[1:], defined, declared, seen_text, merged)
    use = first_use(out, pulled)
    block = find_block(out, unit)
    if use is not None:
        insert_at = enclosing_start(out, use)
    elif block:
        insert_at = block[0]
    else:
        insert_at = next((i for i, l in enumerate(out) if ANNOTATION.match(l)), len(out))
    out = out[:insert_at] + extra + out[insert_at:]

    ref_methods = {n for name, vs in views.items() if vs[0]["file"] == reference
                   for n, _ in methods_of(vs[0]["body"])}
    unit_body = merged.get(unit, ("", []))[1]
    added = [n for n, _ in methods_of(unit_body) if n not in ref_methods]

    emitted = {a for _, a in annotations(out)}
    for f in order[1:]:
        fl = read(f).splitlines()
        anns = annotations(fl)
        for m in sorted((m for m in members if m["file"] == f), key=lambda m: int(m["address"], 16)):
            if m["address"] in emitted:
                continue
            span = member_span(fl, anns, m["address"])
            if span:
                out += [""] + span
                emitted.add(m["address"])

    missing = [m["address"] for m in members if m["address"] not in emitted]
    unresolved = [b for b in entry.get("bases", []) if b not in defined and b not in pulled]
    return "\n".join(out) + "\n", reference, pulled, added, missing, unresolved


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("unit")
    ap.add_argument("-o", "--output", type=Path)
    args = ap.parse_args()

    units = build()["units"]
    if args.unit not in units:
        near = [u for u in units if args.unit.lower() in u.lower()][:5]
        raise SystemExit(f"no unit {args.unit!r} in the map"
                         + (f", did you mean: {', '.join(near)}" if near else ""))
    entry = units[args.unit]

    text, reference, pulled, added, missing, unresolved = generate(args.unit, entry)
    dest = args.output or ROOT / "build/units" / f"{args.unit}.cpp"
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_text(text)

    print(f"wrote {dest.relative_to(ROOT)} ({text.count(chr(10))} lines), "
          f"{len(entry['members'])} members from {len(entry['files'])} files")
    print(f"  reference declaration: {reference}")
    if added:
        print(f"  members added to the declaration: {', '.join(added)}")
    if pulled:
        print(f"  types pulled in: {', '.join(pulled)}")
    if missing:
        print(f"  NOT emitted: {', '.join(missing)}")
    if unresolved:
        print(f"  base class(es) not declared in the unit's files: {', '.join(unresolved)}")
    if entry["conflicts"]:
        print(f"  {len(entry['conflicts'])} offset(s) in dispute, resolved by the specificity rule:")
        for c in entry["conflicts"]:
            views = "  vs  ".join(f"{v['type']} {v['name']}{v['array']}" for v in c["views"])
            print(f"    {c['offset']}  {views}")


if __name__ == "__main__":
    main()
