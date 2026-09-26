"""Summarise agent results into docs/agents.md (between the calibration markers).

Reads data/attempts.csv (one row per function per attempt, re-verified by the
orchestrator) and data/batches.csv (token and time cost per batch).
"""

import csv
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DOC = ROOT / "docs/agents.md"
START, END = "<!-- calibration:start -->", "<!-- calibration:end -->"
# Relative price per token (input and output scale together): Haiku 4.5 $1/$5,
# Sonnet 5 $2/$10, Opus 5.5 $4/$20 per million tokens.
PRICE = {"haiku": 1, "sonnet": 2, "opus": 4}
BANDS = [(1, 16), (17, 40), (41, 64), (65, 160), (161, 400), (401, 1 << 30)]


def band(size: int) -> str:
    lo, hi = next(b for b in BANDS if b[0] <= size <= b[1])
    return f"{lo}+" if hi >= 1 << 30 else f"{lo}-{hi}"


def main() -> None:
    with (ROOT / "data/attempts.csv").open() as fh:
        attempts = [r for r in csv.DictReader(fh) if r["result"] not in ("assigned", "")]
    batches = []
    if (ROOT / "data/batches.csv").exists():
        with (ROOT / "data/batches.csv").open() as fh:
            batches = list(csv.DictReader(fh))

    # First attempts only (escalations are reported separately).
    first = [r for r in attempts if "escalated" not in r.get("notes", "")]
    grid = defaultdict(lambda: [0, 0])
    for r in first:
        g = grid[(r["model"], band(int(r["size"])))]
        g[0] += 1
        g[1] += r["result"] == "matched"
    models = sorted({m for m, _ in grid})
    lines = ["| Size (bytes) | " + " | ".join(m.capitalize() for m in models) + " |",
             "| --- |" + " ---: |" * len(models)]
    for lo, hi in BANDS:
        b = band(lo)
        cells = []
        for m in models:
            n, ok = grid.get((m, b), (0, 0))
            cells.append(f"{ok}/{n} ({100 * ok / n:.0f}%)" if n else "")
        if any(cells):
            lines.append(f"| {b} | " + " | ".join(cells) + " |")

    cost = ["| Batch | Model | Functions | Matched | Tokens | Tokens per match | Cost units per match | Minutes |",
            "| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: |"]
    for b in batches:
        n, ok, tok = int(b["functions"]), int(b["matched"]), int(b["tokens"])
        per = f"{tok // ok:,}" if ok else "n/a"
        units = f"{tok * PRICE.get(b['model'], 1) / ok / 1000:.0f}" if ok else "n/a"
        cost.append(f"| {b['batch']} | {b['model']} | {n} | {ok} | {tok:,} | {per} | {units} | {int(b['seconds']) / 60:.0f} |")

    esc = defaultdict(lambda: [0, 0])
    for r in attempts:
        if "escalated" in r.get("notes", ""):
            e = esc[r["model"]]
            e[0] += 1
            e[1] += r["result"] == "matched"
    esc_lines = [f"- {m.capitalize()} matched {ok} of {n} functions a cheaper model had failed." for m, (n, ok) in esc.items()]

    section = "\n".join([START, "### First-attempt match rate by function size", "", *lines, "",
                         "### Cost per batch", "",
                         "Cost units: thousands of tokens weighted by price relative to Haiku "
                         "(Sonnet 5 costs 2x per token, Opus 5.5 4x). Token counts are the "
                         "harness's totals per agent.", "", *cost, "",
                         "### Escalations", "", *(esc_lines or ["- none yet"]), END])
    text = DOC.read_text()
    if START in text:
        text = text[:text.index(START)] + section + text[text.index(END) + len(END):]
    else:
        text = text.replace("(filled in as batches complete)", section)
    DOC.write_text(text)
    print(section)


if __name__ == "__main__":
    main()
