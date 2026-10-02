# /// script
# requires-python = ">=3.11"
# dependencies = [
#     "capstone>=5",
#     "pefile>=2023.2.7",
#     "tree-sitter>=0.23",
#     "tree-sitter-cpp>=0.23",
# ]
# ///
"""Sort a stuck function into a repair route from the diff alone (issue #4841, suggestion 2).

    uv run tools/classify.py 0x4ba000 0x45f8c0
    uv run tools/classify.py --band 90 99
    uv run tools/classify.py --band 90 99 --csv out.csv

No model is involved: every signal comes out of the two disassemblies that
check.py already builds. The three routes are the issue's:

  frame    the prologue or the frame differs (pushes, [esp+N] slots, saved regs)
           -> docs/splitting-huge-functions.md, the skeleton fix
  slots    every difference is the same instruction with another register or
           another stack slot (permute.py cost 5) -> the permuter, regalloc
  shape    something else: an operand, a moved instruction, an instruction only
           one side has (cost 10, 60, 100) -> a strong model, wrong type or
           missing statement

`--band A B` scores every partial in data/progress.csv between A% and B% and
prints the bucket counts, which is how the split was checked against the tail.
"""

from __future__ import annotations

import argparse
import csv
import difflib
import json
import sys
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))

from check import (DEFAULT_FLAGS, PADDING, Original, annotations, compile_source,  # noqa: E402
                   disasm, find_source, link_placeholders, load_symbols, normalise, select_function)
from coff import parse_object  # noqa: E402
from permute import BRANCH, REG, STACK, instruction_penalty  # noqa: E402

# How many instructions count as the frame: MSVC 5's prologue is push/mov/sub,
# and anything after the first compare or call into the body is not the frame.
PROLOGUE = 6
KIND = {5: "reg", 5.1: "stack", 10: "operand", 60: "move", 100: "insdel"}


def texts(orig, obj, address: int, qualname, symbols) -> tuple[list[str], list[str], int]:
    """(theirs, ours, ours_size) as normalised instruction text, the way
    permute.py's Scorer builds them (linker fields replaced by a placeholder)."""
    picked, err = select_function(obj, None, qualname)
    if not picked:
        raise SystemExit(err)
    _, sec, start, end = picked
    data, mask = sec.data[start:end], sec.mask()[start:end]
    while data and data[-1] in PADDING and mask[-1]:
        data, mask = data[:-1], mask[:-1]
    size = orig.sizes.get(address, len(data))
    theirs = orig.read(address, size)
    lo, hi = address, address + len(theirs)
    in_image = lambda v: orig.base <= v < orig.end  # noqa: E731
    shown = disasm(link_placeholders(orig, sec, start, end, data, address, size), address)
    return ([normalise(i, lo, hi, in_image) for i in disasm(theirs, address)],
            [normalise(i, lo, hi, in_image) for i in shown], len(data))


def classify(theirs: list[str], ours: list[str]) -> dict:
    """Bucket the difference: which instructions differ, how, and where the
    first one is. Instruction-level categories come from permute.py's penalties."""
    ops = difflib.SequenceMatcher(None, theirs, ours, autojunk=False).get_opcodes()
    kinds: Counter = Counter()
    pairs: list[tuple[int, int]] = []          # (index in ours, penalty) for paired lines
    diffs: list[tuple[str, str, int]] = []     # (theirs, ours, penalty), aligned pairs only
    first = None
    for tag, i1, i2, j1, j2 in ops:
        if tag == "equal":
            continue
        a, b = theirs[i1:i2], ours[j1:j2]
        ma = [t.split(" ", 1)[0] for t in a]
        mb = [t.split(" ", 1)[0] for t in b]
        for t2, x1, x2, y1, y2 in difflib.SequenceMatcher(None, ma, mb, autojunk=False).get_opcodes():
            if t2 == "equal":
                for k, (x, y) in enumerate(zip(a[x1:x2], b[y1:y2])):
                    p = 1 if x == y else instruction_penalty(x, y)
                    kinds["same" if x == y else _kind(p)] += 1
                    if first is None:
                        first = j1 + y1 + k
                    pairs.append((j1 + y1 + k, p))
                    if x != y:
                        diffs.append((x, y, p))
            else:
                kinds["insdel"] += x2 - x1 + y2 - y1
                if first is None:
                    first = j1 + y1
    if first is None:
        return {"bucket": "match", "kinds": {}, "first": None, "differing": 0, "pairs": pairs,
                "diffs": diffs}
    shape_kinds = {"same", "reg", "stack"}
    only_slots = all(k in shape_kinds for k in kinds)
    frame = first < PROLOGUE and any(
        p > 1 and ("[esp" in ours[i] or "[ebp" in ours[i] or ours[i].startswith("push"))
        for i, p in pairs if i >= first)
    bucket = "frame" if frame else ("slots" if only_slots else "shape")
    return {"bucket": bucket, "kinds": dict(kinds), "first": first, "differing": sum(kinds.values()),
            "pairs": pairs, "diffs": diffs}


