"""Count the matched functions a header in front of every file would change.

    uv run tools/stateprobe.py --empty                    # the control: an empty header
    uv run tools/stateprobe.py --externs 700              # 700 unused `extern int` declarations
    uv run tools/stateprobe.py link/globals.h --rename    # a real header, its names prefixed
    uv run tools/stateprobe.py HEADER --area 0x430000     # only the files of one 64 KB area

MSVC 5's register allocation and operand order depend on the compiler's state
before a function, not only on its source (docs/agent-guide.md), so a header
shared by many files can break matches that pass today. Before such a header
goes in (phase 2 of docs/linking.md), this compiles every file holding a
matched function with the header force-included (`/FI`), compares each matched
function's bytes again, and lists those that change. Objects go to
build/probe/; nothing under src/ or data/ is touched.

A header declaring names the files also declare would clash with them;
--rename prefixes every name it declares (and every struct it forward-
declares) with `probe_` first, which keeps its size and shape.
"""

import argparse
import csv
import hashlib
import os
import re
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from check import DEFAULT_FLAGS, ROOT, Original, annotations, compare, compile_source, winpath
from coff import parse_object

PROBES = ROOT / "build/probe"


def renamed(text: str) -> str:
    """The header with each name it declares prefixed with probe_."""
    names = set(re.findall(r"^\s*(?:class|struct|union)\s+(\w+)\s*;", text, re.M))
    names |= set(re.findall(r"\b(\w+)\s*(?:\[[^\]]*\]\s*)*(?:\)\s*\([^;]*\))?\s*;", text))
    names -= {"void", "int", "char", "short", "long", "float", "double", "unsigned", "signed", "const"}
    if not names:
        return text
    pattern = r"\b(" + "|".join(sorted(map(re.escape, names), key=len, reverse=True)) + r")\b"
    return re.sub(pattern, r"probe_\1", text)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("header", type=Path, nargs="?", help="the header to put in front of every file")
    ap.add_argument("--empty", action="store_true", help="an empty header (the control)")
    ap.add_argument("--externs", type=int, help="N unused extern declarations")
    ap.add_argument("--rename", action="store_true", help="prefix the header's names with probe_")
    ap.add_argument("--area", type=lambda s: int(s, 16), help="only functions in this 64 KB window")
    args = ap.parse_args()

    if args.empty:
        text, label = "", "empty"
    elif args.externs is not None:
        text, label = "".join(f"extern int probe_{i};\n" for i in range(args.externs)), f"externs{args.externs}"
    elif args.header:
        text, label = args.header.read_text(), args.header.stem
        if args.rename:
            text = renamed(text)
    else:
        sys.exit("give a header, --empty or --externs N")
    PROBES.mkdir(parents=True, exist_ok=True)
    tag = hashlib.sha256(text.encode()).hexdigest()[:8]
    probe = PROBES / f"{label}-{tag}.h"
    probe.write_text(text)

    with (ROOT / "data/progress.csv").open() as fh:
        matched = [r for r in csv.DictReader(fh) if r["status"] == "matched"]
    if args.area is not None:
        matched = [r for r in matched if int(r["address"], 16) & ~0xFFFF == args.area & ~0xFFFF]
    by_file: dict[str, list[int]] = {}
    for r in matched:
        by_file.setdefault(r["file"], []).append(int(r["address"], 16))

    orig = Original()
    flags = f"{DEFAULT_FLAGS} /FI{winpath(probe)}"

    def run(rel: str):
        src = ROOT / rel
        obj, log = compile_source(src, flags, out_dir=f"probe/{label}-{tag}")
        if obj is None:
            return rel, None, log
        o = parse_object(obj.read_bytes(), obj.name)
        names = dict(annotations(src))
        return rel, [(a, compare(orig, o, a, qualname=names.get(a), symbols={}, quick=True).bytes_match)
                     for a in by_file[rel]], ""

    changed, errors = [], []
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        for rel, results, log in pool.map(run, sorted(by_file)):
            if results is None:
                errors.append(f"{rel}: {log.strip().splitlines()[-1] if log.strip() else 'failed'}")
                continue
            changed += [(a, rel) for a, ok in results if not ok]
    total = len(matched)
    print(f"{probe.relative_to(ROOT)} in front of {len(by_file)} files: {len(changed)} of {total} matched "
          f"functions no longer match ({100 * len(changed) / total:.1f}%)"
          + (f"; {len(errors)} files did not compile" if errors else ""))
    for a, rel in sorted(changed):
        print(f"  {a:#x}  {rel}")
    for e in errors[:20]:
        print(f"  compile error: {e}")


if __name__ == "__main__":
    main()
