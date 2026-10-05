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
    uv run tools/c2prio.py 0x424c00 --symbols g_game   # symbol ids and the file's symbol count too
    uv run tools/c2prio.py 0x4c8bb0 --frame            # the frame layout: counts, slots, offsets
    uv run tools/c2prio.py 0x4c8bb0 --rotation         # the expression temporaries' rotation
    uv run tools/c2prio.py 0x4c0820 --ids              # the freed ids that split pieces reuse

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

`--frame` prints C2's frame layout (FUN_0043f93b), which runs after the
allocator, during code generation (the rule is in docs/agent-guide.md, "Get the
frame layout from the reference counts"). The tool switches these hooks on from
the function's allocation end until the next function starts. FUN_00440cbd is
called once per local left in memory, in list order: ecx is the symbol (name
at `[[ecx]+0x18]`, kind at +4, size at +0x20, reference count at +0x34, its
number in the slots' member sets at +0x38) and edx the frame size so far. It
joins the newest earlier slot that is at least half its size and does not
interfere with it, or opens a new one. At 0x43f9dd the packing is done
(`[esp+0x10]` is the total), and at 0x43f9f1 it has been re-sorted by
FUN_00459eb7 (only when the total is over 0x80). The slots are 20-byte entries
at `[0x4910b8]` from index `[0x4910b0]` to `[0x4910c0]` - 1: +0 the member set,
+4 the interference set, +8 size, +0xc reference count. `[0x4910b4]` maps a
member number back to its symbol, and the entries before `[0x4910b0]` hold the
parameters. The tool prints each local in list order with its size, count and
the slot it opens or joins, then the slots nearest esp first with their offset
from the bottom of the locals and their members. A compiler temporary's spill
home is named after its candidate. On 0x4c8bb0 all 16 dword slots and both
arrays land where the original has them (stackcmp's original column, local
size 0x7d60), and so do the 14 named slots of the matched 0x4c8760 and the 7 of
0x4cf570. In 0x4c8bb0 it shows why `y1=vertices[next*4+1]` moves six slots:
nextVertex drops from 6 references to 3, so dl opens its own slot instead of
joining nextVertex's.

`--rotation` prints every expression temporary FUN_00435c37 places (see
"Temporaries" below), in code-generation order, from the same window. At its
entry edx is the tuple (its line at +0x10) and `[0x491120]` the rotating
pointer into the register table at 0x491100. FUN_00435f38 receives the
register chosen in ecx, and its return address gives the rule: 0x435cf9
rotation (the pointer moves past the register), 0x435db5 hint, 0x435e74 first
free register in table order (when no scratch register is free; the pointer
stays), and 0x435e22, 0x435e87 and 0x435eeb spills. On 0x4c8bb0 it shows the
right walk's two head temporaries (ecx, edx at the `y0=` line) leaving the
pointer at eax for du, where the original starts at edx. The left walk's
second head temporary gets esi by the first-free rule and does not move the
pointer, which is why the two walks, with the same head code, start du on
different phases. Adding both options left the default output, `--trace`,
`--blocks`, `--inline` and `--symbols` the same, line for line, on 0x4cf570
and 0x47d2e0.

`--ids` prints C2's freed candidate ids, which decide the ids of split pieces.
FUN_0040ecd2 frees a candidate by pushing it onto a list (head at 0x493230,
chained at +0x2c), and FUN_0040ebb6 gives every new candidate the id on top of
that list, so the id freed last is reused first; a new id is used only when
the list is empty. The tool breaks at 0x40ed2b (edx is the freed id, and the
return address at `[esp+8]` names the pass), reads the list at each split, and
prints each free with its pass: FUN_0041c72e and FUN_0041ca9b drop candidates
before the sort, FUN_004375fe takes one out of allocation, and FUN_00437e67
frees a split candidate once its pieces exist. For each batch of splits it
then prints the candidates split, the freed list, the id each piece took and
the ids freed afterwards. A step splits in candidate id order, and the reloads
after a region come out in that order (the one split later is reloaded
first), so a reload order is a question of which id a piece gets. In the
99.5% 0x4c0820 the drops before the sort leave `4 58 57 26 ...` on the list.
The `p++` constant (#4) was taken out of allocation after walk1's two address
temporaries (58, 57) and its y1 (26) were dropped. The third piece made at
walk2's split is pts's, so it gets 57. That is above the `i - 1` temporary's
49, so pts is split after #49 at walk1's split and reloaded first. The
matched file reads `p->y` and `p->x` into block locals `py` and `px`, which
are dropped too: the list becomes `4 17 20 60 ...`, pts's piece gets 20, and
the reloads swap into the original's order. On 0x4cf570 the list runs out
after three dropped temporaries, so `this`'s two pieces get new ids (36, 37).
The default output and every other option stayed the same, line for line, on
0x4cf570 and 0x47d2e0.

### Symbol ids (`--symbols`)

`--symbols NAME[,NAME...]` prints, before the table, the id of each symbol
named and the counts the ids come from. Part of 0x424c00:

      file total 33860 (0x8444 in 16 bits, bit 14 0): the front end's count at the end of the
      file, from the IL's header, so every declaration in the file moves it. C2 numbers the symbols
      it makes itself (temporaries, inlined locals, sections) on from there, function after function,
      and had reached 34468 (0x86a4) when this function's allocation started.
      name                               kind           id  low 16  bit 14  bit 15
      LoadFeatures (this function)       function    32883  0x8073       0       1
      g_game                             data        32690  0x7fb2       1       0
      Class_004b4560: a type, which never reaches C2. It was numbered before its members; the first one C2 has:
        Class_004b4560::FUN_004b4560     function    32542  0x7f1e       1       0

Where the numbers come from, read out of C2.EXE:

- The front end (C1XX) numbers every symbol it declares with one counter for
  the whole file: globals, functions, parameters, locals, struct and class
  tags, members, enumerators, typedefs and template instantiations. An unused
  `extern int` takes 1, a prototype with one parameter 2 and a one-member
  struct 7. There is no separate count of types; a "type count" measured with
  dummy structs is this counter in steps of 7.
- C2 does not number them. It reads each symbol's number from the IL
  (`FUN_00420250` decodes 15 bits in two bytes, or 31 in four) in the symbol
  reader `FUN_004206b7`, which stores it at symbol +0x28 (kind at +4: 1 data,
  4 function, 9 section; name at +0x18). `FUN_0040d5a8` hashes the global
  symbols into 1024 buckets at 0x48fb6c by `id & 0x3ff`; `FUN_0041f453` looks
  them up. Locals are not hashed; the tool reads theirs through the register
  candidates, so it finds a local, parameter or global that is a candidate.
  Types never reach C2: for a class it shows the first member C2 has, which
  the front end numbered after the class.
- The IL's header (read at 0x452f17, once per file) holds the counter's value
  at the end of the file: the "file total", kept at 0x497df8. The front end
  writes every function's IL after the whole file is parsed, so declarations
  after a function move this number too (checked with 100 `extern int`s after
  the last function, and with two functions: both see the same total).
- C2 numbers the symbols it makes itself (temporaries, the locals of inlined
  functions, COMDAT sections) with `FUN_0040d5d8`, from a counter at 0x491050
  that starts at the file total and is never reset, so the k-th function's
  own symbols come after those of the functions before it.

The effects measured so far follow the ids modulo 65536. Which id decides,
measured with `--symbols` and unused `extern int`s placed before g_game,
between g_game and the function, or at the end of the file:

| function | the id that decides | measured |
|---|---|---|
| 0x424c00 | g_game's | both spot stores are `offset + spots` while bit 14 of g_game's id is set: g_game 32767 gives 99.8%, 32768 97.5%, 49151 97.5%, 49152 99.8%. Moving the file total by up to 31000 changes nothing. |
| 0x47d0e0 | g_game's | the `imul` folds while bit 14 of g_game's id is set: 32767 MATCH, 32768 79.0%. The full `<windows.h>` puts g_game at 29019, the lean one at 12192 (79.0%). The function's own id and the file total crossing 32768 change nothing. The "type count" in its notes is this id. |
| 0x41b2e0 (without /Gi) | its locals', numbered at the function's definition (7 after the function's own id when the definition is its first declaration) | MATCH for function ids 64543 to 64575, 64607 to 64639, 64671 to 64703, 64735 to 64767 and 64799 to 64819; g_game's id and declarations after the function change nothing. With the function's prototype earlier in the file (ta_protos.h, below) its own id is 1910 and the bytes still match with `first` at 64560, so the locals decide. |
| 0x471de0 | the file total | MATCH for totals 65257 to 65554 (and 65556) with the declarations at the end of the file, where they move nothing else; the file's notes found the same window with them before g_game. C2's own counter, 427 above the total by this function's allocation, passes 65536 in that window. |

The option reads C2's symbol table once, at the allocator's first stop for
the function (655 symbols in 0x424c00, well under a second), and the rest of
the output is unchanged: the default output, `--trace`, `--blocks` and
`--inline` stayed the same, line for line, on 0x4cf570 and 0x47d2e0.

### Symbols each header adds

Measured with `--symbols g_game` on a probe file: the headers, then
`struct ProbeG { int a; int b; }; extern ProbeG* g_game;` and one function.
The count is g_game's id minus 170, its id with no header. "At the end" is
what a header adds after g_game (STL instantiations the front end makes at
the end of the file): it moves the file total but not the ids of later
declarations. All 388 headers of `toolchain/msvc5-sp3/INCLUDE` were measured
alone and after the full `<windows.h>`; these are the ones that matter:

| header | alone | after `<windows.h>` | at the end |
|---|---|---|---|
| `<windows.h>` (full; it includes `<mmsystem.h>`, `<winsock.h>`, `<commdlg.h>`, `<shellapi.h>`, `<ole2.h>`) | 28798 | 0 | 0 |
| `<windows.h>` with `WIN32_LEAN_AND_MEAN` | 11971 | | 0 |
| `<winsock2.h>` (instead of `<windows.h>`) | 29220 | | 0 |
| `<commctrl.h>` | | 1011 | 0 |
| `<ddraw.h>` | 29460 | 662 | 0 |
| `<dsound.h>` | 28954 | 156 | 0 |
| `<dplay.h>` | 28982 | 184 | 0 |
| `<d3d.h>` | 30619 | 1821 | 0 |
| `<d3drm.h>` | 32645 | 3847 | 0 |
| `<vfw.h>` | | 1933 | 0 |
| `<shlobj.h>` | 30982 | 2184 | 0 |
| `<imagehlp.h>` | | 410 | 0 |
| `<stdio.h>` | 302 | 295 | 0 |
| `<stdlib.h>` | 319 | 0 | 0 |
| `<string.h>` | 266 | 0 | 0 |
| `<math.h>` | 340 | 336 | 0 |
| `<tchar.h>` | 315 | 48 | 0 |
| `<process.h>`, `<io.h>`, `<mbstring.h>` | 234, 224, 207 | 227, 222, 205 | 0 |
| `<time.h>`, `<conio.h>`, `<direct.h>`, `<malloc.h>`, `<float.h>`, `<memory.h>` | 36 to 67 | 27 to 65 | 0 |
| `<vector>` | 4233 | 3579 | 425 |
| `<list>`, `<deque>`, `<stack>` | 3535, 3385, 3409 | 2881, 2731, 2755 | 109 |
| `<map>`, `<set>` | 2732, 2694 | 2078, 2040 | 0 |
| `<algorithm>` | 3049 | 2395 | 0 |
| `<queue>` | 5433 | 4779 | 425 |
| `<string>` | 6109 | 5415 | 647 |
| `<iostream>`, `<fstream>`, `<sstream>`, `<strstream>` | 6072 to 6280 | 5378 to 5586 | 642 to 684 |
| `<complex>`, `<locale>`, `<bitset>` | 7219, 6663, 6121 | 6525, 5969, 5427 | 647 to 778 |
| `<iostream.h>`, `<fstream.h>`, `<strstrea.h>`, `<iomanip.h>` | 908, 1117, 1042, 1283 | the same less 1 | 0 |
| the largest SDK headers: `<inetsdk.h>`, `<comdef.h>`, `<lm.h>`, `<mapix.h>`, `<setupapi.h>`, `<tspi.h>` | | 5355, 4814, 3569, 2407, 2352, 2383 | 0 |

Headers that share includes do not add up (`<string>` after `<vector>` adds
about 3100, not 5415), and the order matters a little (`<vector>` then
`<windows.h>` puts g_game at 32575, the other order at 32547), so measure a
set as a whole. The exe imports DDRAW, DSOUND, DPLAYX, SHELL32, IMAGEHLP,
ADVAPI32, GDI32, USER32, KERNEL32 and smackw32, which bounds what is
plausible: `<windows.h>` `<ddraw.h>` `<dsound.h>` `<dplay.h>` `<shlobj.h>`
`<imagehlp.h>`, six CRT headers and `<vector>` `<list>` `<map>` `<algorithm>`
`<string>` give 41247 (42209 with what they add at the end), and
`<windows.h>` `<ddraw.h>` `<dsound.h>` `<dplay.h>`, four CRT headers and
`<vector>` 33715. Every header of a generous plausible set, added in turn
after the full `<windows.h>` (`<windowsx.h>`, `<commctrl.h>`, the DirectX
headers including `<d3d.h>` and `<d3drm.h>`, `<vfw.h>`, `<shlobj.h>`,
`<richedit.h>`, `<imagehlp.h>`, `<tlhelp32.h>`, every CRT header, every STL
header that compiles and the old iostream headers on top), puts g_game at
52261: 52091 symbols of headers, 53234 with what they add at the end. Every header in the
directory that still compiles together reaches 86525, but only with MAPI, LAN
Manager, TAPI, setup, ODBC and OLE scripting headers.

What the stuck functions need, in symbols of headers before their own
declarations (each function's file with its own declarations as they are now):

| function | needs | the largest plausible set gives |
|---|---|---|
| 0x41b2e0 (without /Gi) | 64307 to 64339, 64371 to 64403, 64435 to 64467, 64499 to 64531 or 64563 to 64583 (counting `<vector>` and `<windows.h>`, 32405 now) | 52091, 12216 short |
| 0x471de0 | 64859 to 65156 including what the headers add at the end (`<vector>` alone gives 4657) | 53234, 11625 short |
| 0x47d0e0 | bit 14 of g_game's id set: 16163 to 32546 or 48931 to 65314 (the full `<windows.h>`, 28798, matches) | matched |
| 0x424c00 | none: g_game's bit 14 moves both spot stores, and the original has one of each | |

So no plausible set of real headers reaches the two windows: about 12000 more
symbols have to come from the lost Cavedog headers. The windows of
0x41b2e0 and 0x471de0 overlap (a 64500-symbol prefix with `<vector>` fits
both), and the same prefix would give 0x47d0e0's g_game bit 14 and 0x424c00's
`offset + spots` in the Animating loop, so one large common header in
front of every file is consistent with all four.

### Symbols each template instantiation adds

Measured with `--symbols g_game` in 0x424c00's file (`<list>`, `<map>`,
`<set>`, `<deque>`, `<string>` and `<algorithm>` included), each line placed
just before g_game. The front end instantiates a class template where its
complete type is first needed (a member, a global object, `sizeof`, a
`template class` line) and numbers its members there, so those ids come
before every later declaration. Every member function and function template
it instantiates is numbered at the end of the file instead, after the last
declaration, also when an inline or a plain function before g_game calls it
and also for `template class`: those move only the file total.

| instantiation | ids where it is first needed | ids at the end, all members (`template class`) |
|---|---|---|
| `std::allocator<T>` | 26 | 48 |
| `std::vector<T>` (its allocator included) | 137 | 472 |
| `std::deque<T>` | 234 | 959 |
| `std::list<T>` | 285 | 577 |
| `std::set<T>` | 350 | 750 |
| `std::map<int, T>`, `std::multimap<int, T>` | 394 to 400 (`std::map<std::string, T>` 402) | 823 |
| `std::pair<int, T>` | 12 | 4 |
| `std::string` | 0: `<string>` instantiates it | 489 |

- The element type does not matter (`int`, a pointer and a 0x100-byte struct
  all give 137 for a vector). A second container of the same element type
  shares the allocator (a `vector` and a `list` of one T: 395).
- A typedef of a container, a pointer to one, or a prototype taking one by
  reference does not instantiate it (3 or 4 ids).
  `std::vector<std::vector<T> >` instantiates only the outer vector (139).
- The struct holding a container gets implicit members with parameters: 12
  ids for a one-member struct, against 7 for `struct { int a; }`.
- Members used by code are counted at the end: `push_back` 94, `std::sort`
  141, `std::find` 9, `map::operator[]` 178, each with what it calls.

data/symbols.csv names 45 container classes in the whole exe: 40 `vector`
element types, 4 `map` trees and one `list` (`std::string` comes with
`<string>`). All 45, as members of one struct before g_game and on top of the
plausible header set above (`<windows.h>` `<ddraw.h>` `<dsound.h>`
`<dplay.h>` `<shlobj.h>` `<imagehlp.h>`, `<stdio.h>` `<stdlib.h>`
`<string.h>` `<math.h>` `<time.h>` `<io.h>`, `<vector>` `<list>` `<map>`
`<algorithm>` `<string>`), add 7443. The last column is what the invented
containers that reach each window took on top of the headers (one-member
structs as element types, a few more one-member structs to land inside a
narrow window):

| function, the id that decides | plausible headers | + the exe's 45 containers | reached the window with |
|---|---|---|---|
| 0x41b2e0 without /Gi, the function's | 41511 (88.0%) | 48954 (88.8%) | 147 vectors, 4 maps, 1 list, 28 structs: 64558, MATCH |
| 0x424c00, g_game's (the loops' locals follow it) | 41560 (97.5%) | 49003 (97.5%) | 150 vectors, 4 maps, 1 list, 20 structs: 64983, bytes match (only the static's name differs: its `$S` suffix is its symbol id, 64988, where data/symbols.csv has `$S4411`) |
| 0x449bb0 with `unsigned short` fields, g_game's | 41548 (93.0%) | 48991 (93.8%) | 80 vectors, 8 maps, 2 lists: 56402, MATCH |
| 0x471de0, the file total | 42635 (98.6%) | 50078 (98.6%) | 145 vectors, 4 maps, 1 list: 65198, MATCH |

So every window is a symbol-id window that instantiations reach like any
other declaration, but only with two to three and a half times every
container class the game is known to have, all declared in front of each
file. That is padding, so none of it is committed. After the exe's own
containers, 7400 (0x449bb0) to 16000 (0x424c00) ids are still missing: the
size of the game's own lost declarations (struct definitions, prototypes),
not of its templates.

What the real code of the original translation unit adds (#5541, measured
with `--symbols` on a concatenation of the matched files of the TU around
0x408100 to 0x40d290, 105 files from 0x407350 to 0x40d5b0, each in its own
namespace so their private struct views do not clash; that duplicates some
types and vector instantiations, so it overstates what the original had):

| function | the id that decides, and its window | as it is | + the real TU code before it | + the generous header set too |
|---|---|---|---|---|
| 0x409730 | i's front-end id, 65536 to 66547 (the full match needs about 65800 to 66500) | 33684 | 38711 (40 files, +5027) | 57276, 8260 short |
| 0x40d290 | the insert's own id, 65602 to 65654 | 33295 | 38356 (+5061, everything before 0x409160's class, the first `vector<unsigned char>`) | 56259, 9343 short |
| 0x408100 (hunk 3) | C2's counter at the function, about 65585 to 65985 | 5885 | 53008 (the whole TU and its headers, `<windows.h>` and `<ddraw.h>` among them; the TU's code adds about 17300) | 72045, past it |

As the table above has it, a vector instantiation costs its ids where it is first needed (155 here with its element struct and holder); the
member functions a file uses are instantiated at the end of the file (resize
and insert: 114 front-end ids there), so they move only the file total. For
0x408100 a real header set between the two does land in the window:
`<commctrl.h>` `<dsound.h>` `<dplay.h>` `<d3drm.h>` `<vfw.h>` `<shlobj.h>`
`<imagehlp.h>` `<string>` `<list>` `<io.h>` `<process.h>` on top of the TU puts
C2 at 65684 and fixes hunk 3 (99.0%; hunks 1 and 2 do not follow ids), but
only with the whole TU concatenated, so it is not committed.

0x409730 matched the other way round: its ids are made small instead of
wrapped. It inlines the real zero-caller neighbours 0x409520 and 0x4095d0, and
MSVC 5's `<vector>` is cut down to the members it uses (as in 0x437580), so
with `<stdio.h>` and `<minmax.h>` i's id is 946, inside the window the
original's wrapped id sits in modulo 65536 (about 898 to 958). The effects
there follow the ids modulo 65536 relative to each other, so a small file can
stand in for the lost prefix when every symbol that decides is in the file
itself. A cut-down `<vector>` costs a few hundred ids where the real one
costs 4233 (3579 after `<windows.h>`).

Small ids cannot stand in when the window puts one of the function's own
symbols below the compiler's predeclared ones. In a padded 0x40d290 file that
matches, the insert and its parameters have low 16 bits 63 to 66, below
operator new (`??2`, id 151) and operator delete (`??3`, 153), while the
body's locals are above them; without the wrap no declaration gets an id
under 154, and a cut-down `<vector>` (also reordered three ways) gives only
the real header's four states (#5565). 0x4b90a0 (#5550) does not follow the
count the other way either: in its real TU (0x4b86e0 to 0x4b9a50, with the
`<windows.h>` its neighbours need) its ids are about 29850 and it is 53.8%;
with the TU's ids wrapped past 65536 it cycles through the same states as its
small standalone file (best 87.1%), and with only C2's counter wrapped (its
locals just below 65536) it is 27 to 54%. Its plateau is the shape, and it
calls nothing, so no neighbour is inlined into it.

### A prototypes header (`include/ta_protos.h`)

`tools/protos.py` writes the prototypes we know of the game's own free
functions: every decorated `?name@@Y...` name in data/symbols.csv and
data/progress.csv (the names the files under src/ compile to), demangled, in
address order, after a forward declaration of each struct, class and union
they name (`struct Unit;`, with the key the decoration uses; MSVC decorates
with the key of a type's first declaration). `--verify` compiles each
prototype as a definition and checks that it decorates back to its name. On
2026-10-04 it held 2140 prototypes and 1030 forward declarations. Left out:
802 member functions and 246 constructors, destructors and operators (they
need the class bodies, which each file defines itself), 118 members of
template classes, 6 free functions that take a `std::vector` and one that
takes a pointer to member. No file includes it.

The whole header adds 7229 ids (measured in 0x471de0's file, which declares
none of its functions itself): about 1 per forward declaration and about 2.9
per prototype (one, plus one per parameter, plus a little for function
pointer parameters). Where a file already declares one of its functions with
the same signature, that is the same symbol, so in other files it adds a few
less. After a file's headers or in front of them, g_game and everything after
it get the same ids; only the functions the header declares move (a function
keeps the id of its first declaration). Under /Gi it is different: the header
moved 0x41b2e0's g_game by only 112 (25714 to 25826, with the file total read
as 4194304), which fits the earlier finding that dummy declarations change
nothing under /Gi.

Each function's file with `#include "ta_protos.h"` after its own headers (the
few names that clash renamed in the scratch copy, see below), and the same
with the plausible header set of the previous section added:

