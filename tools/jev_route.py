# /// script
# requires-python = ">=3.11"
# dependencies = [
#     "capstone>=5",
#     "pefile>=2023.2.7",
#     "tree-sitter>=0.23",
#     "tree-sitter-cpp>=0.23",
# ]
# ///
"""Route stuck functions with Jev, the typed classifier of issue #4841 suggestion 2.

    uv run tools/jev_route.py 0x4ba000 0x45f8c0
    uv run tools/jev_route.py --band 90 99
    uv run tools/jev_route.py --band 90 99 --csv routes.csv

The API key comes from OPENROUTER_API_KEY (kept in .envrc, which is in
.git/info/exclude, never in the tree). The model is `~typesafe/jev-latest` on
OpenRouter's /api/alpha/decisions endpoint.

The state is a diff summary, not the conversation: the source's function body,
our size against the original's, the differing instruction pairs, and the
histogram of how they differ (same instruction other register = 5, other stack
slot = 5, other operands = 10, moved = 60, only one side = 100; these are
permute.py's costs). Three typed questions come back: the route,
how much understanding of the source a fix needs, and the size of the fix. Confidence comes with every answer, which is
what the "fall back when confidence is low" rule in the issue needs.

Costs about $0.0001 a function, about 0.5 s a call.
"""

from __future__ import annotations

import argparse
import csv
import json
import os
import re
import sys
import urllib.request
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))
sys.path.insert(0, str(HERE))

from classify import PROLOGUE, classify, texts  # noqa: E402
from check import (Original, annotations, compare, compile_source, find_source, load_symbols)  # noqa: E402
from coff import parse_object  # noqa: E402

ENDPOINT = "https://openrouter.ai/api/alpha/decisions"
MODEL = "~typesafe/jev-latest"
KEY = os.environ.get("OPENROUTER_API_KEY", "")

QUESTIONS = {
    "route": {
        "type": "choice",
        "instructions": (
            "Which repair route fits this function? Judge only from the state: the diff "
            "summary and the histogram of how the instructions differ."
        ),
        "criteria": {
            "regalloc": (
                "The instruction sequence is right and only the registers, the stack slots or "
                "the operand order differ (histogram almost all 'reg', 'stack' or small counts "
                "with no 'move' and no 'insdel'). A mechanical meaning-preserving rewrite search "
                "over the source has a real chance."
            ),
            "semantic": (
                "The code does something different: instructions are missing, in a different "
                "order, or the histogram has 'move' or 'insdel'. A type, a struct field, a "
                "callee, a missing statement or a wrong loop shape is wrong and has to be "
                "re-thought by a strong model."
            ),
            "frame": (
                "The prologue or the frame is wrong: the first difference is in the first few "
                "instructions, or pushes, saved registers or [esp+N] slots differ."
            ),
            "compiler_state": (
                "The source is almost certainly already right and the residual is a scheduler or "
                "inliner tie no spelling of the source can reach: one instruction with the same "
                "operands in a different order, or a single byte such as a SIB base swap."
            ),
        },
    },
    "semantic_depth": {
        "type": "score",
        "instructions": "How much does a fix need to understand what the function computes?",
        "criteria": ["Only the spelling of the existing statements (order, registers, slots)",
                     "A local rewrite, e.g. one statement or one declaration",
                     "The function's structure, e.g. a loop shape or a split of a block",
                     "The meaning of the code, e.g. a wrong type, field or callee"],
    },
    "fix_size": {
        "type": "score",
        "instructions": "How large a change to the source does the fix need?",
        "criteria": ["One line or one declaration",
                     "A few lines in one place",
                     "Several places in one function",
                     "A restructure of the function, or the wrong function entirely"],
    },
}

# Which route each question's answer should agree with, for the mechanical cross-check.
AGREE = {"regalloc": "slots", "semantic": "shape", "frame": "frame", "compiler_state": "slots"}


def state_of(address: int) -> dict | None:
    """Everything Jev gets to see, and the mechanical bucket to check it against."""
    src = find_source(address)
    if src is None:
        return None
    qualname = next((q for a, q in annotations(src) if a == address), None)
    obj_path, log = compile_source(src, out_dir=f"jev/{address:#x}")
    if obj_path is None:
        return {"address": address, "error": log.strip().splitlines()[-1][:100]}
    orig = Original()
    symbols = load_symbols()
    obj = parse_object(obj_path.read_bytes(), obj_path.name)
    theirs, ours, ours_size = texts(orig, obj, address, qualname, symbols)
    res = compare(orig, obj, address, None, qualname, symbols)
    info = classify(theirs, ours)

    # The differing instruction pairs, as the agent would read them.
    pairs = [f"{y}   <-   {x}" for x, y, p in info["diffs"] if x != y][:40]
    body = re.sub(r"//.*", "", "\n".join(src.read_text(errors="replace").splitlines()))
    body = "\n".join(l for l in body.splitlines() if l.strip())[:4000]
    first = info["first"]
    head = "\n".join(f"  {i:3d} {' '.join(ours[i].split()[:8])}" for i in range(min(len(ours), PROLOGUE)))
    state = "\n".join([
        f"function: {address:#x} {qualname or ''}".rstrip(),
        f"size: ours {ours_size} bytes, original {res.size} bytes",
        f"check.py ratio {res.ratio * 100:.1f}%, shape ratio {res.shape_ratio * 100:.1f}%, "
        f"{res.target_only} of the diff lines are only jump targets that moved",
        "first difference at instruction {first}; differences by kind: "
        + ", ".join(f"{k} {v}" for k, v in sorted(info["kinds"].items(), key=lambda kv: -kv[1])),
        f"our prologue:\n{head}",
        "differing instructions (ours <- original):\n  " + "\n  ".join(pairs),
        f"our source:\n{body}",
    ])
    return {"address": address, "file": src.name, "state": state, "mech": info["bucket"],
            "ratio": res.ratio, "shape_ratio": res.shape_ratio, "kinds": info["kinds"],
            "first": first, "size": res.size, "ours_size": ours_size}


