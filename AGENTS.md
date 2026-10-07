# Instructions for coding agents

This is TA: Byte Tactics, a matching decompilation of Total Annihilation (1997):
C++ that compiles, with the original Visual C++ 5.0 compiler, to byte-identical
machine code. Every function matches, and `tools/place.py` rebuilds the shipped
exe byte for byte. What is left is cleanup: making the source read like source
(one file per translation unit, real names, shared types) without changing a
single byte. Work is handed out as GitHub issues labelled `cleanup`; you claim
one, do it on your own branch, and open a pull request. A human-run
orchestrator re-checks it and merges it.

Read this file, then `docs/cleanup-roadmap.md` (the plan and what limits it)
and `docs/tidy-up.md` (folders, names and how a merge or rename is done).
`docs/agent-guide.md` is the reference for when a function stops matching.

## 1. Check the setup

Run these from the root of your clone of the repository (the "main
checkout"). A contributor setting up for the first time follows
`CONTRIBUTING.md` first.

```sh
gh auth status                      # must be logged in to github.com
ls toolchain/msvc5-sp3/BIN/CL.EXE orig/TotalA.exe
uv run tools/check.py 0x401070      # must print MATCH
```

If any of these fail, stop and tell the human; do not try to install things.

## 2. Pick and claim an issue

**Take only `cleanup` issues labelled `ready`.** An issue labelled `blocked`
waits on others (listed under "Blocked by" on the issue) and becomes `ready`
by itself when they close; never relabel one by hand.

```sh
gh issue list --label cleanup --label ready --state open --search "no:assignee" --limit 20
```

The work goes in this order, and the labels enforce it:

1. **Gather** (`Clean-up: gather <module> ...`): every one is ready now.
2. **Join** (`Clean-up: join the <module> parts ...`): after all of that
   module's gather parts are merged.
3. **Name** (`Clean-up: name the placeholders in <module>`): after that
   module's gather or join is merged, since both rewrite the same files.
4. **Shared names** (`Clean-up: name the placeholders shared across modules`):
   last, one group per claim.

Work on one module at a time, and do not take a second issue on a module
someone else has claimed. GitHub search lags a minute or more behind claims
and label changes, so the list can show issues that are already taken. Before
claiming, check the issue itself:

```sh
gh issue view <N> --json assignees,labels,comments \
  --jq '{assignees: [.assignees[].login], labels: [.labels[].name], claim: ([.comments[].body | select(startswith("Claimed by") or startswith("Released")) | split("\n")[0]] | last)}'
```

`claim` is the most recent claim or release comment. Skip the issue if it has
an assignee or if `claim` starts with "Claimed by". An issue whose `claim`
starts with "Released" (the orchestrator frees stale claims that way) or is
null is free. Then claim it:

```sh
gh issue edit <N> --add-assignee @me
gh issue comment <N> --body "Claimed by <tool> / <model> on $(hostname) at $(date -u +%H:%MZ). Branch issue-<N>."
gh issue view <N> --comments
```

If `gh issue edit --add-assignee` fails because you are not a collaborator on
the repository (outside contributors can't assign themselves), the "Claimed by"
comment alone is your claim; the orchestrator will assign you. Several agents
can share one GitHub account, so the assignee only says "taken"; the comment
says by whom. If `gh issue view` shows a "Claimed by" comment from a
different agent after the last "Released" comment and before yours, you lost
the race: comment "Lost the claim race, the earlier claim stands" (never start
it with "Released", which would free the issue), do not unassign, and go back
to the list for another issue. If you stop before finishing, comment
`Released` (as the first word) so someone else can take it.

## 3. Work in your own copy

```sh
cd "$(tools/worktree.sh <N>)"       # creates .worktrees/issue-<N> on branch issue-<N>
```

Several agents run at once, so never edit files in the main checkout. Do all
work, compiling and checking inside that folder. Keep scratch files in
`build/scratch/<N>/`.

## 4. Do the issue, and keep every function matching

Each issue's body says exactly what to change and which checks to run; follow
it. A cleanup changes only how the source reads, never what it compiles to, so
every function you touch must still print MATCH and the exe must keep its MD5:

```sh
uv run tools/checkall.py <every address in the files you touched>
uv run tools/place.py               # must end with MD5 8e74a1dffa1f5988624c52048f5b20cd
uv run tools/modules.py --check
```

`place.py` compiles every changed file, so run it once at the end, not after
each edit. `tools/rename.py` runs its own checks; the issue says which flags.

**Why harmless-looking edits break matches.** MSVC 5's register allocation
depends on a symbol counter that grows with every declaration before a
function (`docs/c2-regalloc.md`): a type costs 7 ids, a member function 1 plus
1 per parameter, a forward declaration 1, and including a header costs all of
its declarations. Data members cost nothing. So merging files, adding a header
or removing a cast can move a later function's registers even though nothing
in that function changed. Some casts, declaration orders and unused locals are
load-bearing for the same reason; a one-line comment above such a construct
says so, and it stays.

**When a function stops matching:**

1. Find the smallest part of your change that causes it (undo pieces until it
   matches again).
2. Fix it if the fix reads as plausible source: the patterns are in
   `docs/agent-guide.md`, and `uv run tools/check.py <addr>` shows the
   difference.
3. Otherwise leave that part out: a function that only matches in a file of
   its own stays in one, with a one-line comment at its declaration saying
   why. Say so in the pull request.

Rules:

- Change only what the issue asks for. Change `data/` only through the tools
  the issue names (`rename.py` updates `data/symbols.csv`); do not edit
  `README.md`, `docs/` or `tools/`. Tell the orchestrator in the pull request
  if you think one of them is wrong.
- Never use inline assembly, `#pragma optimize`, hard-coded addresses or
  `volatile` tricks to keep a match (`#pragma auto_inline` has one allowed
  use, in a class's file: `docs/agent-guide.md`).
- When files are gathered into one, keep every model's credit from their first
  lines in the new file's first line, and every `// FUNCTION:`, `// GLOBAL:`
  and `// FLAGS:` line.
- A name needs evidence (strings, callers, Thaldren's names in
  `data/thaldren/`). A name with no evidence stays a placeholder.
- Only report MATCH for functions where the checker printed MATCH.

## 5. Open a pull request

First bring in what was merged while you worked, and re-check, because a
rename or a merge elsewhere may touch your files:

```sh
git add -A
git commit -m "Refactor: <what the issue did>"
git pull --rebase origin main
uv run tools/checkall.py <your addresses>
git push -u origin issue-<N>
gh pr create --title "<Type>: <what the issue did>" --body-file <file>
```

If a rename's rebase conflicts, start a fresh branch from main
(`git fetch origin main && git switch -c issue-<N>-2 origin/main`) and run
`tools/rename.py` again with the same pairs, rather than merging by hand. Never
use `git reset --hard`: it throws work away, and agent sandboxes refuse it.

If you can't push to the repository (an outside contributor), push to your
fork instead: `gh repo fork --remote --remote-name fork` once, then
`git push -u fork issue-<N>` and
`gh pr create --repo HectorBailey/byte-tactics --head <your-login>:issue-<N> ...`.
CI cannot build a fork's pull request until a maintainer approves the run, so
put your own check results in the body.

The pull request body must contain:

```
Closes #<N>

Model: <tool> / <model>

Checks: checkall <matched> of <total> MATCH; place.py MD5 8e74a1dffa1f5988624c52048f5b20cd; modules.py --check passes

Left out:
- what the issue asked for that you did not do, and why (or "none")

Suspected original bugs:
- 0x... : what looks wrong in Cavedog's code, and the evidence (or "none")
```

`Closes #<N>` matters: closing the issue is what frees the issues waiting on
it. Open one pull request per issue. The orchestrator merges pull requests as
soon as they check out, so anything pushed to the branch after that is lost;
open a new pull request for more work on the same issue.

## Writing style

These apply to everything you write in this repository: code comments,
commit messages, pull requests and issue comments.

- Never use em dashes. Use a comma, colon or parentheses instead.
- Commit messages are `<Type>: <Subject>`, where Type is one of Add, Fix,
  Update, Bump, Remove, Optimize, Merge, Refactor, Reformat or Docs. The
  subject is imperative, capitalised, at most 50 characters, with no full stop.
- Do not add "Co-Authored-By" lines, "Generated with ..." lines or any other
  attribution to commits or pull requests.