| function, the id that decides | the file now | + ta_protos.h | its window | still missing | + the plausible set too |
|---|---|---|---|---|---|
| 0x424c00, g_game's | 32690 (99.8%) | 39914 (97.5%: bit 14 clears) | 64976 to 64995 | 25062 | 48781 (97.5%), 16195 missing |
| 0x41b2e0 without /Gi, its locals' (`first`) | 32648 (32.7%) | 39873 (88.0%) | 64550 to 64582 (and four more) | 24677 | 48740 (87.0%), 15810 missing |
| 0x449bb0 with `unsigned short` fields, g_game's | 29394 (93.0%) | 36621 (93.0%) | 56338 to 56562, 56850 to 58386 | 19717 | 48772 (93.8%), 7566 missing |
| 0x471de0, the file total | 5055 (98.6%) | 12284 (98.6%) | 65257 to 65554 | 52973 | 49858 (98.6%), 15399 missing |

So the prototypes we have supply about 7200 of the missing ids, the same order
as the exe's 45 containers (7443), and no function reaches its window with
them. The plausible headers, the 45 containers (as members of one struct,
which is invented) and the header together put 0x449bb0's g_game at 56215,
123 short of its first window; by the two counts added up, the other three
stay about 8000 to 8800 short. What
Cavedog's headers also had and we cannot write yet: the member functions
(their class bodies), prototypes of the about 575 functions without a file,
the globals' declarations and the real struct definitions.

