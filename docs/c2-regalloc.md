# MSVC 5 register allocation, read from C2.EXE

This is the register allocator of the back end we build with
(`toolchain/msvc5-sp3/BIN/C2.EXE`, VC++ 5.0 SP3), read with Ghidra and checked
against small test programs. It follows the frame-layout work in #5113 (see
"Get the frame layout from the reference counts" in `docs/agent-guide.md`). All
addresses are C2.EXE's own.

C2.EXE was built with basic-block reordering, so most functions are split
into hot and cold pieces far apart. Ghidra's function boundaries are therefore
rough, and assertion sites tell you which source file a piece came from: the
allocator is `color.c` (assertions at 0x4676b7, 0x46915e, 0x4692bb, 0x469309)
plus code generation's temporary assignment in `regasg.c`.

To see what the allocator actually does with your function, run
`uv run tools/c2prio.py <addr> [file]` (see "Reading C2's own numbers" below):
it prints every candidate's priority, tie key, list position and register,
straight from C2.EXE, in a few seconds.

## Where it runs

`FUN_00453305` is the per-function pipeline. With global optimisation on
(`/Og`, flag `DAT_0048fb4c`), the order is: global optimiser, then
`FUN_00414a64` (builds the register candidates: one per web of a local,
parameter, compiler temporary or constant), then `FUN_00416e6a` (the global
allocator, color.c), then `FUN_0042a8aa` (assigns registers to expression
temporaries during code generation and cleans up afterwards).

Several decisions below depend on `DAT_0048fb60`, bit 23 of the per-function
optimisation flags. Every decision we could test behaves as if it is set under
`/O2`, so the rules below assume it (it is most likely `/Ot`).

## Register numbers and the order table

C2 numbers registers eax 1, ecx 2, edx 3, ebx 4, esp 5, ebp 6, esi 7, edi 8
(name table at 0x49d698). Both allocators walk the same order, stored twice:

    0x49b4a8 (global allocator) and 0x491100 (temporaries):
    eax, ecx, edx, esi, edi, ebx, ebp

ebp is in the table only when the function has no frame pointer
(`FUN_0041be92`: 7 integer registers with frame-pointer omission, 6 without).

## Global allocator (color.c): which local gets which register

Functions: `FUN_00416e6a` (driver), `FUN_0040ebb6` (creates a candidate),
`FUN_0040ee1d` (priorities), `FUN_0041bdd7` and `FUN_0041b6fa` (priority list),
`FUN_0041a6f8` (drops candidates not worth a register), `FUN_0041b785` (picks
the register), `FUN_0041ba2b` (updates the neighbours), `FUN_00439385`
(live-range splitting).

It is a priority-based colouring (Chow and Hennessy style), not Chaitin's
simplify-and-select:

1. **Candidates.** Every web of a local, parameter, compiler temporary
   (induction variables, loop counters) and constant gets a 0x44-byte candidate
   with a sequential id. Its "allowed" set starts as all registers of its class
   (integer or x87), minus ebp when there is a frame pointer. A web that is live
   across a call loses eax, ecx and edx. A web that interferes with a
   hard-register use (a `rep movs`, a shift count, a division) loses that register.

2. **Dropped candidates.** A candidate with fewer than 2 references and a
   weighted reference total below 1 stays in memory (`FUN_0041a6f8`). A
   candidate whose only two references are a copy is forwarded into its use.

3. **Priority.** `FUN_0040ee1d` walks the basic blocks. For each block `b`:

   - `w(b)` is 1 outside loops, 4 in a loop and 8 in a nested loop (measured
     with `c2prio.py --blocks`; block field +0x86 holds the depth);
   - each reference to a variable costs 2, a constant reference 1 or 0
     (`FUN_0040ed6c`, `FUN_0040fada`);
   - `K(b)` is the number of candidates referenced in the block, so references
     in a crowded block weigh more;
   - a candidate referenced in `b` gains `w(b) * K(b) * (its cost in b)`; a
     candidate live through `b` without a reference loses `w(b) * K(b)`.

   The sum is the priority (candidate +0xc). A second total, `w(b) * cost`
   without the `K(b)` factor, is the spill cost (+0x3c) used in step 2.

