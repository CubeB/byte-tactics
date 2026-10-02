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

# REVIEW_NOFETCH=1: the caller has fetched the PR and main already (several
# reviews at once must not race to update the same refs).
[ -n "${REVIEW_NOFETCH:-}" ] || git -C "$ROOT" fetch -q origin "+pull/$PR/head:pr-$PR"
if [ ! -d "$DIR" ]; then
    git -C "$ROOT" worktree add -q --detach "$DIR" "pr-$PR"
else
    git -C "$DIR" checkout -q -f --detach "pr-$PR"
fi
ln -sfn "$ROOT/toolchain" "$DIR/toolchain"
mkdir -p "$DIR/orig" "$DIR/build"
ln -sf "$ROOT/orig/TotalA.exe" "$DIR/orig/TotalA.exe"
[ -d "$ROOT/build/ghidra" ] && ln -sfn "$ROOT/build/ghidra" "$DIR/build/ghidra"
# Start from the main checkout's compile cache: its keys hash each file's
# contents, so only the files this PR changes are compiled again.
[ -d "$ROOT/build/progress" ] && [ ! -d "$DIR/build/progress" ] && cp -r "$ROOT/build/progress" "$DIR/build/"

cd "$DIR"
changed=$(git diff --name-only "$(git merge-base "pr-$PR" origin/main)" "pr-$PR")
echo "== files changed"
echo "$changed" | sed 's/^/  /'
outside=$(echo "$changed" | grep -v '^src/unsorted/.*\.cpp$' || true)
[ -n "$outside" ] && echo "!! changes outside src/unsorted/: $(echo $outside)"

# Check the PR as it will be after merging: with everything merged since it
# branched, and with names rebuilt from all matched files (a caller and its
# callee in the same PR can disagree on a name, and only the rebuild shows it).
[ -n "${REVIEW_NOFETCH:-}" ] || git fetch -q origin main
# Pin the main being merged: other landings can move origin/main while this
# review runs, and the comparison below must use the same commit.
base=$(git rev-parse origin/main)
if ! git merge -q --no-edit "$base" >/dev/null 2>&1; then
    git merge --abort 2>/dev/null || true
    # Most conflicts are with the conventions the switch to /Gz wrote (#2290)
    # or with a later landing on the same file. Keep the PR's side of each
    # conflicting hunk; the checks below catch anything that gets worse, and
    # the orchestrator commits the checked result instead of merging the PR.
    clash=$(git merge-tree --write-tree --name-only "$base" HEAD 2>/dev/null | sed -n '2,/^$/p' | tr '\n' ' ' || true)
    if git merge -q --no-edit -X ours "$base" >/dev/null 2>&1; then
        echo "== conflicts with origin/main resolved in favour of the PR: $clash"
    else
        git merge --abort 2>/dev/null || true
        echo "!! does not merge cleanly with origin/main; checking the PR branch alone"
    fi
fi
# A PR branched before the switch to /Gz (#2290) was written for the __cdecl
# default; score it with the conventions that landing it will write back.
if ! git show "pr-$PR:tools/check.py" 2>/dev/null | grep -q '^DEFAULT_FLAGS = ".*/Gz'; then
    cpp=$(echo "$changed" | grep '^src/.*\.cpp$' | while read -r f; do [ -f "$f" ] && echo "$f"; done || true)
    if [ -n "$cpp" ]; then
        echo "== written for the old __cdecl default: adding explicit conventions"
        uv run --quiet "$ROOT/tools/fix_conventions.py" $cpp | tail -1 | sed 's/^/  /'
    fi
fi
echo "== whole project after merging"
uv run --quiet tools/progress.py | tail -1 | sed 's/^/  /'
git show "$base":data/progress.csv > build/main-progress.csv
regressed=0
awk -F, 'NR == FNR { if ($5 == "matched") m[$1] = 1; next }
         FNR > 1 && ($1 in m) && $5 != "matched" { print "  !! matched on main, now " $5 " " $6 "%: " $1 " " $3; bad = 1 }
         END { if (!bad) print "  no function that matches on main stops matching"; exit bad }' \
    build/main-progress.csv data/progress.csv || regressed=1
# A branch made before main moved on can carry an older, worse copy of a file
# that is still partial; the squash would overwrite the better one.
lowered=0
awk -F, 'NR == FNR { if ($5 == "partial") s[$1] = $6; next }
         FNR > 1 && ($1 in s) && $5 != "matched" && ($6 == "" || $6 + 0.5 < s[$1] + 0) { print "  !! partial on main at " s[$1] "%, now " ($6 == "" ? $5 : $6 "%") ": " $1 " " $3; bad = 1 }
         END { exit bad }' \
    build/main-progress.csv data/progress.csv || lowered=1

sources=$(echo "$changed" | grep '^src/unsorted/.*\.cpp$' | while read -r f; do [ -f "$f" ] && echo "$f"; done || true)
addresses=$(grep -hoE '^// FUNCTION: 0x[0-9a-f]+' $sources 2>/dev/null | awk '{print $3}' | sort -u || true)
echo "== re-check ($(echo $addresses | wc -w) functions)"
[ -n "$addresses" ] && uv run --quiet tools/checkall.py $addresses | sed 's/^/  /'

echo "== constructs to look at"
grep -nE '__fastcall|volatile|__asm|_emit|#pragma optimize|vtable *= *DAT_|\(void\*\) *0x[0-9a-f]{6}|0x00?[45][0-9a-f]{5}[^0-9a-f]' $sources 2>/dev/null \
    | grep -v '^\S*:[0-9]*:\s*//' | sed 's/^/  /' || echo "  none"

# Raw permuter output (AGENTS.md): self-assignments and its generated names.
echo "== permuter leftovers"
junk=0
if [ -n "$sources" ]; then
    git diff "$base" HEAD -- $sources | grep '^+' | grep -v '^+++' | grep -v '^+\s*//' \
        | grep -P '(?<![\w.>])(\w+(?:(?:->|\.)\w+)*) = \(?\1\)?;|\b(tmp|inl)[0-9]+\b' | sed 's/^+/  /' | head -10 > build/review-junk.txt || true
    [ -s build/review-junk.txt ] && junk=1
    # Code its own comment says the original does not have (a probe left in, #4901).
    if git diff "$base" HEAD -- $sources | grep '^+' | grep -v '^+\s*//' | grep -iE '\S.*//.*(not in the original|probe)' | head -3 | sed 's/^+/  /' | grep . >> build/review-junk.txt; then junk=1; fi
    # Raw control characters (a tool expanded \a or \v while writing the file).
    if git diff "$base" HEAD -- $sources | grep -a '^+' | LC_ALL=C grep -a -qP '[\x00-\x08\x0b\x0c\x0e-\x1f\x7f]'; then
        echo "  control characters in added lines (write them as escapes)" >> build/review-junk.txt; junk=1
    fi
fi
if [ "$junk" = 1 ]; then cat build/review-junk.txt; else echo "  none"; fi

git checkout -q -- data README.md 2>/dev/null || true
if [ "$junk" = 1 ]; then
    echo "!! do not merge as is: raw permuter output (self-assignments or tmpN/inlN names), code marked not in the original, or control characters; see AGENTS.md"
    exit 2
fi
if [ "$regressed" = 1 ]; then
    echo "!! do not merge as is: it breaks a function that matches on main"
    exit 2
fi
if [ "$lowered" = 1 ]; then
    echo "!! do not merge as is: it lowers a partial that main has a better copy of"
    exit 2
fi
