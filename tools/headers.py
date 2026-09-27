"""Try a function's file with every combination of common headers.

    uv run tools/headers.py 0x471f90                # the file under src/
    uv run tools/headers.py 0x471f90 scratch.cpp    # or a scratch copy

MSVC 5's register allocation and operand order can depend on which headers a
file includes, even when the function uses nothing from them. When source
rewrites keep producing the same wrong register or a swapped base and index,
this compiles the file once per header set (in parallel, without touching the
original) and prints the sets that match, or the closest ones.
"""

import argparse
import itertools
import os
import re
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from check import ROOT, Original, annotations, compare, compile_source, find_source
from coff import parse_object

HEADERS = ["windows.h", "stdio.h", "stdlib.h", "string.h", "math.h", "memory.h", "ddraw.h"]
INCLUDE = re.compile(r"^\s*#\s*include\s*<(%s)>\s*$" % "|".join(re.escape(h) for h in HEADERS), re.M)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", type=lambda s: int(s, 16))
    ap.add_argument("source", type=Path, nargs="?")
    args = ap.parse_args()

    src = args.source or find_source(args.address)
    if src is None:
        sys.exit(f"no file under src/ has '// FUNCTION: {args.address:#x}'")
    qualname = next((q for a, q in annotations(src) if a == args.address), None)
    body = INCLUDE.sub("", src.read_text())
    work = ROOT / "build/scratch/headers" / f"{args.address:#x}"
    work.mkdir(parents=True, exist_ok=True)
    orig = Original()

    sets = [c for n in range(len(HEADERS) + 1) for c in itertools.combinations(HEADERS, n)]

    def attempt(i_hs):
        i, hs = i_hs
        variant = work / f"v{i:02d}.cpp"
        variant.write_text("".join(f"#include <{h}>\n" for h in hs) + body)
        obj, log = compile_source(variant, out_dir=f"scratch/headers/{args.address:#x}/obj")
        if obj is None:
            return hs, None
        return hs, compare(orig, parse_object(obj.read_bytes(), obj.name), args.address, None, qualname)

    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        results = list(pool.map(attempt, enumerate(sets)))

    ok = [(hs, r) for hs, r in results if r is not None and not r.error]
    if not ok:
        sys.exit("every variant failed to compile or compare; check the file with tools/check.py first")
    ok.sort(key=lambda x: (not x[1].matched, not x[1].bytes_match, -x[1].ratio, len(x[0])))
    best = ok[0][1]
    label = lambda hs: " ".join(f"<{h}>" for h in hs) or "(none of them)"
    if best.bytes_match:
        good = [(hs, r) for hs, r in ok if r.bytes_match]
        print(f"{len(good)} header set(s) give identical bytes; the smallest:")
        for hs, r in good[:6]:
            print(f"  {label(hs)}" + ("" if r.matched else "   (bytes match, a reference is wrong)"))
    else:
        print("no header set gives identical bytes; closest:")
        for hs, r in ok[:5]:
            print(f"  {r.ratio * 100:5.1f}%  {label(hs)}")
    print(f"({len(sets)} header sets tried; {len(results) - len(ok)} failed to compile)")


if __name__ == "__main__":
    main()