4. **Order.** Candidates are inserted into a list in id order (from a hash on
   `id & 0x3ff`), sorted by priority, largest first, then by the key at +0x40,
   largest first. A new candidate goes **before** equal ones, so a full tie
   goes to the higher id. +0x40 is the number of the last tuple that writes
   the candidate: `FUN_00416a8d` numbers the tuples block by block in block
   order, but each block's tuples from the last to the first, and stores the
   number in every candidate a tuple writes (0 for a constant). So on equal
   priority the candidate whose last write is in a later block goes first,
   and within one block the one written **earlier** goes first. That is why
   small straight-line tests looked like "the variable that appears first
   wins", and why in 0x47d2e0 `bit` (last written in the LOS block) beats the
   parameter `los` on equal priority. Checked on all 506 candidates of
   0x4cf570, 0x47d2e0 and 0x453d40 with `tools/c2prio.py`.

5. **Choice** (`FUN_0041b785`). For the candidate at the head of the list, each
   allowed register gets a cost, starting at 0. A neighbour that wants a
   specific register (a copy to or from it, such as the return value into eax,
   or a value fed to a fixed-register instruction) adds its preference to
   that register's cost; a neighbour with only one register left adds
   `100 * weight`. The candidate's own preferences subtract. The cheapest
   allowed register wins, and **ties go to the earlier register in the table**
   (strict `<` while walking eax, ecx, edx, esi, edi, ebx, ebp).

6. **Neighbours** (`FUN_0041ba2b`). The chosen register is removed from every
   interfering candidate's allowed set. A neighbour left with nothing is split
   (`FUN_00439385`): it keeps a register where it can and is stored and
   reloaded around the blocks where it cannot. This is the "`this` spilled at
   entry and reloaded after the loop" pattern; it is a split, not a whole spill.

Consequences that agents can use directly:

- A local live across a call can only get esi, edi, ebx, ebp, in that order of
  priority. The highest-priority one gets esi.
- A local not live across a call gets eax, ecx, edx first (unless a neighbour's
  preference makes another register cheaper), then esi, edi, ebx, ebp.
- ebp is used only when esi, edi and ebx are all taken (also enforced after the
  fact by `FUN_0042bf00`, which moves a value in ebp to ebx, esi or edi,
  in that order, if one of them ends up unused).
- To move a variable to an earlier register, give it more weighted references
  than its rival, or, on a tie, move its last write to a later block (or
  earlier in the same block). `tools/c2prio.py` shows both numbers.

## Reading C2's own numbers: tools/c2prio.py

    uv run tools/c2prio.py 0x4cf570                    # the file under src/
    uv run tools/c2prio.py 0x4cf570 build/scratch/0x4cf570/try.cpp
    uv run tools/c2prio.py 0x4cf570 --trace            # every colouring step too
    uv run tools/c2prio.py 0x47d2e0 --blocks bit,los   # each block's share of a priority
    uv run tools/c2prio.py 0x424c00 --inline           # the /Ob2 inline decisions too

It compiles the file with the real C2.EXE under a debugger and prints, for the
one function, C2's register candidates in the order `FUN_0041bdd7` sorted them,
which is the order `FUN_0041b785` colours them in. Part of 0x4cf570 (matched):

      #   id  candidate              lines              prio +0x40 spill refs  allowed  register
      0   34  temp                   112                  96    69    24    3  acdsibp  eax
      1   25  j                      149,151-153          88   215    24    7  acdsibp  eax
      2   17  i                      102,108,115,118      80    81    36    7  ...sibp  esi
     ...
     23   16  bestidx                101,115,127          -7    72    12    3  ...sibp  ebp
     24    5  this                   87..153             -11     9    58   20  ...sibp  split: esi #36, esi #37
     ...
     28    1  const 0                                   -122     0    -1   29  ...sibp  split: ebp #26, 4 pieces immediate

- `#` is the position in the sorted list. `id` is C2's candidate id; ids
  restart at 1 for each function, and C2 reuses the id of a freed candidate
  for a new one (the pieces of a split candidate get such ids).