def ask(entry: dict) -> dict:
    """One Jev call: the typed questions, plus the mechanical bucket for comparison."""
    payload = json.dumps({"model": MODEL, "state": entry["state"], "questions": QUESTIONS}).encode()
    req = urllib.request.Request(ENDPOINT, data=payload, headers={
        "Content-Type": "application/json", "Authorization": f"Bearer {KEY}"})
    try:
        with urllib.request.urlopen(req, timeout=120) as fh:
            out = json.load(fh)
    except Exception as exc:
        return {**entry, "error": repr(exc)[:200]}
    a = out.get("answers", {})
    route = a.get("route", {})
    choice = route.get("choice", "")
    probs = route.get("probabilities", {})
    top = max(probs.values()) if probs else 0.0
    entry.update(
        route=choice,
        route_probs=probs,
        # The API's own confidence, and the top probability, are two different
        # thresholds the issue's "fall back when confidence is low" rule can use.
        confidence=route.get("confidence"),
        top_prob=top,
        semantic_depth=a.get("semantic_depth", {}).get("score"),
        fix_size=a.get("fix_size", {}).get("score"),
        model=out.get("model"),
        cost=out.get("usage", {}).get("cost"),
        input_tokens=out.get("usage", {}).get("input_tokens"),
        mech_agrees=(AGREE.get(choice) == entry["mech"]),
    )
    return entry


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("addresses", nargs="*", type=lambda s: int(s, 16))
    ap.add_argument("--band", nargs=2, type=float, metavar=("LOW", "HIGH"))
    ap.add_argument("--jobs", type=int, default=8)
    ap.add_argument("--csv", type=Path)
    ap.add_argument("--threshold", type=float, default=0.65,
                    help="below this top probability the caller must fall back to an LLM")
    args = ap.parse_args()
    if not KEY:
        sys.exit("OPENROUTER_API_KEY is not set (source .envrc in the repo root)")

    addrs = list(args.addresses)
    if args.band:
        lo, hi = args.band
        addrs = [int(r["address"], 16) for r in csv.DictReader((REPO / "data/progress.csv").open())
                 if r["status"] != "matched" and lo <= float(r["similarity"]) < hi]
    if not addrs:
        sys.exit("nothing to route")
    states = [s for s in (state_of(a) for a in addrs) if s]
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        rows = list(pool.map(ask, states))
    rows.sort(key=lambda r: -r.get("ratio", 0))

    print(f"{'address':>10} {'ratio':>7} {'route':>16} {'p':>5} {'conf':>5} "
          f"{'depth':>5} {'size':>4} {'mech':>6} ok")
    low = 0
    for r in rows:
        if r.get("error"):
            print(f"{r['address']:#10x}  ERROR {r['error']}")
            continue
        agree = "=" if r["mech_agrees"] else "x"
        if r["top_prob"] < args.threshold:
            low += 1
        print(f"{r['address']:#10x} {r['ratio'] * 100:6.1f}% {r['route']:>16} {r['top_prob']:5.2f} "
              f"{r['confidence']:5.2f} {r['semantic_depth']:5.0f} "
              f"{r['fix_size']:4.0f} {r['mech']:>6} {agree}")
    counts = Counter(r.get("route") for r in rows if not r.get("error"))
    print(f"\n{len(rows)} functions: " + ", ".join(f"{k} {v}" for k, v in counts.most_common()))
    agree = sum(1 for r in rows if r.get("mech_agrees"))
    print(f"agrees with the mechanical bucket on {agree}/{len(rows)}, "
          f"{low} below the {args.threshold} confidence threshold (fall back to an LLM)")
    print(f"cost so far ${sum(r.get('cost') or 0 for r in rows):.4f} in "
          f"{sum(r.get('input_tokens') or 0 for r in rows)} input tokens")
    if args.csv:
        with args.csv.open("w", newline="") as fh:
            cols = ["address", "file", "ratio", "shape_ratio", "first", "size", "ours_size", "mech",
                    "route", "route_probs", "confidence", "top_prob",
                    "semantic_depth", "fix_size", "mech_agrees", "kinds"]
            w = csv.DictWriter(fh, fieldnames=cols, extrasaction="ignore")
            w.writeheader()
            for r in rows:
                w.writerow({**r, "kinds": json.dumps(r.get("kinds", {})), "route_probs": json.dumps(r.get("route_probs", {}))})
        print(f"wrote {args.csv}")


if __name__ == "__main__":
    main()