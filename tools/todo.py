"""List game functions nobody has attempted yet, smallest first.

    uv run tools/todo.py --leaf --min-size 17 --max-size 64 --limit 10

"Attempted" means annotated somewhere under src/ (data/progress.csv) or listed
in data/attempts.csv.
"""

import argparse
import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def addresses(path: Path) -> set[int]:
    if not path.exists():
        return set()
    with path.open() as fh:
        return {int(r["address"], 16) for r in csv.DictReader(fh)}


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--min-size", type=int, default=0)
    ap.add_argument("--max-size", type=int, default=1 << 30)
    ap.add_argument("--leaf", action="store_true", help="only functions that call no other game function")
    ap.add_argument("--seh", choices=["yes", "no"], help="only functions with / without a C++ exception frame")
    ap.add_argument("--limit", type=int, default=20)
    args = ap.parse_args()

    done = addresses(ROOT / "data/progress.csv") | addresses(ROOT / "data/attempts.csv")
    with (ROOT / "data/functions.csv").open() as fh:
        rows = [r for r in csv.DictReader(fh) if r["kind"] == "game"]
    picked = [
        r for r in rows
        if int(r["address"], 16) not in done
        and args.min_size <= int(r["size"]) <= args.max_size
        and (not args.leaf or r["game_calls"] == "0")
        and (args.seh is None or r["seh"] == ("1" if args.seh == "yes" else "0"))
    ]
    picked.sort(key=lambda r: (int(r["size"]), int(r["address"], 16)))
    for r in picked[:args.limit]:
        print(f"{r['address']} {r['size']}")


if __name__ == "__main__":
    main()
