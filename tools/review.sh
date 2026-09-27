#!/usr/bin/env bash
# Review an agent's pull request (orchestrator):
#
#   tools/review.sh 14          # check out PR #14 in .worktrees/pr-14 and re-check it
#   tools/review.sh 14 --clean  # remove that worktree again
#
# Prints the files it changes (anything outside src/unsorted/ needs a look),
# merges origin/main in and rebuilds data/symbols.csv as the real merge will,
# re-checks every function annotated in the changed files with the real
# checker, lists functions that match on main but would stop matching (a name
# the PR's files disagree on), and lists constructs the agent guide forbids or
# discourages.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PR="${1:?usage: tools/review.sh <PR number> [--clean]}"
PR="${PR#\#}"
DIR="$ROOT/.worktrees/pr-$PR"

if [ "${2:-}" = "--clean" ]; then
    git -C "$ROOT" worktree remove --force "$DIR" 2>/dev/null || true
    git -C "$ROOT" branch -D "pr-$PR" -q 2>/dev/null || true
    exit 0
fi

git -C "$ROOT" fetch -q origin "+pull/$PR/head:pr-$PR"
if [ ! -d "$DIR" ]; then
    git -C "$ROOT" worktree add -q --detach "$DIR" "pr-$PR"
else
    git -C "$DIR" checkout -q -f --detach "pr-$PR"
fi
ln -sfn "$ROOT/toolchain" "$DIR/toolchain"
mkdir -p "$DIR/orig" "$DIR/build"
ln -sf "$ROOT/orig/TotalA.exe" "$DIR/orig/TotalA.exe"
[ -d "$ROOT/build/ghidra" ] && ln -sfn "$ROOT/build/ghidra" "$DIR/build/ghidra"

cd "$DIR"
changed=$(git diff --name-only "$(git merge-base "pr-$PR" origin/main)" "pr-$PR")
echo "== files changed"
echo "$changed" | sed 's/^/  /'
outside=$(echo "$changed" | grep -v '^src/unsorted/.*\.cpp$' || true)
[ -n "$outside" ] && echo "!! changes outside src/unsorted/: $(echo $outside)"

# Check the PR as it will be after merging: with everything merged since it
# branched, and with names rebuilt from all matched files (a caller and its
# callee in the same PR can disagree on a name, and only the rebuild shows it).
git fetch -q origin main
if ! git merge -q --no-edit origin/main >/dev/null 2>&1; then
    git merge --abort 2>/dev/null || true
    echo "!! does not merge cleanly with origin/main; checking the PR branch alone"
fi
echo "== whole project after merging"
uv run --quiet tools/progress.py | tail -1 | sed 's/^/  /'
git show origin/main:data/progress.csv > build/main-progress.csv
regressed=0
awk -F, 'NR == FNR { if ($5 == "matched") m[$1] = 1; next }
         FNR > 1 && ($1 in m) && $5 != "matched" { print "  !! matched on main, now " $5 " " $6 "%: " $1 " " $3; bad = 1 }
         END { if (!bad) print "  no function that matches on main stops matching"; exit bad }' \
    build/main-progress.csv data/progress.csv || regressed=1

sources=$(echo "$changed" | grep '^src/unsorted/.*\.cpp$' | while read -r f; do [ -f "$f" ] && echo "$f"; done || true)
addresses=$(grep -hoE '^// FUNCTION: 0x[0-9a-f]+' $sources 2>/dev/null | awk '{print $3}' | sort -u || true)
echo "== re-check ($(echo $addresses | wc -w) functions)"
[ -n "$addresses" ] && uv run --quiet tools/checkall.py $addresses | sed 's/^/  /'

echo "== constructs to look at"
grep -nE '__fastcall|volatile|__asm|_emit|#pragma optimize|vtable *= *DAT_|\(void\*\) *0x[0-9a-f]{6}|0x00?[45][0-9a-f]{5}[^0-9a-f]' $sources 2>/dev/null \
    | grep -v '^\S*:[0-9]*:\s*//' | sed 's/^/  /' || echo "  none"

git checkout -q -- data README.md 2>/dev/null || true
if [ "$regressed" = 1 ]; then
    echo "!! do not merge as is: it breaks a function that matches on main"
    exit 2
fi
