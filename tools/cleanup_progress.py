"""Print how far the source cleanup has got, as bars against a starting point.

    uv run tools/cleanup_progress.py                  # markdown table for HEAD
    uv run tools/cleanup_progress.py --ref origin/main
    uv run tools/cleanup_progress.py --write docs/cleanup-progress.md
    uv run tools/cleanup_progress.py --readme --issues  # rewrite the README block
    uv run tools/cleanup_progress.py --issues         # add the gather/join/name issue counts (needs gh)

Each row counts something in `src/` and `include/` at a git ref and compares it
with the same count at the roadmap's starting commit (--base, default e9367f13,
"Add the source cleanup roadmap") and a target. A bar is the share of the
distance from start to target that is covered; a row whose target is not a
count (the byte offsets) has no bar. Nothing here compiles anything; the counts
come from reading the sources out of git.
"""

import argparse
import io
import json
import re
import shutil
import subprocess
import tarfile
from pathlib import Path

from trycasts import arrow_groups, code_mask  # phase 2's definition of a cast

ROOT = Path(__file__).resolve().parent.parent
BASE = "e9367f13"

PLACEHOLDERS = {
    "FUN": re.compile(r"\bFUN_[0-9a-f]{8}\b"),
    "DAT": re.compile(r"\bDAT_[0-9a-f]{8}\b"),
    "CLASS": re.compile(r"\bClass_[0-9a-f_]+\b"),
    "FIELD": re.compile(r"\bfield_[0-9a-f]+\b"),
}
UNIT_DEF = re.compile(r"^(?:struct|class) Unit\s*\{", re.M)
# The markers a matching note used (docs/cleanup-roadmap.md, phase 1): a match
# percentage, an attempt, a score, or a model name. `score` is a word
# descriptions can use ("the score reporting setup"), so it counts only next
# to its number or a comparison.
HISTORY = re.compile(
    r"\d+(?:\.\d+)?\s*(?:%|percent\b)"
    r"|\battempt"
    r"|\btried\b|\btries\b|\btrying\b"
    r"|\bscor(?:e|es|ed|ing)\b(?:\s+\S+){0,4}?\s+(?:\d|percent\b)"
    r"|\bscor(?:e|es|ed|ing)\b(?:\s+\S+){0,2}?\s+(?:worse|better|lower|higher|same|flat)\b",
    re.I,
)
MODELS = re.compile(
    r"\b(?:Opus|Sonnet|Haiku|DeepSeek|Claude|GPT|LongCat|mimo|muse|"
    r"space[- ]?bunny|Fledge|Codex)\b",
    re.I,
)
CREDIT_START = re.compile(r"\bDecompiled by\b")
CREDIT_END = re.compile(r"\bprovisional\b", re.I)
# Phase 4's input: a constant byte offset through a pointer, `*(T*)(p + 0x..)`
# and `(char*)p + off`.
OFFSET_CAST = re.compile(
    r"\*\(\s*(?:const\s+)?(?:unsigned\s+|signed\s+)?[A-Za-z_]\w*(?:\s*::\s*\w+)*\s*\*+\s*\)"
    r"\s*\([^()\n]*\+\s*(?:0x[0-9a-fA-F]+|\d+)\s*\)"
)
OFFSET_CHAR = re.compile(r"\(\s*char\s*\*+\s*\)\s*[^;()\n]*\+\s*(?:0x[0-9a-fA-F]+|\d+)")


def git(*args: str, text: bool = True):
    return subprocess.run(["git", *args], cwd=ROOT, check=True, capture_output=True, text=text).stdout


def sources(ref: str) -> dict[str, str]:
    """Every .cpp, .h and .c file under src/ and include/ at the ref."""
    raw = git("archive", ref, "src", "include", text=False)
    out = {}
    with tarfile.open(fileobj=io.BytesIO(raw)) as tar:
        for m in tar:
            if m.isfile() and m.name.endswith((".cpp", ".h", ".c")):
                out[m.name] = tar.extractfile(m).read().decode("utf-8", "replace")
    return out


def history_note(text: str) -> bool:
    """True when the file still opens with matching history: a match
    percentage, an attempt, a score or a model name in the leading comments.
    The credit line (with its `finished by` continuations) does not count."""
    in_credit = False
    for line in text.split("\n"):
        if line.strip() == "":
            continue
        if not line.startswith("//"):
            break
        if in_credit:
            if CREDIT_END.search(line):
                in_credit = False
            continue
        if CREDIT_START.search(line):
            in_credit = not CREDIT_END.search(line)
            continue
        if HISTORY.search(line) or MODELS.search(line):
            return True
    return False


def casts(text: str, mask: bytearray) -> int:
    """`((T*)x)->` occurrences in code: the casts tools/trycasts.py removes."""
    return sum(arrow_groups(text, mask).values())


def matches(text: str, rx: re.Pattern, mask: bytearray) -> int:
    """Occurrences of `rx` in code, not in comments or strings."""
    return sum(1 for m in rx.finditer(text) if mask[m.start()])


