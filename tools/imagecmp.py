"""Compare the linked image with the original: placement and data.

    uv run tools/link.py --carve --map    # build/link/TotalA.exe and its map
    uv run tools/imagecmp.py              # the placement and data reports
    uv run tools/imagecmp.py --verbose    # every run, unit and difference

tools/linkcmp.py checks that every reference in the ordinary link reaches what
the original's does. This is its counterpart for the two questions that only
make sense for the whole image (ideas from LEGO Island's reccmp tools
`roadmap` and `datacmp`):

  * placement. For every game function, its address in the original against
    its address in the linked exe. In an ordinary link every function sits
    where LINK puts it, so what is compared is the order: a run is a stretch
    of functions whose displacement (linked - original) is constant, an island
    of the original's layout. A boundary between runs (a displacement that
    changes) is an inferred object-file boundary, where a whole object sits
    somewhere else in the link order; a boundary that jumps backwards is an
    object placed out of the original's order. The runs also say which of
    tools/unitmap.py's units have their members out of the original's order,
    and the boundaries are grouped by the 64 KB windows of data/areas.csv,
    which measures those area guesses.

  * data. For every global data/globals.csv lists in .rdata or .data, its
    initialised bytes in the linked image against the original's. A pointer
    field is compared by where it leads, not by value, so a pointer to a
    global or a literal is checked as the symbol it names, and a pointer the
    link folded into an identical string elsewhere agrees. Strings and floats
    are shown in readable form. This extends tools/globals.py --check (which
    only covers link/data.cpp) to the linked image, so it reaches globals a
    game file defines and data a static initialiser builds too.

The placement report is a guide for the link order and does not fail. The data
report does: the tool exits non-zero when any global differs, so the later
steps can gate on it (--strict also fails on placement differences).
"""

import argparse
import bisect
import csv
import io
import struct
from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path

import pefile

from carve import Namer, linker_tables
from check import ROOT, base_name, load_symbols
from linkcheck import address_of
from linkcmp import read_map
from place import FUNCTIONS, GLOBALS, load_rows, layout

LINKED = ROOT / "build/link/TotalA.exe"
AREAS = ROOT / "data/areas.csv"
DATA_SECTIONS = (".rdata", ".data")


def load_map(path: Path) -> tuple[dict[str, int], bool]:
    """Public symbol -> linked address, and whether this is a placement build.
    The ordinary link's map is LINK's own, symbol by symbol; a placement build
    (tools/place.py) writes a CSV, its pieces already at the original's
    addresses, so its link is the original."""
    text = path.read_text(errors="replace")
    if text.startswith("address,size,source,symbol,object"):
        out: dict[str, int] = {}
        for row in csv.DictReader(io.StringIO(text)):
            if row["symbol"]:
                out.setdefault(row["symbol"], int(row["address"], 16))
        return out, True
    return read_map(path), False


@dataclass
class Index:
    """A symbol's place in the original and in the link, and the translation
    between the two images the map gives. `linked_of` names an address of the
    original by what the layout placed there (a global, a string, a static);
    `translate` names an address of the link the same way. Linkcmp builds the
    same map for its reference compare."""

    fwd: list[tuple[int, int, int, str]] = field(default_factory=list)   # link, orig, size, name
    rev: list[tuple[int, int, int, str]] = field(default_factory=list)   # orig, link, size, name

    def __post_init__(self) -> None:
        self.fwd.sort()
        self.rev.sort()
        self.fstarts = [x[0] for x in self.fwd]
        self.rstarts = [x[0] for x in self.rev]

    def translate(self, x: int) -> tuple[int | None, str]:
        i = bisect.bisect_right(self.fstarts, x) - 1
        while i >= 0 and x - self.fwd[i][0] < 0x40000:
            la, o, size, name = self.fwd[i]
            if x - la < max(size, 1):
                return o + (x - la), name
            i -= 1
        return None, ""

    def linked_of(self, a: int) -> tuple[int | None, str]:
        i = bisect.bisect_right(self.rstarts, a) - 1
        while i >= 0 and a - self.rev[i][0] < 0x40000:
            start, la, size, name = self.rev[i]
            if a - start < max(size, 1):
                return la + (a - start), name
            i -= 1
        return None, ""


def build_index(img, placer, namer, link_at: dict[str, int], symbols: dict[str, int],
                identity: bool = False) -> Index:
    sizes = {int(r["address"], 16): int(r["size"]) for r in load_rows(FUNCTIONS)}
    orig_of: dict[str, tuple[int, int]] = {}
    for start, end, name in namer.spans + namer.data_spans:
        orig_of.setdefault(name, (start, end - start))
    # origdata.obj's runs of the original's data, one section each (empty now).
    carved = ROOT / "build/link/origdata.obj"
    if carved.exists():
        from place import parse
        obj = parse(carved)
        for name, sym in obj.externals.items():
            if name.startswith("__orig_"):
                orig_of[name] = (int(name[len("__orig_"):], 16), len(obj.secs[sym.section - 1].data))
    for name in link_at:
        if name in orig_of:
            continue
        if name.startswith("__static_"):
            # A static the link gave a public name: the placed piece at its address.
            a = int(name[len("__static_"):], 16)
            pid = img.owner[a - img.base]
            p = placer.pieces[pid - 1] if pid else None
            orig_of[name] = (a, p.hi - p.lo if p else 4)
        else:
            a = address_of(name, symbols)
            if a is not None:
                orig_of[name] = (a, sizes.get(a, 4))
    if identity:
        # A placement build: every piece is already at its original address.
        fwd = [(s, s, sz, n) for n, (s, sz) in orig_of.items()]
    else:
        fwd = [(link_at[n], v[0], v[1], n) for n, v in orig_of.items() if n in link_at]
    rev = [(v[0], v[0], v[1], n) for n, v in orig_of.items()] if identity else \
          [(v[0], link_at[n], v[1], n) for n, v in orig_of.items() if n in link_at]
    return Index(fwd, rev)


