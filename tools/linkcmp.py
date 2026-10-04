"""Compare an ordinary link with the original: does every reference reach
what the original's does?

    uv run tools/link.py --carve --map   # build/link/TotalA.exe and its map
    uv run tools/linkcmp.py              # compare it with orig/TotalA.exe
    uv run tools/linkcmp.py --verbose    # list every difference

In an ordinary link every function sits where LINK puts it, so comparing
addresses with the original says little. What must agree is where each
reference leads. For every game function tools/place.py places, this reads
each relocated field in the linked exe, maps the address it holds back to the
original through the map file (a public symbol of the link covering it, and
that symbol's address in the original), and compares the result with the
address the original's code holds in the same field. A difference means the
link bound a name to something other than what the original calls or uses: a
wrong alias, a copy kept so that it inlines, a stub, or a second copy of a
global.

References to a function's own constants and string literals are skipped
(each object keeps its own copies, with the same contents), and so are the
import slots (LINK builds its own table). Calls to std::_Lockit, which the
original links twice (data/aliases.csv), reach one copy instead of two.
"""

import argparse
import bisect
import re
import struct
from collections import Counter
from pathlib import Path

import pefile

from carve import Namer
from check import ROOT, load_symbols
from linkcheck import address_of
from place import CODE, FUNCTIONS, GAPCODE, REL_DIR32, REL_REL32, layout, load_rows

LINKED = ROOT / "build/link/TotalA.exe"
SCN_MEM_WRITE = 0x80000000


def read_map(path: Path) -> dict[str, int]:
    """Public symbol -> address, from a LINK map file."""
    out: dict[str, int] = {}
    for line in path.read_text(errors="replace").splitlines():
        m = re.match(r"\s*[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8})", line)
        if m:
            out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe", type=Path, default=LINKED)
    ap.add_argument("--verbose", "-v", action="store_true")
    args = ap.parse_args()
    map_path = args.exe.with_suffix(".map")
    if not map_path.exists():
        raise SystemExit(f"{map_path.relative_to(ROOT)} is missing: link with --map")

    img, placer = layout()
    namer = Namer(img, placer)
    symbols = load_symbols()
    linked = pefile.PE(str(args.exe))
    base = linked.OPTIONAL_HEADER.ImageBase
    image = linked.get_memory_mapped_image()
    link_at = read_map(map_path)

    # Where each public of the link sits in the original, and how far it runs.
    sizes = {int(r["address"], 16): int(r["size"]) for r in load_rows(FUNCTIONS)}
    orig_of: dict[str, tuple[int, int]] = {}
    for start, end, name in namer.spans:
        orig_of.setdefault(name, (start, end - start))
    for name in link_at:
        if name in orig_of:
            continue
        if name.startswith("__orig_"):
            start, size = namer.sections["." + name[len("__orig_"):]]
            orig_of[name] = (start, size)
        else:
            a = address_of(name, symbols)
            if a is not None:
                orig_of[name] = (a, sizes.get(a, 4))
    pairs = sorted((link_at[n], n) for n in orig_of if n in link_at)
    link_starts = [a for a, _ in pairs]

    def translate(x: int) -> tuple[int | None, str]:
        i = bisect.bisect_right(link_starts, x) - 1
        while i >= 0 and x - pairs[i][0] < 0x40000:
            la, name = pairs[i]
            o, size = orig_of[name]
            if x - la < max(size, 1):
                return o + (x - la), name
            i -= 1
        return None, ""

    copies = {"??0_Lockit@std@@QAE@XZ", "??1_Lockit@std@@QAE@XZ"}
    stats = Counter()
    lines = []
    for p in placer.pieces:
        if p.source not in (CODE, GAPCODE) or p.obj.library:
            continue
        names = [s.name for s in p.obj.syms.values()
                 if s.section == p.sec.index and s.sclass == 2 and s.value == p.lo]
        at = next((link_at[n] for n in names if n in link_at), None)
        if at is None:
            stats["functions with no public name in the link"] += 1
            continue
        stats["functions compared"] += 1
        for off, symidx, rtype in p.sec.relocs:
            if not p.lo <= off <= p.hi - 4 or rtype not in (REL_DIR32, REL_REL32):
                continue
            sym = p.obj.syms[symidx]
            if sym.section > 0:
                target = p.obj.secs[sym.section - 1]
                if target is p.sec or (not target.is_code and not target.chars & SCN_MEM_WRITE):
                    continue                 # jump tables, its own constants and literals
                if target.name.startswith(".data") and sym.name.startswith("??_C@"):
                    continue
            if sym.name.startswith("__imp_"):
                continue
            site_o, site_l = p.va + off - p.lo, at + off - p.lo
            want = img.u32(site_o)
            got = struct.unpack_from("<I", image, site_l - base)[0]
            if rtype == REL_REL32:
                want = (site_o + 4 + want) & 0xFFFFFFFF
                got = (site_l + 4 + got) & 0xFFFFFFFF
            mapped, via = translate(got)
            if mapped is None:
                stats["references the map cannot place"] += 1
                if args.verbose:
                    lines.append(f"  {p.label} +{off - p.lo:#x}: {sym.name} -> {got:#x}, not in the map")
                continue
            if mapped == want:
                stats["references that agree"] += 1
            elif sym.name in copies:
                stats["references to the other copy of std::_Lockit"] += 1
            else:
                stats["references that differ"] += 1
                lines.append(f"  {p.label} +{off - p.lo:#x}: {sym.name} -> {got:#x} = {mapped:#x} in the "
                             f"original ({via}), where the original's holds {want:#x}")
    for k, v in sorted(stats.items()):
        print(f"{k}: {v:,}")
    differ = [l for l in lines if "where the original's holds" in l]
    if lines:
        print("\n".join(lines if args.verbose else differ[:30]))
    raise SystemExit(1 if differ else 0)


if __name__ == "__main__":
    main()