The header also clashes with three of the four files as they stand. 0x424c00
declares `unsigned short LoadFeatureType(char*)` where 0x4224b0.cpp has `int`
(the `int` version scores 91.8% here) and `Cell_00424c00* GetMapCell(int,
int)` where 0x481550.cpp returns `Cell_00481550*` (overloads that differ only
in their return type, error C2556). 0x41b2e0's `OpenGeneratorDialog(0)` becomes an
ambiguous call between its own `Unit_0041b2e0*` declaration and the header's
`Unit_0041b0f0*` one. 0x449bb0 declares `FUN_004455b0` with the default
convention where 0x4455b0.cpp has `__cdecl` (C2373; the call is the same for a
function without parameters), and `layer->handler = HandleBattleRoomClick` cannot pick
between two overloads. 0x471de0 compiles with it unchanged.

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

### A game types header (`include/ta_types.h`)

`tools/gametypes.py` writes one definition of each game type the files
declare, merged from their views (the tool's docstring has the rules). Every
file is compiled again with /Z7 and its CodeView records give each view as VC5
laid it out; the decorated names in each object say which views are one type
(the same global or function declared in two files, a member function's class,
a struct at the same offset of two views of one type, and, where the fields
agree, the same name). Fields are merged by byte range: a range keeps the field
most views declare over exactly those bytes, and stays unknown where as many
views disagree. Member functions come from the class bodies and from the
decorated names in data/progress.csv. A type the evidence says is another,
whose views disagree with that one's, is left out (declared, not defined).
`--verify` compiles a check of every type's size and every field's offset
against the views. It is not `tools/types.py`, which would shadow Python's own
`types` module for every tool in tools/. The files that include it are listed
under "Files that depend on its exact size" below.

**One unit's own types last (`--last`, `--reorder`).** Types come in address
order of the first file that uses them, but a translation unit's own types
(the ones only its files use) came from its own header in the original,
included after the common ones, so for that unit's files they belong last.
`--last LO-HI` (repeatable) puts the types every view of which is in a file
from LO to HI after all the others; a type that stays and holds one by value
or as a base keeps it in place. "Every view" and not "the first view": the 12
types first used in the AI unit but used elsewhere too (ByteMap_00408090 is
held by value by later types) cannot move without breaking the header.
`--reorder HEADER --last LO-HI` applies it to an existing header without
regenerating it: the views only pick the types (one whose size in the header
differs from the views' stays), every type's text is kept byte for byte, the
result is written over HEADER (or `--out`) and compiled once as a check, and
running it again changes nothing. A pure reorder keeps the header's id total;
only where each type and container instantiation is numbered moves.
include/ta_types.h is the 2026-10-04 header with
`--reorder include/ta_types.h --last 0x407350-0x40d5b0`: the AI unit's 40 own
types last (Owner, which holds the first `std::vector<unsigned char>`, among
them), which put 0x40d290's insert at 65604, in its window (#5668).

On 2026-10-04 it held 2038 types from 9161 views in the 3237 files, 74 names
only declared, and 2470 member functions, among them 777 members, 112
constructors and destructors and 3 operators that data/progress.csv decorates
(29 more belong to classes left out, 140 to template or nested classes). 28
types have a byte range the views disagree on, 160 groups of views are left
out, and all 10523 sizes and offsets check. It includes the system headers its
types use: `<windows.h>` `<ddraw.h>` `<dsound.h>` `<dplay.h>` `<stdio.h>`
`<vector>` `<list>` `<map>`.

What a declaration costs, measured with 100 of each in front of 0x471de0's file:

| declaration | ids |
|---|---|
| a struct or class, with no data members or with 1, 2 or 5 ints, two bitfields, a pointer, an embedded struct or a base | 7 |
| a forward declaration; with a definition after it | 1; 7 in all |
| a member function | 1, plus 1 per parameter |
| a constructor or `operator=` | 1 per parameter (it takes an implicit member's place) |
| a destructor | 4 |
| a class's first virtual member | 1 more |
| an anonymous union inside a struct | 7 |

So data members take no ids at all: a type costs 7 whatever its size, and what
a header of types adds is set by how many types, member functions and
container instantiations it has, not by how many fields it knows. After the
plausible header set of "Symbols each header adds" (which already includes
every system header it needs), ta_types.h adds 27968 ids: 14847 for the types
(7 each, with the anonymous unions in them and the names only declared), 6050
for the member functions, and 7082 for the 50 container classes its types hold
(45 vectors, two lists, a map and two pairs, each instantiated where the first
type holding it is defined).

Each function's file with the headers at its top in one order (the plausible
set, then ta_types.h, then ta_protos.h), the names the file defines that a
header defines too renamed in the scratch copy (the file keeps its own views,
a few hundred ids at most, which a file using the header would drop):

| function, the id that decides | its window | the file now | ta_types.h | + ta_protos.h | + the plausible set |
|---|---|---|---|---|---|
| 0x424c00, g_game's | 64976 to 64995 | 32690 | 62486, 2490 short | 69411, 4416 past | 76452 |
| 0x41b2e0 without /Gi, `first` | 64550 to 64582 and four more to 64826 | 32648 | 62415, 2135 short | 69339, 4513 past | 76380 |
| 0x471de0, the file total | 65257 to 65554 | 5055 | 62995, 2262 short | 69920, 4366 past | 77499 |
| 0x408100, C2's counter at the function | about 65585 to 65985 | 5889 | 63830, 1755 short | 70755, 4770 past | 78252 |
| 0x449bb0, g_game's | 56338 to 56562, 56850 to 58386 | 29394 | 62474, 4088 past | 69400 | 76441 |
| 0x40d290, the insert's own | 65602 to 65654, even | 33295 | 38371 | 38371 | 45414 |
| 0x4b90a0, its own | none known (its notes) | 175 | 62348 | 67861 | 74902 |

No function lands in its window with the headers whole. With the types header
and the system headers it needs, the four functions whose windows lie just
below or across 65536 are 1755 to 2490 short; with the prototypes header after
it they are 4366 to 4770 past, and with the plausible set 11000 to 12000 past.
0x449bb0 is past its window with the types header alone. 0x40d290's insert is
numbered where the first game type holding a `std::vector<unsigned char>` is
defined (38371), while the original's comes after about 65600 declarations.
That type is Owner, one of the AI unit's own types; with them last (the
reordered header, above) and its file's headers in front, the insert is at
65604 and 0x40d290 matches.
0x409730, matched with its ids made small, defines a cut-down `std::vector`
that cannot sit next to `<vector>`, so it cannot take the header.

What the two headers cannot settle, and how far it moves the counts: the 240
classes known only by one member function (`Class_<address>`, which in the
original are members of other classes) cost 7 each as classes of their own;
views of one type that no evidence joins are counted twice; the types no file
declares are missing; and ta_protos.h names each type by its defining file's
provisional name, so 731 of its 1030 forward declarations are names ta_types.h
does not use (1 id each, against 0 for a name already declared).

None of the windows above has a reachable image at small ids, the way
0x409730's (i at 65536 to 66547, so 0 to 1011 modulo 65536) did. 0x41b2e0's and
0x449bb0's are ids well below 65536 (64.5k, 56k to 58k). 0x424c00 needs the 3D
loop's `c` past the wrap and the Anim loop's below it; at small ids both are
below and the threshold rule gives the opposite mix. 0x471de0 and 0x408100
need C2's own counter to pass 65536 (0x408100 at 49 to 449 modulo 65536 would
need a file total under about 130, since C2 makes 322 symbols before this
function's allocation). 0x40d290's insert would have to sit below operator new
and delete, and 0x4b90a0's notes scanned small counts (0 to 2047 externs from
its 197-symbol file) without a match.

