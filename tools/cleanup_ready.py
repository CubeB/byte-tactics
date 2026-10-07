"""Label each open `cleanup` issue `ready` or `blocked` from what it waits on.

    uv run tools/cleanup_ready.py              # relabel the issues whose state changed
    uv run tools/cleanup_ready.py --dry-run    # print what would change

An issue is blocked while any issue it waits on is open. It waits on the
issues GitHub links to it as "blocked by", and on every `#<number>` in a
`**Blocked by:**` paragraph of its body, for issues that wait on more than
GitHub's 50 links allow. Agents pick only `ready` issues (AGENTS.md, "Pick and
claim an issue"). The build workflow runs this whenever an issue closes.
"""

import argparse
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BLOCKED_BY = re.compile(r"\*\*Blocked by:\*\*(.*?)(?=\n###|\Z)", re.S)


def gh(*args: str) -> str:
    return subprocess.run(["gh", *args], cwd=ROOT, check=True, capture_output=True, text=True).stdout


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    repo = gh("repo", "view", "--json", "nameWithOwner", "--jq", ".nameWithOwner").strip()
    issues = [i for page in json.loads(gh("api", "--paginate", "--slurp",
                                          f"repos/{repo}/issues?labels=cleanup&state=open&per_page=100"))
              for i in page if "pull_request" not in i]
    open_numbers = {i["number"] for i in issues}

    changed = 0
    for i in sorted(issues, key=lambda i: i["number"]):
        waits = (i.get("issue_dependencies_summary") or {}).get("blocked_by", 0)
        m = BLOCKED_BY.search(i.get("body") or "")
        if m:
            waits += len({int(n) for n in re.findall(r"#(\d+)", m.group(1))} & open_numbers)
        want, drop = ("blocked", "ready") if waits else ("ready", "blocked")
        labels = {label["name"] for label in i["labels"]}
        if want in labels and drop not in labels:
            continue
        changed += 1
        print(f"#{i['number']}: {want} ({waits} open blockers)  {i['title']}")
        if not args.dry_run:
            gh("issue", "edit", str(i["number"]), "--add-label", want, "--remove-label", drop)
    print(f"{changed} of {len(issues)} issues relabelled" + (" (dry run)" if args.dry_run else ""))


if __name__ == "__main__":
    main()
