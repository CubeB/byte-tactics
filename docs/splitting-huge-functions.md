# Splitting huge functions between agents

The 139 unmatched functions over 1,000 bytes hold about 258 KB, the bulk of
what is left. MSVC allocates registers and lays out blocks across the whole
function, so a fragment cannot be compiled and matched alone. This workflow
splits the work on one function instead of the function itself. It comes from
two pilots (pilot 1 on `0x43f0e0`, pilot 2 on `0x4d8e60`, see
`splitting-pilot2-results.md`); neither reached a byte-identical match, but the
second reached 48.8% by `check.py` in 16 minutes.

## Workflow

Target: 20 minutes of wall clock for one function.

| Minutes | Phase | Who |
| --- | --- | --- |
| 0 to 5 | **Skeleton**: types, callee calling conventions, the frame, shared helpers, region markers with stub bodies | strong model |
| 5 | **Gate**: frame size and prologue shape (see below) | orchestrator |
| 5 to 14 | **Regions**: one agent per region in parallel, 9 minute limit each | cheap models |
| 14 to 17 | **Merge**: guard, merge, score each branch | orchestrator |
| 17 to 20 | **Integration**: cross-region fixes, final `check.py` | strong model |

1. **Skeleton.** Write the function's file so it compiles and has the
   right frame. Put everything shared (structs, helpers, declarations, the
   locals, callee prototypes) between `// SHARED begin` and `// SHARED end`.
   Mark each region `// REGION rN begin` and `// REGION rN end`. Region bodies
   may be stubs (a call with the right string and no arguments).
   Get the calling conventions from how the callers set up `ecx` and the stack,
   not from Ghidra: a wrong `__stdcall` against `__thiscall` adds a push at
   every call site and no region can fix that.
2. **Gate.** The frame constant (`sub esp, N` or the `_alloca_probe` size) and
   the prologue shape must match. Total size is not a fair gate while stub
   bodies leave out arguments (pilot 2's skeleton was 13.6% short by design).
   If the gate fails, still fan out but keep the prologue for the strong model.
3. **Regions.** `tools/regions.py <addr> --write` proposes region seams (the
   cases of a jump table, or greedy cuts of flat code) into
   `data/regions/<addr>.csv`; edit them where they look wrong. Give each agent
   the brief in `region-brief.md`. `tools/regionrun.sh` creates one worktree per
   region, starts the agents (at most four at once, see below) and kills each at
   its deadline.
4. **Merge.** For every branch run `tools/regguard.py` against the skeleton
   commit; it rejects any change outside the branch's own markers and any new
   variable in a region. Then merge, run `tools/regcheck.py <addr>`, and revert
   a merge that lowers the whole-function score.
5. **Integration.** Apply what the agents proposed, one change at a time,
   scoring each. Start with values that flow between regions (a call's return
   stored in a local, a library call repeated in two regions), because those
   change the frame and everything after it. Finish with `check.py`.

## Rules that came from the pilots

- **Regions do not touch shared code and declare no variables.** A helper or
  struct edit moves every region's score at once (pilot 1: one agent's helper
  edit raised its own region and lowered three others). A local declared inside
  a region still adds to the shared frame (pilot 2: a stray `char* reason` cost
  4 bytes and shifted every stack offset). Agents propose shared changes in
  their report, with the instruction evidence.
- **The frame decides the score.** Fix it in the skeleton. In pilot 1 the
  skeleton left an extra saved register and two wrong calling conventions, and
  every region inherited them.
- **Guard against the fixed skeleton commit.** `regionrun.sh` writes it to
  `<log-dir>/base.txt`; pass it with `--base-file`. Using the branch tip gives
  false alarms once other branches are merged into it.
- **At most four agents at once in a capped container.** Each opencode agent
  uses 55 to 70 threads plus Wine for every compile. Seven agents hit a 2,048
  pid cgroup limit and crashed (`uv` and `tokio` "can't spawn worker thread").
  `regionrun.sh` waits for free slots (`MAX_PARALLEL`, `PIDS_PER_AGENT`) and
  should be run in the background.
- **Limits are enforced by the launcher, not by the agent.** Give the limit in
  the brief and let `regionrun.sh` kill at the deadline. An agent's own step
  count let one pilot 1 agent run 35 minutes.
- **Shared repeated patterns first.** Unit lookup and visibility test code
  appeared six times in `0x43f0e0`, and the same `sprintf(log + strlen(log), ...)`
  line 40 times in `0x4d8e60`. Decide how the skeleton writes them once.
- **Score the merge, not just the branch.** A branch's own score can rise while
  the merged score falls.

## Scoring

- `tools/check.py <addr>` is the real check; it prints MATCH or a similarity
  and a diff. On a draft with the right code but the wrong frame it reads low
  (about 11% for pilot 1's skeleton).
- `tools/regcheck.py <addr>` scores each region separately. It matches every
  original basic block to its best block anywhere in our output, so a moved
  layout does not zero the score, and reports it exactly and by "shape"
  (register names ignored). It is a progress signal, not a match check: small
  blocks inflate it. `--diff <region>` lists a region's original instructions
  with their best-match score.
- Until 2026-09-29 `regcheck.py` did not apply our object's relocations, so
  every call and every string push counted as a mismatch. Region scores in the
  pilot results and in PRs before then are about 11 points low (0x40fbe0:
  74.5% exact and 85.5% shape as reported, 85.7% and 96.7% with the fix).
- **In a function with a switch, address ranges and source blocks do not line
  up.** MSVC can emit the cases in a different order from the source (0x40fbe0
  dispatches case 2, case 1, case 0), so a region's address range can hold code
  written in another region's markers. Its score then measures a different
  agent's lines, which that region's agent cannot edit. Cut such a function by
  the emitted order, or score each region by the code its markers produce.

## Tools

| Tool | Purpose |
| --- | --- |
| `tools/regions.py` | propose region seams from a jump table or by greedy cuts |
| `tools/regcheck.py` | per-region score (exact and shape) |
| `tools/regguard.py` | reject edits outside a region and new locals |
| `tools/regionrun.sh` | worktree, agent and deadline per region, with concurrency cap |
| `docs/region-brief.md` | brief to give every region agent |

## Results so far

| | Pilot 1 (`0x43f0e0`) | Pilot 2 (`0x4d8e60`) |
| --- | --- | --- |
| Shape | switch, 14 cases | flat sequence, 7 sections |
| Size | 4,420 bytes | 2,644 bytes |
| `check.py` after merge | 14.1% | 48.8% |
| Wall clock | 33 minutes | 16 minutes |
| Merge conflicts | 1 | 0 |
| What held it back | wrong frame and calling conventions in the skeleton | stack slot order (8 diff hunks) |

Both drafts are in the repository, with the remaining work listed at the top of
each file: `src/orders/order_dispatch_43f0e0.cpp` (39.6%) and `src/debug/debug_lib.cpp`
(48.8%).

## Open questions

- Whether a good skeleton lets regions reach exact matches, or the last gap is
  always integration work for a strong model (pilot 2 suggests the latter).
- Whether `regcheck.py` should become a real `check.py --region` with region
  stubs.
- Whether region work fits the issue and pull request flow in `AGENTS.md`
  (one issue per function, one pull request per merged result, regions as
  branches the orchestrator merges).