**Files that depend on its exact size.** These files match only with `ta_types.h` at its current size, so rerun `uv run tools/checkall.py 0x408100 0x424c00 0x41b2e0 0x471de0 0x449bb0 0x410850 0x40d290` after any regeneration or reorder: 0x40d290 (`<windows.h>` `<shlobj.h>` `<imagehlp.h>` `<d3d.h>` `<memory.h>` `<math.h>` + `ta_types.h`; it also needs the AI unit's own types last, so a regeneration needs `--last 0x407350-0x40d5b0`) (#5668), 0x410850 (the header's system headers and `<shlobj.h>` `<imagehlp.h>` `<math.h>` `<tchar.h>` `<time.h>` `<float.h>`, then `ta_types.h` inside `namespace ta { }`) (#5656), 0x424c00 and 0x471de0 (`ta_types.h` + `<shlobj.h>` + `<imagehlp.h>`), 0x41b2e0 (`ta_types.h` + `<shlobj.h>` + `<memory.h>`; `<time.h>`, `<malloc.h>` or `<direct.h>` also land), 0x449bb0 (`ta_types.h`, its own types and externs, then `ta_protos.h`) (#5657, #5658, #5660). 0x408100 includes `ta_types.h` with
`<time.h>` and `<shlobj.h>`, which puts its file total at 65680 in a window
ending at 65732. Regenerating `ta_types.h` with about 50 more ids breaks that
hunk again; rerun `check.py 0x408100` after any regeneration and switch it to
`<commctrl.h>` + `<algorithm>` (total 65314) or another real set that lands
(#5654).

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
- Where C2 reads symbol ids in 16 bits is not found: the bit-14 and
  modulo-65536 rules in "Symbol ids" are measured, not read. One place that
  keeps only the low 16 bits is at 0x414678, which copies a variable's id
  into another symbol's +0x3a.
