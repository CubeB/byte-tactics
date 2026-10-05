"""Group the matched members under src/ into the units they belong to.

    uv run tools/unitmap.py            # write build/units.json and summarise
    uv run tools/unitmap.py --list     # print the units, largest first
    uv run tools/unitmap.py --at 0x4b1000   # the unit nearest an address

A unit is a class whose matched members are spread over more than one file
under src/. Consolidating a unit means giving the class one home, so the map
records for each one:

  members      the matched members, in address order, and the file defining each
  files        every file the unit's members live in
  declared_in  the files that declare the class
  fields       the union of the field ledgers those declarations give, one entry
               per offset, with the other views of the same offset where they
               disagree (the reconciliation report)
  vtable       the vtable address, when data/symbols.csv knows one

Nothing here is a decision: a disagreement is reported, not resolved.

The map is built fresh from data/progress.csv and src/ on every run, so it is
never out of date. build() is the entry point tools/unitgen.py and the two
lookup modes use; the default mode also writes build/units.json, which git
ignores, for inspection.
"""

import argparse
import csv
import json
import re
from pathlib import Path

from check import ROOT, base_name
from sources import primary_address

PROGRESS = ROOT / "data/progress.csv"
SYMBOLS = ROOT / "data/symbols.csv"
UNITS = ROOT / "build/units.json"

ANNOTATION = re.compile(r"^\s*//\s*FUNCTION:")
BLOCK = re.compile(r"^\s*(struct|class|union|enum)\s+([A-Za-z_]\w*)\s*(:[^{;]*)?\s*\{")
FIELD = re.compile(r"^\s*((?:const\s+|unsigned\s+|signed\s+|static\s+)*)"
                   r"([A-Za-z_][\w:]*[\w:<>]*\s*[*&]?)\s+([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*;")
OFFSET = re.compile(r"^\s*\+?\s*0x([0-9a-fA-F]+)")
STL_MEMBERS = ("_Ucopy", "_Ufill", "_Destroy", "_Construct")


def source_of(rel: str) -> str:
    return (ROOT / rel).read_text(errors="replace")


def prelude(text: str) -> str:
    """Everything before the first // FUNCTION: annotation in a file."""
    lines = text.splitlines()
    for i, line in enumerate(lines):
        if ANNOTATION.match(line):
            return "\n".join(lines[:i])
    return text


def class_body(text: str, name: str) -> list[str] | None:
    """The lines inside `class/struct <name> { ... }`, or None."""
    lines = text.splitlines()
    for i, line in enumerate(lines):
        m = BLOCK.match(line)
        if not (m and m.group(2) == name and m.group(1) in ("class", "struct")):
            continue
        depth, body = 0, []
        for j in range(i, len(lines)):
            if j > i:
                body.append(lines[j])
            depth += lines[j].count("{") - lines[j].count("}")
            if j > i and depth <= 0:
                return body[:-1]
        return body
    return None


def class_bases(text: str, name: str) -> list[str]:
    """The base classes the header declares, without access or virtual."""
    for line in text.splitlines():
        m = BLOCK.match(line)
        if m and m.group(2) == name and m.group(1) in ("class", "struct") and m.group(3):
            out = []
            for part in m.group(3).split(","):
                words = [w for w in part.split() if w not in ("public", "private", "protected", "virtual")]
                if words:
                    out.append(words[-1])
            return out
    return []


def fields_of(body: list[str]) -> list[dict]:
    """The data members of a class body, with the offset its comment gives."""
    out = []
    for line in body:
        code, _, comment = line.partition("//")
        if "(" in code:                     # methods, and function pointers
            continue
        m = FIELD.match(code)
        if not m:
            continue
        type_ = (m.group(1) + m.group(2)).strip()
        if type_ in ("public", "private", "protected"):
            continue
        off = OFFSET.search(comment.strip())
        out.append({"offset": int(off.group(1), 16) if off else None,
                    "type": type_, "name": m.group(3), "array": m.group(4) or ""})
    return out


