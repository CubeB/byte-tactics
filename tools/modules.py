"""Where each source file belongs: data/modules.csv and the file names.

    uv run tools/modules.py 0x401070          # the file, module and folder an address belongs to
    uv run tools/modules.py --summary         # files per folder and module
    uv run tools/modules.py --check           # files not where data/modules.csv puts them
    uv run tools/modules.py --move [--folder orders ...] [--dry-run]
                                              # git mv them there (only into those folders)

data/modules.csv cuts the game's code into address ranges, one row per
range, from `start` to the next row's start. Each range is a module, a guess
at one of Cavedog's translation units (the linker kept each one's functions
together), and sits in a folder of src/ by subsystem; `evidence` says what
the guess rests on (strings, call graphs, classes, docs/consolidation.md).
docs/tidy-up.md has the taxonomy and how the ranges were found. A module may
take several rows when another one's code sits inside it.

A file holding game or gap code is `src/<folder>/<module>_<address>.cpp`,
where the address is the one the file is about (tools/sources.py's
primary_address, without 0x). A named class's file holds all its methods and
is named after the class (`src/network/bit_writer.cpp`): it keeps its name and
goes to the folder of its first method. Runtime library files go to src/runtime/ under
their own names, and data files to the folder DATA_FOLDERS gives them. The
tools find every file by its annotations (tools/sources.py), so a move never
changes a build. --move runs `git mv`, then rewrites the old paths where the
docs and the sources' comments spell them out (REFERENCES); the generated
data/progress.csv, data/layout.csv, data/globals.csv and link/globals.h
follow when tools/progress.py, place.py --write-layout and globals.py run.
"""

import argparse
import bisect
import csv
import re
import subprocess
import sys
from collections import Counter
from functools import lru_cache
from pathlib import Path

from sources import NAME_ADDRESS, ROOT, SRC, kind_of, primary_address, relative, source_files

MODULES = ROOT / "data/modules.csv"
# The text that names source files by path: rewritten when a file moves.
REFERENCES = ["AGENTS.md", "CONTRIBUTING.md", "README.md", "docs/*.md", ".opencode/**/*.md", "src/**/*.cpp"]
PATH = re.compile(rb"src/[\w/]+\.cpp")
# The data files (tools/sources.py's `data` kind) by subject. The ones that
# serve the whole game, or no code yet, stay in src/data/.
DATA_FOLDERS = {
    "ballistics": "weapons",
    "console_commands": "game",
    "debug_dialogs": "debug",
    "perf_counters": "debug",
    "unit_messages": "units",
    "unit_orders": "orders",
}


@lru_cache(maxsize=1)
def table() -> tuple[list[int], list[dict]]:
    with MODULES.open() as fh:
        rows = list(csv.DictReader(fh))
    starts = [int(r["start"], 16) for r in rows]
    if starts != sorted(starts):
        raise SystemExit(f"{MODULES.relative_to(ROOT)}: the rows are not in address order")
    return starts, rows


def module_of(address: int) -> dict | None:
    """The data/modules.csv row an address falls in."""
    starts, rows = table()
    i = bisect.bisect_right(starts, address) - 1
    return rows[i] if i >= 0 else None


def place(src: Path) -> Path:
    """Where a source file belongs."""
    kind = kind_of(src)
    if kind == "data":
        return SRC / DATA_FOLDERS.get(src.stem, "data") / src.name
    if kind == "library":
        return SRC / "runtime" / src.name
    address = primary_address(src)
    row = module_of(address)
    if row is None:
        return src
    if not NAME_ADDRESS.search(src.stem):
        return SRC / row["folder"] / src.name
    return SRC / row["folder"] / f"{row['module']}_{address:06x}.cpp"


def rewrite_references(moved: dict[str, str]) -> int:
    """Replace each moved file's old path with its new one in REFERENCES
    (byte for byte otherwise: line ends and encodings are kept)."""
    table = {old.encode(): new.encode() for old, new in moved.items()}
    changed = 0
    for pattern in REFERENCES:
        for f in sorted(ROOT.glob(pattern)):
            data = f.read_bytes()
            new = PATH.sub(lambda m: table.get(m.group(0), m.group(0)), data)
            if new != data:
                f.write_bytes(new)
                changed += 1
    return changed


def misplaced() -> list[tuple[Path, Path]]:
    return [(s, place(s)) for s in source_files() if place(s).resolve() != s.resolve()]


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", nargs="*", type=lambda s: int(s, 16))
    ap.add_argument("--summary", action="store_true", help="files per folder and module")
    ap.add_argument("--check", action="store_true", help="list the files that are not in their place")
    ap.add_argument("--move", action="store_true", help="git mv the files that are not in their place")
    ap.add_argument("--folder", action="append", default=[], help="with --move: only into this folder")
    ap.add_argument("--dry-run", action="store_true", help="with --move: print the moves only")
    args = ap.parse_args()

    for a in args.address:
        row = module_of(a)
        if row is None:
            print(f"{a:#x}: before the first module")
            continue
        print(f"{a:#x}: src/{row['folder']}/{row['module']}_{a:06x}.cpp (module {row['folder']}/{row['module']}, "
              f"from {row['start']}: {row['evidence']})")
    if args.summary:
        folders, modules = Counter(), Counter()
        for s in source_files():
            p = place(s)
            folders[p.parent.name] += 1
            row = module_of(primary_address(s)) if kind_of(s) in ("game", "gap") else None
            modules[(p.parent.name, row["module"] if row else p.stem)] += 1
        for folder, n in sorted(folders.items(), key=lambda kv: -kv[1]):
            mods = sorted(((m, c) for (f, m), c in modules.items() if f == folder), key=lambda kv: -kv[1])
            print(f"{folder:10s} {n:5d}  " + ", ".join(f"{m} {c}" for m, c in mods))
    if args.check or args.move:
        # tools/link.py and carve.py keep objects by file name, so a name may
        # be used once under src/.
        names = Counter(s.name for s in source_files())
        same = sorted(n for n, c in names.items() if c > 1)
        if same:
            raise SystemExit("file names used twice under src/: " + ", ".join(same[:10]))
        moves = misplaced()
        if args.folder:
            moves = [(s, d) for s, d in moves if d.parent.name in args.folder]
        clash = Counter(d for _, d in moves)
        dup = [d for d, n in clash.items() if n > 1 or (d.exists() and all(d.resolve() != s.resolve() for s, _ in moves))]
        if dup:
            raise SystemExit("two files would get the same name: " + ", ".join(relative(d) for d in dup[:10]))
        for s, d in moves:
            print(f"{relative(s)} -> {relative(d)}")
        if args.move and not args.dry_run:
            for s, d in moves:
                d.parent.mkdir(parents=True, exist_ok=True)
                subprocess.run(["git", "mv", relative(s), relative(d)], cwd=ROOT, check=True)
            n = rewrite_references({relative(s): relative(d) for s, d in moves})
            print(f"moved {len(moves)} file(s); rewrote their paths in {n} file(s)")
        elif args.check:
            print(f"{len(moves)} file(s) not where data/modules.csv puts them")
            if moves:
                sys.exit(1)


if __name__ == "__main__":
    main()