- `candidate` is the local, parameter or global (globals can be candidates
  too), `temp` for a compiler temporary (a common subexpression, a hoisted
  value, an induction variable or a strength-reduced pointer), `local temp`
  for an unnamed front-end local (an inlined function's argument or result,
  for one; with its frame offset if it has one), or `const N`. Narrow ones are
  marked 8-bit or 16-bit. A variable with several webs has one row per web.
- `lines` are the source lines of the tuples that read or write it. Each tuple
  carries a line relative to the line before the body's `{`, and inlined code
  gets the line of the call. Inside loops C2 has restructured they are only
  roughly right.
- `prio` (+0x0c), `+0x40`, `spill` (+0x3c) and `refs` (+0x24) are the
  candidate fields of steps 2 to 4, as they are when the list is built.
- `allowed` is the registers still allowed then, in the order table's order
  (a=eax, c=ecx, d=edx, s=esi, i=edi, b=ebx, p=ebp; a dot where not allowed):
  `...sibp` is a value live across a call.
- `register` is what `FUN_0041b785` gave it. A split candidate lists the
  registers its pieces got, with their ids (and how many pieces stayed in
  memory); `memory`, or `immediate` for a constant, if it never got one. Later
  passes can still change a few of these (`FUN_0042bf00` moves a lone ebp to
  ebx, esi or edi; a callee-saved register holding only constants worth less
  than 3 goes back to immediates).

Under the table come the candidates `FUN_0041a6f8` dropped before the sort,
and the floating-point ones, which `FUN_0045f4b7` puts on the x87 stack and the
tool does not trace. `--trace` adds each colouring step in order: the
candidate, its priority at that moment, the registers still allowed, the
register it got and `FUN_0041b785`'s nonzero costs (`ebp +8200` is a neighbour
that has only ebp left, a negative cost is a preference), then splits,
candidates skipped because their spill cost is not positive, and the re-sorts
after `FUN_0040ee1d` recomputes the priorities following a split.