def vtable_address(symbols: dict[str, int], unit: str) -> str | None:
    address = symbols.get(f"??_7{unit}@@6B@")
    return f"{address:#x}" if address else None


def build() -> dict:
    with PROGRESS.open() as fh:
        rows = [r for r in csv.DictReader(fh) if r["status"] == "matched"]
    symbols = {}
    with SYMBOLS.open() as fh:
        for r in csv.DictReader(fh):
            symbols.setdefault(r["name"], int(r["address"], 16))

    members: dict[str, list[dict]] = {}
    for r in rows:
        name = base_name(r["symbol"])
        if "::" not in name:
            continue
        unit = name.split("::")[0]
        if unit in ("std", "type_info") or unit.startswith("std::"):
            continue
        members.setdefault(unit, []).append(
            {"address": r["address"], "name": name, "file": r["file"], "size": int(r["size"] or 0)})

    units = {}
    for unit, mem in sorted(members.items()):
        if len(mem) < 2:                    # one member cannot be consolidated
            continue
        mem.sort(key=lambda m: int(m["address"], 16))
        # In address order (tools/sources.py), wherever the files live.
        files = sorted({m["file"] for m in mem}, key=lambda f: (primary_address(ROOT / f), f))
        if len(files) < 2:                  # already consolidated, nothing to merge
            continue

        ledgers = {}
        for f in files:
            body = class_body(source_of(f), unit)
            if body is not None:
                ledgers[f] = fields_of(body)
        # The richest declaration leads the union; other views become conflicts.
        reference = max(files, key=lambda f: (len(ledgers.get(f, [])), -files.index(f)))
        union: dict[object, dict] = {}
        for f in [reference] + [f for f in files if f != reference]:
            for field in ledgers.get(f, []):
                key = field["offset"] if field["offset"] is not None else field["name"]
                view = (field["type"], field["name"], field["array"])
                entry = union.setdefault(key, {"offset": field["offset"], "type": field["type"],
                                               "name": field["name"], "array": field["array"],
                                               "file": f, "others": []})
                if entry["file"] != f and view != (entry["type"], entry["name"], entry["array"]) \
                        and view not in [(o["type"], o["name"], o["array"]) for o in entry["others"]]:
                    entry["others"].append({"type": field["type"], "name": field["name"],
                                            "array": field["array"], "file": f})
        fields = sorted(union.values(), key=lambda e: (e["offset"] is None, e["offset"] or 0))
        conflicts = [{"offset": e["offset"], "name": e["name"], "file": e["file"],
                      "views": [{"type": e["type"], "name": e["name"], "array": e["array"],
                                 "file": e["file"]}] + e["others"]}
                     for e in fields if e["others"]]
        for c in conflicts:
            offset = f"{c['offset']:#x}" if c["offset"] is not None else c["name"]
            for v in c["views"]:
                v["offset"] = offset

        units[unit] = {
            "kind": "stl" if any(m["name"].split("::")[-1].startswith(STL_MEMBERS)
                                 for m in mem) or unit.startswith("UElem_") else "class",
            "bases": sorted({b for f in files for b in class_bases(source_of(f), unit)}),
            "files": files,
            "declared_in": sorted(ledgers),
            "members": mem,
            "fields": fields,
            "conflicts": conflicts,
            "vtable": vtable_address(symbols, unit),
            "safe": not conflicts,
        }
    return {"units": units}


def matched_rows() -> list[dict]:
    with PROGRESS.open() as fh:
        return [r for r in csv.DictReader(fh) if r["status"] == "matched"]


def unit_of(symbol: str, units: dict) -> str | None:
    """The unit the function named by `symbol` belongs to, if its class is one."""
    name = base_name(symbol)
    if "::" not in name:
        return None
    cls = name.split("::")[0]
    return cls if cls in units else None


