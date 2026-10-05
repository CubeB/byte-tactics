# Pilot 2 results: 0x4d8e60

Second run of the region-splitting workflow described in
`splitting-huge-functions.md`, on a function of a different shape: the crash
handler that writes `ErrorLog.txt` (2,644 bytes, flat, about 40 `sprintf` report
lines, 83 KB frame). Branch `pilot2-4d8e60`. Not a match, but far ahead of
pilot 1.

## Outcome

| | `check.py` | bytes (original 2,644) |
| --- | ---: | ---: |
| Skeleton (stub report lines, no arguments) | 29.2% | 2,285 |
| After merging all seven regions | 40.4% | 2,663 |
| After the integration fixes | 48.8% | 2,652 |

Pilot 1 ended at 14.1% after 33 minutes. Pilot 2 ended at 48.8% in 16 minutes
15 seconds of wall clock (17:29:58 to 17:46:13). Eight diff hunks remain, all
stack slot order and the register the `CreateFileA` result lives in; the frame
size and every string, call and constant already match.

`regcheck.py` reports 51.5% exact and 59.3% shape for the same file. On flat
code it is close to `check.py`; per region it reads r1 84%, r2 75%, r3 49%,
r4 45%, r5 48%, r6 33%, r7 52%.

## Timeline

| Time | Minute | Step |
| --- | ---: | --- |
| 17:29:58 | 0 | clock starts; tools already built (`regions.py`, `regguard.py`, `regionrun.sh`) |
| 17:33:08 | 3 | skeleton committed, seven agents launched |
| 17:35 | 5 | crashes (see below); r5 to r7 stopped, r1 to r4 kept |
| 17:38:44 | 9 | pids limit raised by the user; r5 to r7 relaunched |
| 17:41:42 | 12 | r2, r3, r4 guarded and merged (no conflicts) |
| 17:42:08 | 12 | kill timers fire (r5 to r7 cut short, see below) |
| 17:42:44 | 13 | r1, r5, r6, r7 guarded and merged (no conflicts) |
| 17:46:13 | 16 | integration fixes done, final `check.py` |

## What happened

**Skeleton (3 minutes).** Written from the disassembly and Ghidra. The frame
(0x143f0), the callee conventions, the SEH-style argument (`EXCEPTION_POINTERS`),
the inline `sprintf(log + strlen(log), ...)` line and seven region markers came
out in one pass. The stub lines had the right format strings and no arguments,
so the file was 13.6% short. The gate as written (size within 1%) could not
pass at that point; the gate should be frame and prologue shape.

**Region phase.** Seven `deepseek-v4.1-flash` agents, one per region, all
editing only their own lines. `regguard.py` confirmed every branch stayed
inside its markers. All seven branches merged with no conflict. The agents
added the `sprintf` arguments (context and exception record fields), wrote the
loops and fixed structure.

**Integration (3.5 minutes).** Two fixes moved `check.py` from 40.4% to 48.8%,
and both were shared changes no region could make:

- `GetExceptionName` returns the exception description string that r2 prints.
  That value is a function-scope local set in r1 and read in r2. r2 had
  declared its own uninitialised `char* reason`, which added 4 bytes to the
  frame. The fix changed a prototype, a local and two regions.
- r1 called `strrchr` twice; the original reuses the first result.
  With the frame right the whole diff shrank from the first instruction to
  eight late hunks.

## Problems and what they show

**Process limit, not load.** Seven agents at once hit the container's pid
limit (2,048, 1,644 in use, 176 failed thread spawns: `uv` and `tokio`
panics). Each opencode agent uses 55 to 70 threads, plus Wine for every
compile. The safe number at once in a capped container is about four. The
pids limit was then raised. Agents that were mid-run retried and finished
(r1 to r4 needed two attempts).

**Launcher bug.** `regionrun.sh` starts a kill timer per region that globs
every `.env` file for that region name when it fires. The timers of the first
launch fired at 17:42:08 and also killed the relaunched r5 to r7, which had a
6 minute budget to 17:44:40. Those three got about 3.5 minutes each. Fix: the
timer must remember the exact env file of its own run, or the run must be
tracked by PID.

**`regguard.py` base ref.** After merging some branches, `pilot2-4d8e60` (the
base) contained their work, so later branches looked like they edited outside
their region. The guard must run against the skeleton commit, not the moving
branch tip.

**A region can add to the frame.** A region-local variable is still a frame
change: r2's `reason` and r6's `room` are declared inside their regions and
each costs stack. `regguard.py` cannot see this. Rule for the brief: regions
declare no new variables; if a value must live across a region, propose it.

**Slot order is not declaration order.** All twelve orderings of the three
locals gave the same score. MSVC's stack slot assignment did not follow
declaration order here, so the remaining stack hunks need a different lever
(usage order, scope or types) and are a job for the integration phase.

## Compared with pilot 1

| | Pilot 1 (0x43f0e0) | Pilot 2 (0x4d8e60) |
| --- | --- | --- |
| Shape | switch, 14 cases | flat sequence, 7 sections |
| `check.py` at the end | 14.1% | 48.8% |
| Wall clock | 33 minutes | 16 minutes |
| Merge conflicts | 1 (region r1 edited shared code) | 0 |
| Cause of shortfall | frame and callee conventions wrong in the skeleton | small stack layout details |

The two changes that helped most were not new agents but process: the frame
is fixed in the skeleton, and shared code is guarded so agents cannot edit it.
The stage that turned out to matter is integration: the fixes that moved the
score most were the ones that cross regions.

## Changes for the next run

Applied: 1 to 3 and 6 in `splitting-huge-functions.md` and `region-brief.md`,
2 also as a check in `regguard.py` (new variables in a region are rejected),
3 and 4 in `regionrun.sh` (concurrency cap with a pids check, kill timer tied
to its own run), 5 as `regguard.py --base-file` with the skeleton commit that
`regionrun.sh` records. Item 7 is the open work on the function itself, listed
at the top of `src/debug/debug_lib_4d8e60.cpp`.

1. Gate the skeleton on the frame size and the prologue shape, not on total
   size.
2. Regions declare no variables; report shared needs instead.
3. Run at most four agents at once in a capped container, or check the
   cgroup's `pids.max` first.
4. Fix `regionrun.sh` timers to kill only their own run.
5. Give `regguard.py` the skeleton commit as its fixed base.
6. Reserve the last five minutes for the strong model to apply cross-region
   fixes, and start by looking at values that flow between regions (returns
   stored in locals, repeated library calls).
7. Try the remaining eight hunks with usage order and scope changes.