`--blocks` shows where each priority comes from. `--blocks bit,#65` limits it
to the candidates named, by the name the table prints or by `#id`. For each
basic block in `FUN_0040ee1d`'s first pass it prints w and K and the
candidate's share: w * K * cost where the block references the candidate, and
-w * K where the candidate is live through the block without a reference.
Below that is a list of the candidates each block counts in K. Constants count
in K at cost 0 or 1. The shares add up to the table's priority; this held for
all 95 candidates of 0x47d2e0 and 0x4cf570. Measured w is 1 outside loops,
4 in a loop and 8 in a nested one (step 3). On 0x47d2e0,
`bit` is 70, of which 64 comes from the vis-test block (K 8, cost 8: `1 << player`
is three references and the test one). The width temporary is 38, and `los`
is 70, gathered over nine blocks. The tool reads two points in
`FUN_0040ee1d`. At 0x40f5c7 (a block's end) it reads the block at `[esp+0x20]`,
the referenced list in ebx, the live list at `[esp+0x10]` (flag 0x10 at +6
marks referenced) and each candidate's cost at +0x18. At 0x40f5f5 it reads
w in edi and K in esi. From these it repeats C2's own arithmetic at 0x40f70d
and 0x40f742 (gains) and 0x40f72c (losses). The hooks are only on between the
allocator's entry and the sort, so the default output and `--trace` are
unchanged. A label, `do { } while (0)`, `switch (0)` or a constant initialiser
set earlier (`bit = 1;` and later `bit <<= n;`) is merged or propagated before
allocation, so none of them adds a block or changes K.

`--inline` prints, before the table, the /Ob2 inliner's decision at each call
site to an inline candidate (the rule is in docs/agent-guide.md, "The /Ob2
inline budget, read out of C2.EXE"): the function's IL size and budget, then
per site the depth, R (that level's sites still to come, this one included),
the budget left at that level, the callee's IL size and whether it was
inlined, in the order C2 visits them (depth-1 sites in source order, each
inlined callee's own sites right after it). It breaks at 0x42491e (a
function's pass starts; `[ecx]` is its symbol), 0x424eef (a site: the callee's
symbol in ebx, its name at `[ebx+0x18]` and its IL size in the low 16 bits of
`[ebx+0x64]`, the budget left at `[esp+0x48]`, the depth at `[esp+0x30]`, R at
`[esp+0x2c]`) and 0x424f95 (that site is inlined). The numbers read this way
make the rule exact: an inlined callee costs its IL size if 41 or more and
nothing otherwise, its own sites start from (budget left - cost) / R, and each
level also loses what is inlined below it. On the real-`<vector>` spelling of
0x424c00 the first resize gets (2024 - 115) / 18 = 106, enough for two
size() calls (43 each) but not for insert (470), a third size() or erase (70),
and the second resize's erase finds 47 of its (1351 - 115) / 7 = 176 left, as
in the file's notes. On 0x410850 it reproduces the inlined and out-of-line
vector calls the file's header lists (Patrol's sites start from 1000 - 606 =
394; units.empty()'s size() gets 302 / 7 = 43 for its 42). Adding the option
left the default output, `--trace` and `--blocks` the same, line for line, on
0x4cf570 and 0x47d2e0.

How it works: the tool copies C2.EXE to `build/c2prio/<run>/c2p<run>.exe` with
`jmp $` at the entry point (toolchain/ is never changed) and compiles with
`/B2` pointing at the copy, so CL runs it with the usual `MSC_CMD_FLAGS`. It
finds the spinning copy by its unique name in winedbg's `info process`, starts
`winedbg --gdb` on it, and runs gdb with the tool itself as the gdb Python
script. That puts the two entry bytes back and records what the allocator does
at a dozen addresses (listed at the top of the tool), with breakpoints whose
Python `stop()` returns without stopping. The compile then finishes normally;
its object is byte-identical to a plain compile. Two quirks needed handling:
winedbg answers gdb's first packet only after more bytes arrive, so gdb talks
to it through a small relay in the tool that sends one `+` after that packet
(otherwise every run waits 2 s for gdb to resend), and gdb must keep the
breakpoints inserted (`breakpoint always-inserted`) or each stop rewrites all
of them.

Needs gdb with Python (the distribution's gdb package) and Wine's winedbg; no
mingw and no change to the Wine prefix. Each run uses its own directory and
copy of C2, so several agents can run it at once (six parallel runs gave the
same output as one). Typical times: 2 to 3 s for 0x4cf570, 4 s for 0x47d2e0,
18 s for 0x453d40 (8944 bytes, 411 candidates); most of it is one stop per
reference for the `lines` column.

Validated on 0x4cf570 (matched) and 0x47d2e0 (88.5%):

- 0x4cf570: `bestidx` -7 and `this` -11, the numbers #5115 measured for the
  matched source; `bestidx` gets ebp, `this` is split into two pieces that both
  get esi (re-sorted at 80 and 22), and `i` esi, `best` ebx, the second loop's
  pointer edi, `slot` edi and the zero constant ebp all agree with the
  original's code.
- 0x47d2e0: `bit` 70, `los` 70 and the width temporary 38, with `bit` ahead of
  `los` on +0x40 (71 against 26), exactly the numbers in the file's header
  (#5068). `FUN_0045aaf9` moved nothing in this function.
- For both functions every colouring step (candidate, priority, register: 30
  and 55 steps) is identical to an independent trace made the #5115 way (the
  IL captured by a mingw-built `/B2` wrapper and C2 rerun under gdb).

## Temporaries (regasg.c): the scratch rotation

`FUN_00435c37` gives each expression temporary a register during code
generation, after the global allocator has placed the variables:

1. If the temporary has a hint (it is copied to or from a register variable,
   or it holds a value going to a fixed register) and that register is free,
   it takes the hint.
2. Otherwise (with `DAT_0048fb60` set) it takes the first free register among
   eax, ecx, edx **starting from a rotating pointer** (`DAT_00491120`), and the
   pointer moves to the register after the one chosen. The pointer is reset to
   eax once per function (`FUN_0042a8aa`), not per statement or per block. So
   consecutive temporaries rotate eax, ecx, edx, eax, ..., skipping registers
   that hold variables or live temporaries.
3. Otherwise the first free register in table order (eax, ecx, edx, esi, edi,
   ebx, ebp).
4. Otherwise a register is freed by spilling (`FUN_0045a827`, `FUN_0045d93e`,
   `FUN_0045d33f`).

`FUN_0042b2c4` is a second round-robin over the whole table (with a per-block
reset) used by `FUN_0042b3e2`, a pass that renames registers for Pentium
pairing.

## Constants in registers

Constants are candidates too (kind 0xd). A constant reference costs 1, and
references that could just as well be immediates cost 0, so a constant needs
several uses to win a register. After code generation, `FUN_0042bf00` checks
each callee-saved register that holds only constants and gives it back
(re-materialising the immediates) if its weighted benefit is below 3.
Measured with `p->f[i] = 0` stores:

| stores of 0 | no call in between | a call after each store |
|---|---|---|
| 1 to 2 | immediates | immediates |
| 3 to 4 | `xor ecx, ecx` | immediates |
| 5 or more | `xor ecx, ecx` | `xor edi, edi` (callee-saved) |

## Validation

All tests compile with the project flags (`/O2 /Ob2 /MT /Gz`) and read the
`/Fa` listing. The scripts generate random C functions, predict each local's
register from the rules above and compare.

| test | what it checks | result |
|---|---|---|
| straight-line, calls between uses (2 to 5 locals, 0 to 5 extra uses each, random definition order) | priority = reference count in one block; ties by first appearance; esi, edi, ebx, ebp; fifth local in memory | 820 of 820 |
| straight-line leaf code (2 to 6 locals loaded from globals, 1 to 5 uses each) | same ranking onto eax, ecx, edx, esi, edi, ebx | 460 of 460 |
| ties with declaration order reversed, definition order reversed, and use order reversed | first appearance in code wins, declaration order does not matter | 5 of 5 |
| the same tie after 0 to 69 unused `extern` declarations | ties do not depend on earlier symbols | 70 of 70 stable, but too short a range: in 0x487080 the pattern changes at fixed symbol-count thresholds (286 and 798 externs there), so scan a wide range of dummy declarations before ruling symbol count out |
| scratch rotation (four `G[i] = G[j] + k` statements; the same through a pointer held in eax) | eax, ecx, edx, eax; and ecx, edx, ecx, edx when eax is taken | as predicted |
| loop weighting (one local used k times in a loop, its rival N times before it) | a loop reference is worth several straight-line ones | see below |

Loop weighting, measured. Local `a` is used `k` times inside a loop and once
in the peeled first iteration; `b` is used `N` times before the loop. `b`
first takes esi at:

| loop | k = 1 | k = 2 | k = 3 |
|---|---|---|---|
| `for (;;) { g(a); if (g(9)) break; }` (depth 1) | N = 6 | N = 9 | N = 13 |
| the same loop nested in another (depth 2) | N = 19 or 20 | N = 31 or 32 | |
| `while (n--) g(a);` (depth 1, with a counter) | N = 5 | N = 7 | |

Both depth-1 shapes fit the priority sum above with one ratio,
`w1 * K1 / (w0 * K0) = 8/3`, ties going to `a`: for the `for (;;)` shape `b`
wins when `w0 * K0 * (N - k) > w1 * K1 * (1 + 2k) / 2`, for the counter loop
when `w0 * K0 * N > w1 * K1 * (1 + 2k) / 2`. The weight ratio `w1 / w0` in the
code is 2, which needs `K1 / K0 = 4/3`; a plain count of the values touched in
each block gives 2/3 at most. So the `K(b)` term
counts more than the source-level locals (compiler temporaries such as call
results and loop counters are candidates too), and we could not reproduce it
exactly from source. In practice: **one reference inside a loop is worth
about 3 to 4 references outside it, and one inside a nested loop about 12**.

## The coordinator's observations, checked

1. *A constant 0 is held in a register depending on how many uses are
   counted.* Confirmed: constants are candidates, need several weighted uses
   (3 stores for a scratch register, 5 for a callee-saved one, table above),
   and `FUN_0042bf00` gives a callee-saved register back when a constant's
   benefit is below 3. A shared tail written as an inline helper called from two
   arms is counted twice before the arms merge, so it can push the zero over
   the threshold (0x4a4170).
2. *Per-statement scratch rotation ecx, edx, eax.* Confirmed and explained:
   a round-robin pointer over eax, ecx, edx that is reset once per function
   and advances past each register handed to a temporary. So any change in
   the number or order of temporaries earlier in the function shifts every
   later one: reordering statements (0x4876c0) and adding a temporary
   (0x4b5980) both work through this pointer. Our guess for the latter is that
   the temporary was given a register and then folded away, advancing the
   pointer without emitting code.
3. *A local split between a register and a stack home counts one extra
   reference.* Consistent: the split (`FUN_00439385`) adds a store or reload
   tuple that the frame-layout count sees. Not separately tested here.
4. *Inline helper boundaries change allocation without adding code.*
   Consistent: an inlined helper's parameters and locals are separate
   candidates (later coalesced with the copies), so they change the per-block
   counts `K(b)`, the candidate ids that break ties, and the copy preferences in
   step 5.
5. *A field's declared width changed register choice elsewhere.* Plausible
   through `K(b)`: a different width changes the temporaries in that block
   (a `movzx` load and so on), which changes the weight of every reference
   in the block. Not tested.

## Stuck functions

No stuck function moved in this session; the time went into reading C2 and the
validation suite. What the model says about them:

- **0x4cf570** (`this` spilled at entry and reloaded): that is a split
  (step 6). `this` must lose the loop region to the loop's own locals on
  priority while keeping esi outside it. The extra do-while level that
  reaches 90.2% works by raising the loop's weight (`w(b)` doubles per loop
  level and `K(b)` is large there). A version with the same weights and no
  extra test would need another way to add loop-weighted references to bestidx
  and best, or to remove references to `this` inside the loop. MATCHED in
  #5164 by reading C2's priorities under a debugger (the method
  `tools/c2prio.py` now packages): bestidx had to outrank `this` (-7 against
  -11) so that bestidx takes ebp and `this` is split.
- **0x487080** (one step of the bit-copy chain on edx instead of ecx):
  MATCHED in #5156, and the hypothesis above it was wrong. Removing any one of
  the 23 calls and copies before the chain, or adding code-free temporaries,
  left the chain's registers unchanged, so it is not the temporary rotation.
  The edx step is the one where the `or` keeps its result in the old flags
  register, and it is set by the chain's own spelling (the record's 12-bit block
  layout from the matched save function 0x4876c0 put it on the original's step)
  and by the number of symbols declared before the function, which changes the
  pattern at fixed thresholds (an `extern int` counts 1, a one-member struct 7).
- **0x4a3780** (`flags` in a register against in memory): `flags` wins a
  register because `flag8`'s loop uses share it at the in-place shift. In the
  original it stays in memory, so its priority there must be below the
  register's other claimants, or its spill-cost total must fall below 1
  (step 2). The file's probe that makes `flags` live across the 0x10 path's
  calls (removing eax, ecx, edx from its allowed set) reproduces the original
  frame, which fits: with only callee-saved registers left, `flags` loses to
  higher-priority locals.
- **0x453d40** (8944 bytes): too large for hand counting. The rules above say
  which way each change pushes priorities; `tools/c2prio.py` lists its 411
  candidates in about 18 s.

## Not done

- A predictor that works from the source alone would need C2's basic blocks,
  webs and compiler temporaries, which are not visible in the source;
  `tools/c2prio.py` reads them from C2 instead. The exact `K(b)` count (the
  three sets `FUN_0040ee1d` adds up at 0x40f5c9 to 0x40f704) is still not
  written down, and the tool does not yet show each block's share of a
  priority.
- `FUN_0041a985` (532 lines), which builds the conflict and preference
  information before each choice, was skimmed, not read.