def at(addr: int, units: dict) -> None:
    """Report the matched neighbours of an address and the unit nearest them."""
    rows = matched_rows()
    exact = next((r for r in rows if int(r["address"], 16) == addr), None)
    if exact:
        unit = unit_of(exact["symbol"], units)
        where = f" (unit {unit})" if unit else " (no unit)"
        print(f"{exact['address']} is {base_name(exact['symbol'])}{where}")
        if unit:
            report(unit, exact, units[unit])
        return
    below = sorted((r for r in rows if int(r["address"], 16) < addr),
                   key=lambda r: int(r["address"], 16))[-1:]
    above = sorted((r for r in rows if int(r["address"], 16) > addr),
                   key=lambda r: int(r["address"], 16))[:1]
    print(f"0x{addr:x} is not a matched member")
    for label, neighbours in (("below", below), ("above", above)):
        for r in neighbours:
            unit = unit_of(r["symbol"], units)
            where = f" (unit {unit})" if unit else " (no unit)"
            print(f"  nearest matched {label}: {r['address']} "
                  f"{base_name(r['symbol'])}{where}")
    units_near = [unit_of(r["symbol"], units) for r in below + above]
    units_near = [u for u in units_near if u]
    if units_near:
        report(units_near[0], None, units[units_near[0]])


def report(unit: str, member: dict | None, e: dict) -> None:
    size = sum(m["size"] for m in e["members"])
    vtable = e["vtable"] or "-"
    bases = ", ".join(e["bases"]) or "-"
    print(f"{unit}: {len(e['members'])} members, {size} bytes, {len(e['files'])} files, "
          f"vtable {vtable}, bases {bases}")
    print("  fields (offset, type, name, file it came from):")
    if not e["fields"]:
        print("    none parsed")
    for f in e["fields"]:
        off = f"+{f['offset']:#x}" if f["offset"] is not None else "  ?  "
        print(f"    {off:6s} {f['type']} {f['name']}{f['array']}  <- {f['file']}")
    for c in e["conflicts"]:
        views = "  vs  ".join(f"{v['type']} {v['name']}{v['array']} ({v['file']})" for v in c["views"])
        print(f"    in dispute at {c['offset']}: {views}")
    print("  members (address, name, file):")
    for m in e["members"]:
        mark = "  <-" if member and m["address"] == member["address"] else ""
        print(f"    {m['address']}  {m['name']:45s} {m['file']}{mark}")
    print(f"  generate the class with: uv run tools/unitgen.py {unit}")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--list", action="store_true", help="print the units, largest first")
    ap.add_argument("--at", metavar="ADDR",
                    help="print the unit a matched address belongs to, or the nearest one")
    args = ap.parse_args()

    units = build()["units"]
    if args.at:
        try:
            addr = int(args.at, 16)
        except ValueError:
            raise SystemExit(f"--at wants a hex address, not {args.at!r}")
        at(addr, units)
        return
    if args.list:
        for u in sorted(units.values(), key=lambda u: -len(u["members"])):
            size = sum(m["size"] for m in u["members"])
            state = "clean" if u["safe"] else f"{len(u['conflicts'])} in dispute"
            unit = u["members"][0]["name"].split("::")[0]
            print(f"{len(u['members']):3d} members {size:7d}B  {len(u['files']):2d} files  "
                  f"{state:14s} {unit}")
        return
    UNITS.parent.mkdir(parents=True, exist_ok=True)
    UNITS.write_text(json.dumps({"units": units}, indent=1, sort_keys=True) + "\n")
    clean = sum(1 for u in units.values() if u["safe"])
    print(f"{len(units)} units, {sum(len(u['members']) for u in units.values())} functions, "
          f"{len({f for u in units.values() for f in u['files']})} files")
    print(f"  {clean} with agreeing declarations, {len(units) - clean} with an offset in dispute")
    print(f"wrote {UNITS.relative_to(ROOT)} (git ignores it; the map is rebuilt on every run)")


if __name__ == "__main__":
    main()