def _kind(p: int) -> str:
    if p == 1:
        return "same"
    if p == 60:
        return "move"
    if p == 100:
        return "insdel"
    bx = BRANCH.match("")  # unused, keeps the import honest for linters
    return {5: "reg", 10: "operand"}.get(p, "reg" if REG.search("x") else "operand")


def score_address(address: int) -> dict | None:
    src = find_source(address)
    if src is None:
        return None
    qualname = next((q for a, q in annotations(src) if a == address), None)
    obj_path, log = compile_source(src, out_dir=f"classify/{address:#x}")
    if obj_path is None:
        return {"address": address, "bucket": "error", "note": log.strip().splitlines()[-1][:80]}
    orig = Original()
    obj = parse_object(obj_path.read_bytes(), obj_path.name)
    theirs, ours, size = texts(orig, obj, address, qualname, load_symbols())
    res = __import__("check").compare(orig, obj, address, None, qualname, load_symbols())
    info = classify(theirs, ours)
    info.update(address=address, file=src.name, ratio=res.ratio, shape_ratio=res.shape_ratio,
                target_only=res.target_only, size=res.size, ours_size=size, bytes_match=res.bytes_match,
                note="")
    return info


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("addresses", nargs="*", type=lambda s: int(s, 16))
    ap.add_argument("--band", nargs=2, type=float, metavar=("LOW", "HIGH"))
    ap.add_argument("--csv", type=Path)
    args = ap.parse_args()

    rows = []
    if args.band:
        lo, hi = args.band
        for r in csv.DictReader((REPO / "data/progress.csv").open()):
            if r["status"] == "matched":
                continue
            s = float(r["similarity"])
            if lo <= s < hi:
                rows.append(score_address(int(r["address"], 16)))
    else:
        for a in args.addresses:
            rows.append(score_address(a))
    rows = [r for r in rows if r]
    counts = Counter(r["bucket"] for r in rows)
    print(f"{'address':>10} {'ratio':>7} {'shape':>7} {'tgt':>4} {'bytes':>11} {'first':>6}  bucket  kinds")
    for r in sorted(rows, key=lambda r: -r.get("ratio", 0)):
        kinds = " ".join(f"{k}:{v}" for k, v in sorted(r.get("kinds", {}).items(), key=lambda kv: -kv[1]))
        print(f"{r['address']:#10x} {r.get('ratio', 0) * 100:6.1f}% {r.get('shape_ratio', 0) * 100:6.1f}% "
              f"{r.get('target_only', 0):4d} {r.get('size', 0):5d}/{r.get('ours_size', 0):<5d} "
              f"{str(r.get('first')):>6}  {r['bucket']:6s}  {kinds}"
              + (f"  [{r['note']}]" if r.get("note") else ""))
    print(f"\n{len(rows)} functions: " + ", ".join(f"{k} {v}" for k, v in sorted(counts.items())))
    if args.csv:
        with args.csv.open("w", newline="") as fh:
            w = csv.DictWriter(fh, fieldnames=["address", "file", "bucket", "ratio", "shape_ratio",
                                               "target_only", "size", "ours_size", "first", "kinds"])
            w.writeheader()
            for r in rows:
                w.writerow({**r, "kinds": json.dumps(r.get("kinds", {}))})
        print(f"wrote {args.csv}")


if __name__ == "__main__":
    main()