@dataclass
class Run:
    """A stretch of functions whose displacement is constant in link order."""
    start: int
    disp: int
    members: list[tuple[int, int, str]] = field(default_factory=list)   # orig, link, name


def placement_report(index: Index, verbose: bool) -> int:
    entries = []
    unresolved = 0
    for r in load_rows(FUNCTIONS):
        if r["kind"] not in ("game", "gap"):
            continue
        a = int(r["address"], 16)
        la, via = index.linked_of(a)
        if la is None:
            unresolved += 1
            continue
        entries.append((a, la, base_name(via) if via else f"FUN_{a:08x}"))
    linksort = sorted(entries, key=lambda t: t[1])
    runs: list[Run] = []
    for a, la, name in linksort:
        disp = la - a
        if runs and runs[-1].disp == disp:
            runs[-1].members.append((a, la, name))
        else:
            runs.append(Run(a, disp, [(a, la, name)]))
    backward = sum(1 for i in range(1, len(runs)) if runs[i].disp < runs[i - 1].disp)
    print("placement:")
    print(f"  functions resolved: {len(entries):,} of {len(entries) + unresolved:,} game and gap "
          f"functions in data/functions.csv" + (f" ({unresolved} not in the map)" if unresolved else ""))
    print(f"  placement runs (link order, constant displacement): {len(runs):,}")
    print(f"  inferred object boundaries: {len(runs) - 1:,} "
          f"({len(runs) - 1 - backward} forward, {backward} backwards)")

    areas = [(int(r["window"], 16), r["area"]) for r in csv.DictReader(AREAS.open())]
    windows = [w for w, _ in areas]

    def area_of(a: int) -> str:
        i = bisect.bisect_right(windows, a) - 1
        return areas[i][1] if i >= 0 else "before the image"

    if len(runs) > 1:
        by_area = Counter(area_of(r.start) for r in runs[1:])
        print("  boundaries by area (data/areas.csv):")
        for name, n in by_area.most_common():
            print(f"    {n:5,}  {name}")

    # Units whose members are not in the original's order in the link.
    import unitmap
    units = unitmap.build()["units"]
    out_of_order = []
    for u in units.values():
        members = sorted(u["members"], key=lambda m: int(m["address"], 16))
        seq = []
        for m in members:
            la, _ = index.linked_of(int(m["address"], 16))
            if la is not None:
                seq.append((int(m["address"], 16), la, m["name"]))
        inversions = [(seq[i], seq[i + 1]) for i in range(len(seq) - 1) if seq[i + 1][1] < seq[i][1]]
        if inversions:
            out_of_order.append((len(seq), len(inversions), members[0]["name"].split("::")[0], inversions))
    print(f"  unitmap units whose link order differs: {len(out_of_order):,} of {len(units):,}")
    shown = out_of_order if verbose else sorted(out_of_order, reverse=True)[:10]
    for n, inv, name, inversions in sorted(shown, reverse=True):
        (_, l0, m0), (_, l1, m1) = inversions[0]
        print(f"    {name}: {n} members, {inv} inversion(s); e.g. {m0} at {l0:#x}, then {m1} at {l1:#x}")

    if verbose:
        print("  every boundary:")
        for r in runs[1:]:
            print(f"    {r.start:#x}  displacement {r.disp:#x}, run of {len(r.members)}")
    else:
        print(f"  first boundaries ({len(runs) - 1:,} in all; --verbose for every one):")
        for r in runs[1:11]:
            print(f"    {r.start:#x}  displacement {r.disp:#x}, run of {len(r.members)}")

    placement_fail = bool(backward or out_of_order)
    return 1 if placement_fail else 0


def readable(row: dict, raw: bytes) -> str:
    """A global's bytes in the form its kind reads best."""
    if row["kind"] == "string":
        text = raw.split(b"\0", 1)[0]
        return repr(text.decode("cp1252", "replace"))
    if row["kind"] == "float" and len(raw) >= 8:
        (v,) = struct.unpack_from("<d", raw, 0)
        return repr(v)
    if row["kind"] == "float" and len(raw) >= 4:
        (v,) = struct.unpack_from("<f", raw, 0)
        return repr(v)
    return raw[:16].hex()


