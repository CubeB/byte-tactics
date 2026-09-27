"""Verify an issue's functions after its pull request is merged, and record the results (orchestrator only).

    uv run tools/record.py 12 gpt-6-astra
    uv run tools/record.py 12 glm-5.3 --model-for 0x401000=deepseek-v4.1-flash --escalate
    uv run tools/record.py 12 opus --tokens 140000 --seconds 600

Re-checks every function of issue #12 (the rows `tools/issues.py` wrote to
data/attempts.csv as batch `#12`) against the checked-out tree, fills in the
model and result, and appends a row to data/batches.csv for
tools/calibration.py. Agents' own claims are never trusted: this is the check
that counts.

`--model-for` records a different model for single functions (a subagent that
wrote them, per the pull request table). `--escalate` opens a `hard` issue for
every function left unmatched, so a stronger model retries it.
"""

import argparse
import csv
import subprocess

from check import Original, annotations, compare, compile_source, find_source, load_symbols
from coff import parse_object
from progress import ROOT


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("issue", help="issue number (the batch is '#<issue>')")
    ap.add_argument("model", help="model the agent used, e.g. gpt-6-astra, deepseek-v4.1-flash, opus")
    ap.add_argument("--tokens", type=int, default=0)
    ap.add_argument("--seconds", type=int, default=0)
    ap.add_argument("--model-for", action="append", default=[], metavar="ADDR=MODEL",
                    help="model that wrote one function, if not the main one (repeatable)")
    ap.add_argument("--escalate", action="store_true",
                    help="open a hard issue for the functions left unmatched")
    args = ap.parse_args()
    per_function = {int(a, 16): m for a, m in (x.split("=", 1) for x in args.model_for)}
    batch = f"#{args.issue.lstrip('#')}"

    path = ROOT / "data/attempts.csv"
    with path.open() as fh:
        rows = list(csv.DictReader(fh))
    mine = [r for r in rows if r["batch"] == batch]
    if not mine:
        raise SystemExit(f"no functions recorded for batch {batch} in data/attempts.csv")
    orig, symbols = Original(), load_symbols()
    for r in mine:
        address = int(r["address"], 16)
        r["model"] = per_function.get(address, args.model)
        src = find_source(address)
        if src is None:
            r["result"], r["similarity"] = "no file", "0"
            continue
        qualname = next((q for a, q in annotations(src) if a == address), None)
        obj, _ = compile_source(src, out_dir="record")
        if obj is None:
            r["result"], r["similarity"] = "compile error", "0"
            continue
        res = compare(orig, parse_object(obj.read_bytes(), obj.name), address, qualname=qualname, symbols=symbols)
        r["result"] = "matched" if res.matched else ("error" if res.error else "partial")
        r["similarity"] = f"{res.ratio * 100:.1f}"
    with path.open("w", newline="") as fh:
        w = csv.DictWriter(fh, rows[0].keys(), lineterminator="\n")
        w.writeheader()
        w.writerows(rows)

    bpath = ROOT / "data/batches.csv"
    matched = [r for r in mine if r["result"] == "matched"]
    with bpath.open("a", newline="") as fh:
        csv.writer(fh, lineterminator="\n").writerow(
            [batch, args.model, len(mine), len(matched), sum(int(r["size"]) for r in matched),
             sum(int(r["size"]) for r in mine), args.tokens, "", args.seconds])
    for r in mine:
        print(r["address"], r["size"], r["result"], r["similarity"])
    print(f"{batch} ({args.model}): {len(matched)}/{len(mine)} matched")

    left = [r["address"] for r in mine if r["result"] != "matched"]
    if args.escalate and left:
        models = sorted({r["model"] for r in mine if r["result"] != "matched"})
        subprocess.run(["uv", "run", "--quiet", "tools/issues.py", "--addresses", *left,
                        "--title", f"Retry: {len(left)} functions left unmatched in {batch}",
                        "--label", "near-miss", "--escalation",
                        "--note", f"Tried by {', '.join(models)} in {batch}. Each file says what still "
                                  "differs; treat it as a starting point, not as correct."],
                       cwd=ROOT, check=True)


if __name__ == "__main__":
    main()
