# Brief for a region agent

Give this to every agent of a `tools/regionrun.sh` run, with the function
address, file and region names filled in (`<...>`). See
`splitting-huge-functions.md` for the workflow.

---

You are one of several agents decoding ONE big function of a matching
decompilation of Total Annihilation (MSVC 5.0 SP3, `/O2 /Ob2 /MT /Gz`). Function
`<addr>` (`<size>` bytes, `<what it does>`) is split into regions; you own
exactly one region of `src/unsorted/<addr>.cpp`. Work autonomously; nobody
answers questions.

## Your worktree

- Work only inside the worktree path in your task (the branch is checked out).
  The toolchain, the original exe and the Ghidra export are linked in. Do not
  touch other worktrees or switch branches.
- Read `docs/agent-guide.md` by grep for specifics, and `AGENTS.md` section 4
  and "Writing style". No inline asm, no `#pragma optimize`, no hard-coded
  addresses, no em dashes.
- A compiling skeleton is in `src/unsorted/<addr>.cpp`. `<how the stubs are
  incomplete, e.g. every report line is a stub with the arguments missing>`.

## How to work

- `uv run tools/regcheck.py <addr>` prints a per-region score, independent of
  layout. "shape" ignores register names: get shape to 100 first, then exact.
  `--diff <region>` lists your region's original instructions with their score.
- `uv run tools/ctx.py <addr>` gives the annotated disassembly;
  `build/ghidra/decomp/<addr>.c` is Ghidra's pseudo-C (it drops varargs and
  gets types wrong, read the disassembly). Your region's original address range
  is your row in `data/regions/<addr>.csv`.
- `uv run tools/check.py <addr>` compiles and prints the whole-function diff.
  It will not match while other regions are unfinished, ignore that.
- Register allocation and layout are global, so a region cannot reach 100%
  exact while others are unfinished. Push shape to 100 first.

## Rules for your region

- Edit ONLY the lines between your region's `// REGION rN begin` and
  `// REGION rN end` markers. Everything outside (the `SHARED` block, the
  frame, other regions, the markers) is read-only.
- Do NOT declare new variables in your region, even block-scope ones: they
  change the shared stack frame. Use the locals the skeleton provides.
- If you need a shared change (a struct field, a declaration, a local, a
  helper, a prototype), do not make it. Describe it in your final report under
  "shared proposals" with the instruction evidence (addresses).
- `tools/regguard.py` rejects your branch if you touch anything else.
- Never kill processes by pattern (`pkill -f`, `killall`, `ps | grep` then kill).
- Code comments: default none; only a real why.
- HARD LIMIT: `<minutes>` minutes from your start (check `date` at the start
  and regularly). The launcher kills you at the deadline; anything uncommitted
  is lost. Commit every improvement immediately.

## Committing

- `git add src/unsorted/<addr>.cpp` only. `git commit -m "Update: <subject>" -m
  "Assisted-by: opencode:<model>"`. Subject imperative, at most 50 characters,
  no full stop. No Co-Authored-By. Do not push, do not open a pull request.

## Final report

Plain text: region, start and final regcheck scores (exact and shape), what you
changed, shared proposals with evidence, what still differs and why, number of
compile runs, commit hash.
