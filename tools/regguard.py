"""Check that a branch only edited the code inside its own region markers.

    uv run tools/regguard.py src/unsorted/0x4d8e60.cpp --base pilot2-4d8e60 \
        --head region-r3 --region r3

The file marks each region with `// REGION <name> begin` and
`// REGION <name> end` lines. Everything outside those markers (structs,
helpers, the frame, the markers themselves) is shared, and a region agent
must not change it. Every changed or inserted line in --head must fall between
the begin and end markers of one of the --region names (repeat the flag to
allow several). Exit code 0 when clean, 1 with the offending line ranges.

Use the skeleton commit as --base (tools/regionrun.sh writes it to
<log-dir>/base.txt, pass that with --base-file), never a branch that later
merges move. A region must also not declare new variables: a local declared
inside a region still changes the stack frame, and the frame is shared. Added
lines that look like a declaration are reported unless --allow-locals is given;
propose the local in the report instead.
"""

import argparse
import difflib
import re
import subprocess
import sys
from pathlib import Path

# A marker may carry the region's address range after begin/end, as
# docs/region-brief.md writes them: `// REGION r3 begin   0x40feda-0x4100d0`.
MARK = re.compile(r"^\s*//\s*REGION\s+(\S+)\s+(begin|end)\b")
NOT_A_TYPE = {"return", "goto", "else", "case", "delete", "new", "break", "continue", "sizeof",
              "typedef", "using", "throw", "do", "if", "while", "switch", "for"}
DECL = re.compile(
    r"^\s*(?:for\s*\(\s*)?(?:(?:const|unsigned|signed|struct|class|static|register)\s+)*"
    r"([A-Za-z_]\w*)(?:\s+|\s*\*+\s*)\**([A-Za-z_]\w*)\s*(?:=[^=]|;|\[|,)")


def declares_variable(line: str) -> bool:
    m = DECL.match(line)
    return bool(m) and m.group(1) not in NOT_A_TYPE


def git_show(ref: str, path: str, cwd: Path) -> list[str]:
    out = subprocess.run(["git", "show", f"{ref}:{path}"], capture_output=True, text=True,
                         cwd=cwd, check=True).stdout
    return out.splitlines()


def spans(lines: list[str]) -> dict[str, tuple[int, int]]:
    """Region name -> (first inner line index, index of the end marker)."""
    open_at, found = {}, {}
    for k, line in enumerate(lines):
        m = MARK.match(line)
        if not m:
            continue
        name, kind = m.groups()
        if kind == "begin":
            open_at[name] = k + 1
        elif name in open_at:
            found[name] = (open_at[name], k)
    return found


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("file")
    ap.add_argument("--base", help="skeleton commit or ref")
    ap.add_argument("--base-file", type=Path, help="file holding the skeleton commit (regionrun's base.txt)")
    ap.add_argument("--head", default="HEAD")
    ap.add_argument("--region", action="append", required=True)
    ap.add_argument("--repo", default=".", type=Path)
    ap.add_argument("--allow-locals", action="store_true", help="do not reject new variable declarations")
    args = ap.parse_args()
    if args.base_file:
        args.base = args.base_file.read_text().split()[0]
    if not args.base:
        sys.exit("regguard: give --base or --base-file")
    base = git_show(args.base, args.file, args.repo)
    head = git_show(args.head, args.file, args.repo)
    allowed = spans(base)
    missing = [r for r in args.region if r not in allowed]
    if missing:
        sys.exit(f"regguard: no markers for {', '.join(missing)} in {args.base}:{args.file}")
    if spans(head).keys() != allowed.keys():
        sys.exit("regguard: the region markers were changed")
    bad = []
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, base, head, autojunk=False).get_opcodes():
        if tag == "equal":
            continue
        # an insertion at i1 == i2 sits between base lines i1-1 and i1
        inside = any(lo <= i1 and max(i2, i1) <= hi for lo, hi in (allowed[r] for r in args.region))
        if not inside:
            what = f"base lines {i1 + 1}-{i2}" if i2 > i1 else f"insert before base line {i1 + 1}"
            bad.append(f"{tag}: {what} (head lines {j1 + 1}-{j2})")
        elif not args.allow_locals and tag != "delete":
            kept = {line.strip() for line in base[i1:i2]}
            for k in range(j1, j2):
                if head[k].strip() not in kept and declares_variable(head[k]):
                    bad.append(f"new variable at head line {k + 1}: {head[k].strip()}")
    if bad:
        print(f"regguard: {len(bad)} change(s) outside {', '.join(args.region)}:")
        print("\n".join("  " + b for b in bad))
        sys.exit(1)
    print(f"regguard: ok, only {', '.join(args.region)} changed")


if __name__ == "__main__":
    main()
