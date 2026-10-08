"""Print how far the source cleanup has got, as bars against a starting point.

    uv run tools/cleanup_progress.py                  # markdown table for HEAD
    uv run tools/cleanup_progress.py --ref origin/main
    uv run tools/cleanup_progress.py --write docs/cleanup-progress.md
    uv run tools/cleanup_progress.py --readme --issues  # rewrite the README block
    uv run tools/cleanup_progress.py --issues         # add the gather/join/name issue counts (needs gh)

Each row counts something in `src/` and `include/` at a git ref and compares it
with the same count at the roadmap's starting commit (--base, default e9367f13,
"Add the source cleanup roadmap") and a target. A bar is the share of the
distance from start to target that is covered. Nothing here compiles anything;
the counts come from reading the sources out of git.
"""

import argparse
import io
import json
import re
import shutil
import subprocess
import tarfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BASE = "e9367f13"

PLACEHOLDERS = {
    "FUN": re.compile(r"\bFUN_[0-9a-f]{8}\b"),
    "DAT": re.compile(r"\bDAT_[0-9a-f]{8}\b"),
    "CLASS": re.compile(r"\bClass_[0-9a-f_]+\b"),
    "FIELD": re.compile(r"\bfield_[0-9a-f]+\b"),
}
UNIT_DEF = re.compile(r"^(?:struct|class) Unit\s*\{", re.M)
NOTE_LINES = 8  # a file that opens with more comment lines than this carries a matching note


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


def long_note(text: str) -> bool:
    n = 0
    for line in text.split("\n"):
        if line.startswith("//"):
            n += 1
        else:
            break
    return n > NOTE_LINES


def measure(ref: str) -> dict[str, int]:
    files = sources(ref)
    cpp = {k: v for k, v in files.items() if k.startswith("src/") and k.endswith(".cpp")}
    seen = {k: set() for k in PLACEHOLDERS}
    for text in files.values():
        for k, rx in PLACEHOLDERS.items():
            seen[k].update(rx.findall(text))
    m = {
        "files": len(cpp),
        "notes": sum(long_note(t) for t in cpp.values()),
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
        ("Source files", "files", target_files, "one per module (`data/modules.csv`)"),
        ("Placeholder functions `FUN_<addr>`", "fun", 0, "named"),
        ("Placeholder globals `DAT_<addr>`", "dat", 0, "named"),
        ("Placeholder classes `Class_<addr>`", "class", 0, "named"),
        ("Placeholder fields `field_<offset>`", "field", 0, "named"),
        ("Files that define `Unit`", "unit_defs", 1, "one shared definition"),
        ("Files opening with a matching note", "notes", 0, "none"),
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
        span = a - target
        done = 1.0 if span <= 0 else max(0.0, min(1.0, (a - b) / span))
        total.append(done)
        shown = f"{target:,}" if key == "files" else "0" if target == 0 else str(target)
        lines.append(f"| {label} | {a:,} | {b:,} | {shown}, {tgt_text} | {100 * done:.0f}% | `{bar(done)}` |")
    overall = sum(total) / len(total)
    head = [f"**Readability cleanup: about {100 * overall:.0f}% of the way** (mean of the rows below)", "",
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