def measure(ref: str) -> dict[str, int]:
    files = sources(ref)
    cpp = {k: v for k, v in files.items() if k.startswith("src/") and k.endswith(".cpp")}
    seen = {k: set() for k in PLACEHOLDERS}
    cast_count = offset_count = 0
    for text in files.values():
        for k, rx in PLACEHOLDERS.items():
            seen[k].update(rx.findall(text))
        mask = code_mask(text)
        cast_count += casts(text, mask)
        offset_count += matches(text, OFFSET_CAST, mask) + matches(text, OFFSET_CHAR, mask)
    m = {
        "files": len(cpp),
        "notes": sum(history_note(t) for t in files.values()),
        "casts": cast_count,
        "offsets": offset_count,
        "unit_defs": sum(bool(UNIT_DEF.search(t)) for t in cpp.values()),
    }
    m.update({k.lower(): len(v) for k, v in seen.items()})
    return m


def modules(ref: str) -> int:
    return len(git("show", f"{ref}:data/modules.csv").strip().split("\n")) - 1


def bar(done: float, width: int = 20) -> str:
    n = round(done * width)
    return "[" + "#" * n + "-" * (width - n) + "]"


def issue_rows() -> list[str]:
    gh = shutil.which("gh") or shutil.which("gh.exe")
    if not gh:
        return []
    raw = subprocess.run(
        [gh, "api", "--paginate", "-X", "GET", "search/issues",
         "-f", "q=repo:HectorBailey/byte-tactics label:cleanup is:issue", "-f", "per_page=100"],
        cwd=ROOT, capture_output=True, text=True)
    if raw.returncode:
        return []
    kinds = {"gather": "Gather", "join": "Join", "name": "Name"}
    rows = []
    dec, pos, items = json.JSONDecoder(), 0, []
    while pos < len(raw.stdout.rstrip()):
        page, pos = dec.raw_decode(raw.stdout, pos)
        items += page["items"]
        while raw.stdout[pos:pos + 1].isspace():
            pos += 1
    for key, label in kinds.items():
        mine = [i for i in items if i["title"].lower().startswith(f"clean-up: {key}")]
        if not mine:
            continue
        closed = sum(i["state"] == "closed" for i in mine)
        rows.append(f"| {label} issues | {closed} of {len(mine)} closed | `{bar(closed / len(mine))}` | {100 * closed // len(mine)}% |")
    return rows


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ref", default="HEAD")
    ap.add_argument("--base", default=BASE)
    ap.add_argument("--write", metavar="FILE")
    ap.add_argument("--readme", action="store_true", help="rewrite the block between the cleanup markers in README.md")
    ap.add_argument("--issues", action="store_true")
    args = ap.parse_args()

    now, then = measure(args.ref), measure(args.base)
    target_files = modules(args.ref)
    rows = [
        ("Source files (`.cpp`)", "files", target_files, "one per module (`data/modules.csv`)"),
        ("Placeholder functions `FUN_<addr>`", "fun", 0, "named"),
        ("Placeholder globals `DAT_<addr>`", "dat", 0, "named"),
        ("Placeholder classes `Class_<addr>`", "class", 0, "named"),
        ("Placeholder fields `field_<offset>`", "field", 0, "named"),
        ("Files that define `Unit`", "unit_defs", 1, "one shared definition"),
        ("Files opening with matching history", "notes", 0, "none"),
        ("Casts `((T*)x)->`", "casts", 0, "casts the types allow"),
        ("Byte-offset access `*(T*)(p + off)` and `(char*)p + off`", "offsets", None, "cases with no struct"),
    ]
    lines = [
        f"Counts in `src/` and `include/` at `{args.ref}`, against `{args.base}` (the roadmap's starting point).",
        "",
        "| | Start | Now | Target | Done | |",
        "| --- | ---: | ---: | --- | ---: | --- |",
    ]
    total = []
    for label, key, target, tgt_text in rows:
        a, b = then[key], now[key]
        if target is None:
            lines.append(f"| {label} | {a:,} | {b:,} | {tgt_text} | | |")
            continue
        span = a - target
        done = 1.0 if span <= 0 else max(0.0, min(1.0, (a - b) / span))
        total.append(done)
        shown = "0" if target == 0 else f"{target:,}"
        lines.append(f"| {label} | {a:,} | {b:,} | {shown}, {tgt_text} | {100 * done:.0f}% | `{bar(done)}` |")
    overall = sum(total) / len(total)
    head = [f"**Readability cleanup: about {100 * overall:.0f}% of the way** (mean of the rows with a target)", "",
            f"`{bar(overall, 40)}`", ""]
    out = head + lines
    if args.issues:
        extra = issue_rows()
        if extra:
            out += ["", "| Cleanup issues | | | |", "| --- | --- | --- | ---: |"] + extra
    text = "\n".join(out) + "\n"
    if args.readme:
        path = ROOT / "README.md"
        old = path.read_text()
        start, end = "<!-- cleanup:start -->", "<!-- cleanup:end -->"
        a, b = old.index(start) + len(start), old.index(end)
        intro = "\n## Cleanup progress\n\nThe matching is done; this is how far the source has got toward reading like source.\n\n"
        path.write_text(old[:a] + intro + text + old[b:])
    elif args.write:
        Path(args.write).write_text("# Cleanup progress\n\nWritten by `tools/cleanup_progress.py`.\n\n" + text)
    else:
        print(text)


if __name__ == "__main__":
    main()