def data_report(index: Index, img, namer, image: bytes, base: int, symbols: dict[str, int],
                verbose: bool) -> int:
    by_addr = {a: n for n, a in symbols.items()}
    data_lo = namer.sections[".rdata"][0]
    data_hi = namer.sections[".data"][0] + namer.sections[".data"][1]
    tables = bytearray(img.size)                    # LINK builds these itself: not compared
    for va, n in linker_tables(img):
        tables[va - img.base:va - img.base + n] = b"\1" * n

    def orig_text(va: int) -> bytes | None:
        if not data_lo <= va < data_hi:
            return None
        raw = bytes(img.pristine[va - img.base:va - img.base + 512])
        return raw.split(b"\0", 1)[0] if b"\0" in raw else None

    def link_text(va: int) -> bytes | None:
        if not base <= va < base + len(image):
            return None
        raw = image[va - base:va - base + 512]
        return raw.split(b"\0", 1)[0] if b"\0" in raw else None

    rows = [r for r in load_rows(GLOBALS)
            if r["section"] in DATA_SECTIONS and int(r["size"] or 0) > 0]
    compared, not_located, diffs = 0, [], []
    stats = Counter()
    for row in rows:
        a, size = int(row["address"], 16), int(row["size"])
        la, _ = index.linked_of(a)
        if la is None:
            not_located.append(row)
            continue
        compared += 1
        ours = bytes(image[la - base: la - base + size]).ljust(size, b"\0")
        theirs = bytes(img.pristine[a - img.base:a - img.base + size]).ljust(size, b"\0")
        if ours == theirs:
            continue
        wrong, i = [], 0
        while i < size:
            if tables[a - img.base + i]:
                i += 1
                continue
            if ours[i] == theirs[i]:
                i += 1
                continue
            for j in range(max(0, i - 3), i + 1):
                if j + 4 > size:
                    continue
                mine, want = struct.unpack_from("<I", ours, j)[0], struct.unpack_from("<I", theirs, j)[0]
                mapped, _ = index.translate(mine)
                if mapped == want:
                    stats["pointer fields that agree"] += 1
                    i = j + 4
                    break
                if orig_text(want) is not None and link_text(mine) == orig_text(want):
                    # The same string at another address: the link folded the
                    # literal into another object's identical one.
                    stats["pointer fields that reach an identical string"] += 1
                    i = j + 4
                    break
            else:
                wrong.append(i)
                i += 1
        if wrong:
            diffs.append((row, ours, theirs, wrong[0]))

    print("data:")
    print(f"  globals compared: {compared:,} of {len(rows):,} in .rdata/.data "
          f"(data/globals.csv lists {len(load_rows(GLOBALS)):,})"
          + (f"; {len(not_located)} not placed in the link" if not_located else ""))
    for k, v in sorted(stats.items()):
        print(f"  {k}: {v:,}")
    print(f"  pieces of data that differ: {len(diffs):,}")
    for row in not_located:
        print(f"    {row['address']} {row['name']}: not placed in the link")
    for row, ours, theirs, off in (diffs if verbose else diffs[:30]):
        where = f"{row['address']} {row['name']} ({row['kind']}, {row['size']} bytes)"
        if row["kind"] in ("string", "float"):
            print(f"    {where}: ours {readable(row, ours)}, the original's {readable(row, theirs)}")
        else:
            want_name = base_name(by_addr.get(struct.unpack_from('<I', theirs, off)[0], "?"))
            print(f"    {where}: +{off:#x} ours {ours[off:off + 4].hex()}, "
                  f"the original's {theirs[off:off + 4].hex()} ({want_name})")
    if not verbose and len(diffs) > 30:
        print(f"    ... {len(diffs) - 30} more")
    return 1 if diffs or not_located else 0


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe", type=Path, default=LINKED,
                    help="the linked exe (default: build/link/TotalA.exe)")
    ap.add_argument("--map", type=Path, help="its map (default: the exe's, with .map)")
    ap.add_argument("--strict", action="store_true",
                    help="also exit non-zero on a placement difference (an out-of-order object or unit)")
    ap.add_argument("--verbose", "-v", action="store_true",
                    help="list every boundary, every out-of-order unit and every data difference")
    args = ap.parse_args()

    exe = args.exe if args.exe.is_absolute() else ROOT / args.exe
    map_path = args.map or exe.with_suffix(".map")
    if not exe.exists():
        raise SystemExit(f"{exe.relative_to(ROOT)} is missing: run tools/link.py --carve --map")
    if not map_path.exists():
        raise SystemExit(f"{map_path.relative_to(ROOT)} is missing: link with --map")

    link_at, placement = load_map(map_path)
    symbols = load_symbols()
    img, placer = layout()
    namer = Namer(img, placer)
    index = build_index(img, placer, namer, link_at, symbols, placement)
    linked = pefile.PE(str(exe))
    base = linked.OPTIONAL_HEADER.ImageBase
    image = linked.get_memory_mapped_image()

    placement_bad = placement_report(index, args.verbose)
    print()
    data_bad = data_report(index, img, namer, image, base, symbols, args.verbose)
    raise SystemExit(1 if data_bad or (args.strict and placement_bad) else 0)


if __name__ == "__main__":
    main()
