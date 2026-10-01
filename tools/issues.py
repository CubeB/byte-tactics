"""Open GitHub issues that hand out batches of functions to agents (orchestrator only).

    uv run tools/issues.py --band medium --count 6            # six issues of 65-160 byte functions
    uv run tools/issues.py --band large --count 4 --per 6     # four issues of six 161-400 byte functions
    uv run tools/issues.py --addresses 0x419400 0x4223e0 --title "Near-misses" --label near-miss
    uv run tools/issues.py --band medium --count 2 --dry-run  # print what would be opened

Each issue lists neighbouring functions (address order, so related code stays
together) and is labelled `decomp` plus a size label. Huge functions (over 1000
bytes) and escalations are also labelled `hard`, which only marks their size;
every model may take any issue (see AGENTS.md). Its functions are written
to data/attempts.csv as `assigned` to batch `#<issue>` so they are not handed
out twice; tools/record.py fills in the results after the pull request is
merged. Agents find and claim issues as described in AGENTS.md.
"""

import argparse
import csv
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BANDS = {"small": (1, 64), "medium": (65, 160), "large": (161, 400), "xl": (401, 600),
         "xxl": (601, 1000), "huge": (1001, 1 << 30)}
FIELDS = ["address", "size", "model", "batch", "result", "similarity", "runs", "notes"]


def gh(*args: str) -> str:
    return subprocess.run(["gh", *args], cwd=ROOT, check=True, capture_output=True, text=True).stdout


def taken() -> set[int]:
    """Functions already annotated under src/, recorded in data/attempts.csv, or listed in an open issue."""
    out = set()
    for name in ("data/progress.csv", "data/attempts.csv"):
        path = ROOT / name
        if path.exists():
            with path.open() as fh:
                out |= {int(r["address"], 16) for r in csv.DictReader(fh)}
    for issue in json.loads(gh("issue", "list", "--label", "decomp", "--state", "open", "--limit", "1000",
                                "--json", "body")):
        out |= {int(a, 16) for a in re.findall(r"`(0x[0-9a-f]+)`", issue["body"])}
    return out


def band_of(size: int) -> str:
    return next(name for name, (lo, hi) in BANDS.items() if lo <= size <= hi)


def body_for(funcs: list[tuple[int, int]], note: str) -> str:
    lines = [
        "Decompile these functions to byte-identical C++. Follow `AGENTS.md` (claim the issue first) and "
        "`docs/agent-guide.md`.",
        "",
        *[f"- [ ] `{a:#x}` ({s} bytes)" for a, s in funcs],
    ]
    if note:
        lines += ["", note]
    return "\n".join(lines) + "\n"


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--band", choices=BANDS)
    ap.add_argument("--count", type=int, default=1, help="number of issues to open")
    ap.add_argument("--per", type=int, help="functions per issue (default: 12 small/medium, 6 large, 4 xl, 3 xxl, 3 huge)")
    ap.add_argument("--addresses", nargs="+", help="exact functions for one issue (e.g. escalations)")
    ap.add_argument("--title", help="title for an --addresses issue")
    ap.add_argument("--label", action="append", default=[], help="extra label (repeatable)")
    ap.add_argument("--note", default="", help="text appended to the issue body")
    ap.add_argument("--escalation", action="store_true",
                    help="these functions were already tried; record the new attempt as an escalation")
    ap.add_argument("--open", action="store_true",
                    help="never add `hard`: a retry any model may take (a weak model's leftovers)")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    with (ROOT / "data/functions.csv").open() as fh:
        game = {int(r["address"], 16): int(r["size"]) for r in csv.DictReader(fh) if r["kind"] == "game"}

    groups: list[tuple[str, list[tuple[int, int]], list[str]]] = []
    if args.addresses:
        funcs = [(int(a, 16), game[int(a, 16)]) for a in args.addresses]
        title = args.title or f"Decomp: {len(funcs)} functions"
        groups.append((title, funcs, sorted({band_of(s) for _, s in funcs})))
    else:
        if not args.band:
            sys.exit("give --band or --addresses")
        lo, hi = BANDS[args.band]
        per = args.per or {"small": 12, "medium": 12, "large": 6, "xl": 4, "xxl": 3, "huge": 3}[args.band]
        busy = taken()
        todo = sorted((a, s) for a, s in game.items() if lo <= s <= hi and a not in busy)
        for i in range(args.count):
            funcs = todo[i * per:(i + 1) * per]
            if not funcs:
                break
            title = (f"Decomp: {len(funcs)} functions of {lo}-{hi if hi < 1 << 30 else 'more'} bytes, "
                     f"{funcs[0][0]:#x} to {funcs[-1][0]:#x}")
            groups.append((title, funcs, [args.band]))

    for title, funcs, bands in groups:
        labels = ["decomp", *[f"size:{b}" for b in bands], *args.label]
        # `hard` marks the biggest functions and escalations (their size only;
        # nobody is excluded). Issues labelled `claude` are the orchestrator's own clean-up.
        if ("claude" not in labels and not args.open
                and (args.escalation or "near-miss" in labels or "huge" in bands)):
            labels.append("hard")
        if args.dry_run:
            print(f"{title}  [{', '.join(labels)}]")
            continue
        for label in labels:
            color = {"decomp": "5319e7", "hard": "b60205", "claude": "d97757"}.get(label, "c5def5")
            gh("label", "create", label, "--force", "--color", color)
        url = gh("issue", "create", "--title", title, "--body", body_for(funcs, args.note),
                 *[x for label in labels for x in ("--label", label)]).strip()
        number = url.rsplit("/", 1)[-1]
        path = ROOT / "data/attempts.csv"
        with path.open("a", newline="") as fh:
            w = csv.writer(fh, lineterminator="\n")
            for a, s in funcs:
                note = f"issue #{number}" + (", escalated" if args.escalation else "")
                w.writerow([f"{a:#x}", s, "", f"#{number}", "assigned", "", "", note])
        print(url)


if __name__ == "__main__":
    main()
