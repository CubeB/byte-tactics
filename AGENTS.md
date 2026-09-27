# Instructions for coding agents

This is TA: Byte Tactics, a matching decompilation of Total Annihilation (1997):
C++ that compiles, with the original Visual C++ 5.0 compiler, to byte-identical
machine code. Work is handed out as GitHub issues. Each issue lists a few
functions by address; you claim one, decompile its functions on your own
branch, and open a pull request. A human-run orchestrator re-checks every
function, merges the pull request and hands out the next issues.

Read this file, then `docs/agent-guide.md` (the technical guide: tools, rules,
naming, and hundreds of solved patterns). Follow both exactly.

## 1. Check the setup

Run these from the main checkout (`~/repos/personal/byte-tactics`):

```sh
gh auth status                      # must be logged in to github.com
ls toolchain/msvc5-sp3/BIN/CL.EXE orig/TotalA.exe
uv run tools/check.py 0x401070      # must print MATCH
```

If any of these fail, stop and tell the human; do not try to install things.

## 2. Pick and claim an issue

Issues labelled `hard` (larger functions, and near-misses other models could
not finish) are reserved for the strongest models: **GPT-6 Astra** and
**Claude Opus**.

If you are one of those models, take `hard` issues first:

```sh
gh issue list --label decomp --label hard --state open --search "no:assignee" --limit 20
```

Only when none are left, fall back to the list below.

If you are any other model, never take a `hard` issue:

```sh
gh issue list --label decomp --state open --search "no:assignee -label:hard" --limit 20
```

Take the lowest-numbered issue from your list, unless the human told you which
size label to work on (`size:medium`, `size:large`, `size:huge`, `near-miss`).
GitHub search lags a minute or more behind claims and label changes, so the
list can show issues that are already taken. Before claiming, check the issue
itself:

```sh
gh issue view <N> --json assignees,labels,comments \
  --jq '{assignees: [.assignees[].login], labels: [.labels[].name], claims: [.comments[].body | select(startswith("Claimed by"))]}'
```

Skip it if it has an assignee or any "Claimed by" comment, or if its labels
are not for you. Then claim it:

```sh
gh issue edit <N> --add-assignee @me
gh issue comment <N> --body "Claimed by <tool> / <model> on $(hostname) at $(date -u +%H:%MZ). Branch issue-<N>."
gh issue view <N> --comments
```

Every agent uses the same GitHub account, so the assignee only says "taken";
the comment says by whom. If `gh issue view` shows an earlier "Claimed by"
comment from a different agent, you lost the race: comment "Released, claimed
twice", do not unassign, and go back to the list for another issue.

## 3. Work in your own copy

```sh
cd "$(tools/worktree.sh <N>)"       # creates .worktrees/issue-<N> on branch issue-<N>
```

Several agents run at once, so never edit files in the main checkout. Do all
work, compiling and checking inside that folder. Keep scratch files in
`build/scratch/<first address of the issue>/`.

## 4. Decompile

For each function in the issue:

1. `uv run tools/ctx.py <addr>` shows the disassembly, the callees and their
   calling conventions, and Ghidra's pseudo-C.
2. Look for already-matched neighbours and near-copies in `src/unsorted/`
   (grep for a distinctive offset, string or callee address) and copy them.
3. Write `src/unsorted/<addr>.cpp` with `// Decompiled by <model>. Names are provisional.`
   as its first line, where `<model>` is the model you actually are.
4. `uv run tools/check.py <addr>` until it prints MATCH. Use
   `uv run tools/checkall.py <addr> ...` for a whole batch and
   `uv run tools/headers.py <addr>` when registers or operand order won't
   budge.

### Time limits: give up and move on

Some functions will not match with the model you are. That is expected: the
orchestrator re-issues what you leave to a stronger model. What is not useful
is spending hours on one function. So:

- **Per function:** stop after 15 `check.py` runs or 20 minutes (check with
  `date`), whichever comes first.
- **Per issue:** after 2 hours, stop and open the pull request with what you
  have.
- **When you stop on a function:** leave your best version in its file, with a
  comment at the top saying what still differs. Mark it `gave up` in the pull
  request table. It then counts as attempted, and the orchestrator hands it to
  a bigger model with your notes as a head start.

### Subagents (OpenCode)

In OpenCode, do not decompile the functions yourself first. Hand them to the
`decomp-worker` subagent, which runs on a cheap model and has its own step
limit:

1. Give each worker one or two addresses and the absolute path of your
   worktree (`.worktrees/issue-<N>`). Run up to four workers at once, each
   with different addresses.
2. When they report, run `uv run tools/checkall.py <all the issue's
   addresses>` yourself. Only trust MATCH lines you see from the checker.
3. For each function a worker left partial, try it yourself: at most 8
   `check.py` runs, starting from the worker's file and notes. If it still
   does not match, mark it `gave up`.
4. In the pull request table, the `model` column says which model wrote the
   final version of each file.

Other tools without subagents simply work through the functions in order.

Rules that matter most (the guide has the rest):

- Only create or edit `src/unsorted/<addr>.cpp` files for your issue's
  addresses. Do not change `data/`, `README.md`, `docs/` or `tools/`; the
  orchestrator updates those after merging. Tell the orchestrator in the pull
  request if you think one of them is wrong.
- Never use inline assembly, `#pragma optimize`, hard-coded addresses or
  `volatile` tricks to force a match. The checker rejects most of these, and
  the rest will be undone in review.
- Use exactly the names `ctx.py` shows for callees, globals and vtables. If a
  check fails only because a name in `data/symbols.csv` looks wrong, say so in
  the pull request with the evidence instead of working around it.
- Only report MATCH for functions where `check.py` printed MATCH.

## 5. Open a pull request

```sh
git add src/unsorted/
git commit -m "Add: <matched> of <total> functions for #<N>"
git push -u origin issue-<N>
gh pr create --title "Decomp #<N>: <matched> of <total> matched" --body-file <file>
```

The pull request body must contain:

```
Closes #<N>

Model: <tool> / <model>

| address | model | result | best % | check runs | notes |
| 0x401234 | deepseek-v4.1-flash | MATCH | 100 | 3 | needed unsigned char param |
| 0x401260 | glm-5.3 | gave up | 87.5 | 15 | register swap in loop I could not fix |

Suspected original bugs:
- 0x... : what looks wrong in Cavedog's code, and the evidence (or "none")

Advice for docs/agent-guide.md:
- one or two sentences per technique that is not in the guide yet (or "none")
```

Include partial files too: a close attempt with notes helps whoever tries next.
If you have to stop before finishing, open the pull request with what you have
and list the functions you did not reach as `not reached`.

## Writing style

These apply to everything you write in this repository: code comments,
commit messages, pull requests and issue comments.

- Never use em dashes. Use a comma, colon or parentheses instead.
- Commit messages are `<Type>: <Subject>`, where Type is one of Add, Fix,
  Update, Bump, Remove, Optimize, Merge, Refactor, Reformat or Docs. The
  subject is imperative, capitalised, at most 50 characters, with no full stop.
- Do not add "Co-Authored-By" lines, "Generated with ..." lines or any other
  attribution to commits or pull requests.
