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

Every `decomp` issue is open to every model. Issues labelled
`hard` hold the biggest functions (over 1000 bytes); the label only marks
their size, not a narrower list of models. Every function is worked until it
matches or stops improving (see below).

```sh
gh issue list --label decomp --state open --search "no:assignee" --limit 20
```

Take the lowest-numbered issue from your list, unless the human told you which
size label to work on (`size:medium`, `size:large`, `size:huge`, `near-miss`).
GitHub search lags a minute or more behind claims and label changes, so the
list can show issues that are already taken. Before claiming, check the issue
itself:

```sh
gh issue view <N> --json assignees,labels,comments \
  --jq '{assignees: [.assignees[].login], labels: [.labels[].name], claim: ([.comments[].body | select(startswith("Claimed by") or startswith("Released")) | split("\n")[0]] | last)}'
```

`claim` is the most recent claim or release comment. Skip the issue if it has
an assignee, if `claim` starts with "Claimed by", or if its labels are not for
you. An issue whose `claim` starts with "Released" (the orchestrator frees
stale claims that way) or is null is free. Then claim it:

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
to the list for another issue.

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
   as its first line, where `<model>` is the model you actually are. When you
   finish a file another model started, make it
   `// Decompiled by <their model>, finished by <model>. Names are provisional.`;
   never remove another model's credit from a first line.
4. `uv run tools/check.py <addr>` until it prints MATCH. Use
   `uv run tools/checkall.py <addr> ...` for a whole batch and
   `uv run tools/headers.py <addr>` when registers or operand order won't
   budge.
5. When a function is close (about 90% or more) and stuck, run
   `uv run tools/permute.py <addr>` (`docs/permuter.md`). It tries thousands
   of meaning-preserving rewrites of your file for 15 minutes and writes the
   best one to `build/permute/<addr>/best.cpp`, with `best.diff` beside it.
   Read the diff and tidy it before you copy anything into your file: the
   output can contain temporaries named `tmp0`, helpers named `inl0` and
   other leftovers that need a sensible name or can go. Re-check with
   `check.py` after each edit, since a change that looks dead can be needed.
   Commit a permuter result only once it reads as plausible source: no
   self-assignments (`x = x;`), helpers that just return their argument, or
   stacked casts that do nothing. A MATCH that truly needs one of those can
   keep it with a comment saying so; for a partial, leave it out and list
   the useful changes in your notes as a lead for the next attempt.
   It runs 12 compiles at a time; pass `--jobs 4` when other agents on the
   machine are running it too.

### When to stop: keep going while you are getting closer

Work on each function until it matches. Earlier rounds stopped every function
after a fixed 20 to 60 minutes, and most of what is left has been retried ten
or more times that way: a near-miss usually needs a long run of small
experiments, not another short look. So there is no time limit and no cap on
`check.py` runs. Stop on a function only when you are stuck:

- **Stuck means no progress:** the function's best score has not gone up in
  the last 30 `check.py` runs or the last 60 minutes, whichever comes first
  (check the time with `date`). Every new best score starts both counts again.
  Scoring scratch variants with `check.py --sym` does not count as a run, but
  a new best found that way does reset the counts.
- **The issue is finished** when every function has matched or is stuck. Then
  open the pull request. If your session has to end before that (a usage
  limit, say), open it with what you have, as below.
- **Keep your best version in the file as you go:** whenever a scratch
  variant scores higher than `src/unsorted/<addr>.cpp`, copy it into the file
  at once. A step limit or a stopped session then never strands a better
  version in `build/scratch/`.
- **When you stop on a function:** leave your best version in its file, with a
  comment at the top saying what still differs and what you tried. Mark it
  `gave up` in the pull request table. It then counts as attempted, and the
  next attempt starts from your notes.

### Subagents (OpenCode)

In OpenCode, do not decompile the functions yourself first. Hand them to the
`decomp-worker` subagent. It runs on the same model as your session (whatever
model you were started with) and stops when it is stuck or out of steps:

1. Start one worker per function, all at the same time, each given its one
   address, the absolute path of your worktree (`.worktrees/issue-<N>`) and
   the name of the model you are (for its `// Decompiled by` line). Each
   worker only touches its own function's file.
2. When they report, run `uv run tools/checkall.py <all the issue's
   addresses>` yourself. Only trust MATCH lines you see from the checker.
3. For each function a worker reports as `partial (still improving)` (it ran
   out of steps while its score was still going up), start a fresh worker on
   it, telling it to start from the file and notes already there. Repeat
   until the function matches or a worker reports it `partial (stuck)`; then
   mark it `gave up`.
4. In the pull request table, the `model` column says which model wrote the
   final version of each file.

Other tools without subagents simply work through the functions in order.

Rules that matter most (the guide has the rest):

- Only create or edit `src/unsorted/<addr>.cpp` files for your issue's
  addresses. Do not change `data/`, `README.md`, `docs/` or `tools/`; the
  orchestrator updates those after merging. Tell the orchestrator in the pull
  request if you think one of them is wrong.
- Never use inline assembly, `#pragma optimize`, hard-coded addresses or
  `volatile` tricks to force a match. (The one exception is a field the
  original evidently declared `volatile`: every write to it in the exe goes
  through a register and every repeated read re-loads it. Say so in the pull
  request with the addresses; the orchestrator decides. The network flags at
  g_game+0x38d75 are such a field.) The checker rejects most of these, and
  the rest will be undone in review.
- Use exactly the names `ctx.py` shows for callees, globals and vtables. If a
  check fails only because a name in `data/symbols.csv` looks wrong, say so in
  the pull request with the evidence instead of working around it.
- Only report MATCH for functions where `check.py` printed MATCH.

## 5. Open a pull request

First bring in what was merged while you worked, and re-check your functions,
because a callee you call may have been matched under a new name in the
meantime:

```sh
git add src/unsorted/
git commit -m "Add: <matched> of <total> functions for #<N>"
git pull --rebase origin main
uv run tools/checkall.py <your addresses>
git push -u origin issue-<N>
gh pr create --title "Decomp #<N>: <matched> of <total> matched" --body-file <file>
```

**A follow-up after your pull request has merged** (you kept improving a
function): the old `issue-<N>` branch is already merged, so a new pull request
from it is empty. Start a new branch from the current main, copy your file in,
and check that the diff holds it before opening the pull request:

```sh
git fetch origin main
git switch -c issue-<N>-2 origin/main
cp <your improved file> src/unsorted/<addr>.cpp
git diff --stat origin/main    # must list your file
uv run tools/check.py <addr>   # the score you claim, on current main
```

If you can't push to the repository (an outside contributor), push to your
fork instead: `gh repo fork --remote --remote-name fork` once, then
`git push -u fork issue-<N>` and
`gh pr create --repo HectorBailey/byte-tactics --head <your-login>:issue-<N> ...`.

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
Open one pull request per issue, once, when you have finished all of the
issue's functions (each one matched or stuck), with every function in the
table. The orchestrator reviews and merges pull requests as soon as they
appear, so anything pushed to the branch after that is lost; if you do more
work on the same issue afterwards, open a new pull request for it.

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
