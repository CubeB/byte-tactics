// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, retried by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, retried by claude-opus-5-5. Names are provisional.
// Codex GPT-6 retry for #5189 (2026-10-03): baseline remains 88.5%.
// A no-op goto/label placed immediately before or after the bit definition
// keeps the same 88.5% residual; prior allocation and header results remain.
// GPT-6 retry (#5237): rechecked at 88.5%; the documented live-range ordering remains.
// #5285 Codex retry: re-confirmed 88.5%. The forced LOS coloring remains
// unreachable through the source shapes and block labels already recorded below.
//
// claude-opus-5-5 pass (#5165, 88.5%, no score gain; about 150 variants read
// with tools/c2prio.py). Block names below: B4 `if (los)`, B5 call + first
// Contains x test, B6 its y test, B7 the vis test, F the flag test, E1/E2/E3
// the explored arm's x test, y test and byte read, S1/S2/S3 the seen arm's.
//  1. PER-BLOCK PRIORITIES, read out of C2. A copy of tools/c2prio.py with five
//     more hooks in FUN_0040ee1d gives each block's w and K and every
//     candidate's share: 0x40f5c7 (block end; ebx = list of candidates
//     referenced, [esp+0x10] = live list, flag 0x10 at +6 = referenced),
//     0x40f5f5 (edi = w, esi = K), 0x40f70d and 0x40f742 (eax gains
//     edx * [eax+0x18]), 0x40f72c (eax loses edx). Constants count in K at cost
//     0. For this file: bit 70 = B7 64 (K 8, cost 8: `1 << player` is 3
//     references in every spelling tried, the test 1) + S3 12 - F, S1, S2;
//     W1 38 = B7 32 + S3 12 - 6; los 70 = B4 10 + B5 14 + B6 4 + B7 16 + E1 6
//     + E2 4 + E3 10 + S1 4 + S2 4 - F 2.
//  2. THE TARGET IS CONFIRMED BY A FORCED BUILD. With bit kept out of a
//     register (`volatile`, or a function-scope bit also used at the end, so
//     it is live through the loop) and two fake width uses in S3 so that W1
//     outranks los, the LOS block becomes the original's: edi = los in the
//     prologue, esi = W1, `mov [esp+0x4c], esi` after the shift, `xor ebp, ebp`
//     moved up, `mov ebx, [esp+0x4c]; test ebp, ebx` (that operand order comes
//     only with bit a real candidate, not volatile), S1 a memory compare. With
//     the explored arm spelled as in the matched sibling 0x4658e0,
//     `m->size.Contains(tx, ty) && m->Get(tx, ty) != 0` (MapSize-level
//     Contains), E1 also becomes the original's `mov ecx, esi; cmp edx, ecx`.
//     That copy is NOT a CSE use of W1: c2prio puts E1's reference on los. The
//     code generator replaces the load of [edi+0x80] with the register it
//     knows holds it (only along the fall-through path, which is why S1, a
//     jump target, stays a memory compare). The forced build's only other
//     difference is the imul operand order (`mov ebx, eax; imul ebx, esi`
//     against `mov ebx, esi; imul ebx, eax`), which its fake uses may cause.
//  3. NO PLAUSIBLE SPELLING GETS THERE. The original needs W1 >= los (W1 wins
//     the tie on +0x40) and bit below the g_game pieces (the ebx piece is 32
//     after the split) and vis. With the original's visible references (W1:
//     B7 def and use, S3, perhaps E3; E1 and S1 read memory) W1 is 38 to 44
//     against los 68 to 70 and bit 70. Measured, W1 - los: both helpers' return
//     forms (24 combinations of if/&&/r/early-return/ternary) -6 to -14; the
//     same with IsSeen computing the bit (a CSE temp, 54 to 60, but los then
//     takes esi) -6 to -12; nine spellings of the bit's definition (all 70);
//     index and width locals, `m` placement, a helper taking the width, the
//     vis test through a helper: unchanged. The gap is 25 to 30 points, so
//     the original's IL must differ in a way the bytes do not show. One lead
//     not tested: a block boundary inside B7 with no jump (a label), which
//     would put bit's 3-reference definition in a small-K block.
//
// claude-opus-5-5 pass (#5068, 88.457% -> 88.486%; check.py shows both as
// 88.5%). The body now writes IsExplored through a `ByteMap* m =
// &los->explored` local with ByteMap-level Contains + Get and no bit
// parameter, and IsSeen returns through `int r` (if/else) with the bit still a
// parameter. That restores two sites of the original for free: the `cols` home
// at [esp+0x18] and the seen arm's `xor eax, eax; jmp` fail path (40 diff
// lines against 41). The LOS register choice is unchanged. This section
// replaces guesswork about that choice with C2's own numbers.
//  A. READING C2's PRIORITIES. Patch a copy of C2.EXE with `jmp $` (EB FE) at
//     0x4172f4 (just after FUN_0041bdd7 sorts the candidates), compile with
//     that BIN under Wine, find the spinning C2.EXE with `winedbg` (`info
//     process`; winedbg uses Wine's debug API, so ptrace_scope=1 is no
//     obstacle), `attach 0x<pid>` and dump memory with `x /Nx`. List head at
//     [0x4910d4]; candidate +0x0c priority, +0x14 next, +0x1c id, +0x24 refs,
//     +0x3c spill cost, +0x40 tie key (larger goes first); *cand = symbol,
//     *symbol = storage record, record+0x18 = name. A second copy patched at
//     0x417317 (end of allocation) gives each candidate's register as
//     (+0x10 - 0x494758) / 0x50 (1..8 = eax ecx edx ebx esp ebp esi edi), and
//     a code cave at 0x48cf00 entered from 0x41b785 logs the order in which
//     candidates are coloured (count at 0x49e700, pointers after it). Scripts:
//     build/scratch/0x47d2e0/c2prio/ in the #5068 worktree.
//  B. WHAT THEY SAY. The colouring order is the sorted priority order (split
//     pieces are re-inserted later), and esi in the LOS block goes to the
//     first of bit, los and W1 to be coloured. los must not get it, or the
//     prologue rotates (los esi, cols edi, y0 si: the old notes' "rotation").
//     The original (W1 esi, los edi, bit no register) therefore needs
//     W1 > los and W1 > bit, with bit also below the g_game pieces (ebx, ebp)
//     and vis (ecx). Measured, as bit / los / W1:
//       file before this pass                     76 / 74 / 44 (refs 5 / 10 / 3)
//       this file                                 70 / 70 / 38 (bit first on +0x40)
//       explored arm CSE'd to W1, bit parameter   70 / 62 / 50
//       the same with IsSeen computing the bit    54 / 62 / 50 (los takes esi)
//       #4959 shape (b), seen Contains on W1 too  54 / 56 / 56 (W1 first on +0x40)
//       #4959 shape (a), first test on W1 too     46 / 51 / 54
//     So the rule describes the mechanism correctly, but applied to the
//     original's visible reference pattern (W1 read in B1 twice, E1, E3 and
//     S3; the first test and the seen Contains read memory) it puts W1 about
//     12 below los and 4 to 20 below bit. Moving one width read between W1 and
//     los in a seen-arm block swings each by 6 (K = 3 there); a reference in
//     B1 is worth about 16 (K about 8). bit's 5 references (inferred: the def
//     `mov reg, 1; shl reg, cl` counts 3, plus the B1 test and the S3 use)
//     fall mostly in B1, the most crowded block, so a named or CSE'd bit
//     always outranks W1. The only shapes with the original's order give W1 a
//     reference the original does not show. Either the original's IL reaches
//     W1 by a path not found yet (a K or reference difference invisible in the
//     bytes), or something besides the sorted priority decides it:
//     FUN_0045aaf9 can demote a candidate to just below a set's minimum
//     priority before it is coloured (not traced here).
//  C. Flat in this pass: Index() helpers on ByteMap or MapSize for the three
//     index reads, a PlayerBit() inline, helpers taking (x, y) (folds the seen
//     arm to `mov eax, 1`), a named `w` local for the width, `&bit` through a
//     pointer (optimised away), and the explored arm's `m` and direct forms
//     against the r, r0 and if/else seen arms (r0 costs about 27 points).
//     Symbol count (the 0x487080 threshold lever): 0 to 1390 unused `extern
//     int` (step 10) and 0 to 395 one-member structs (step 5) in front of this
//     file never move the LOS choice (30 to 110 externs only cost 18 points
//     elsewhere); from 120 externs on they give the original's cell-pointer
//     fold (`imul eax, [ebp+0x14233]`, 2 bytes shorter, so 71.8% until the
//     LOS block is fixed), and 335+ structs reach shape 92.2% (18 diff blocks
//     against 21) at 72.3%.
//
// claude-opus-5-5 pass (#4959, from 88.5%, no score gain; about 15000 scratch
// compiles scored with check.py's own compare plus a flag per residual site).
// The body is unchanged, but the LOS spill now has a lever. Read this first:
//  0. THE LEVER: IsSeen COMPUTES THE BIT ITSELF AND RETURNS THROUGH A LOCAL.
//     The original keeps the width (W1) in esi and spills `bit` right after the
//     shift; this file keeps bit and spills W1. Out of ~3000 LOS-block shapes
//     (items 1 and 2) the only ones that spill bit (`shl ebp, cl ...
//     mov [esp+0x4c], ebp`, then the seen arm's `mov esi, [esp+0x4c]` reload
//     and `xor eax, eax; jmp` fail path, exactly the original's) have BOTH:
//     IsSeen takes no bit parameter and tests `& (1 << g_game->player)` itself
//     (MSVC CSEs it with main's `bit`, as the sibling 0x4658e0's IsSeen reads),
//     and IsSeen returns through `int r` (`int r = 0; if (Contains) r = ...;
//     return r;` or the if/else form), never through two `return`s. Those
//     shapes also put the `cols` home at [esp+0x18], the original's slot, so the
//     cols item below is downstream of this spill, as the older notes guessed.
//     The closest full shapes so far (each scores lower only because the LOS
//     block is 4 to 12 bytes short, which shifts every later jump):
//      a) 67.8%, 21 blocks, 1327 bytes: main's first test hand-written
//         (`if (x >= los->explored.size.width || y >= ...height) return 0;`),
//         IsExplored = `los->explored.Contains(tx, ty) &&
//         los->explored.data[los->explored.size.width * ty + tx] != 0` with a
//         ByteMap-level `int Contains(int x, int y) { return x < size.width &&
//         y < size.height; }`, IsSeen = MapSize `size.Contains` + `int r = 0`
//         form. This gets the seen arm's memory `cmp edx, [edi+0x80]` AND the
//         explored arm's `mov ecx, esi; cmp edx, ecx` (W1 CSE'd). Still wrong:
//         the first test uses W1 in esi instead of the original's separate
//         `mov ecx, [edi+0x80]` load, the vis test loads the vis pointer before
//         the shift (the original shifts first, into esi, then reloads W1 into
//         esi), the explored Get reassociates (direct read; the method Get
//         loses the bit spill here), and `int r = 0` puts `xor ecx, ecx` at the
//         top of the seen arm instead of in its fail path.
//      b) 69.0%, 20 blocks, 1335 bytes: everything through a `ByteMap* m =
//         &los->explored;` local declared after `Fix hgt;`, helpers taking that
//         `m`, MapSize Contains for main's first test, ByteMap Contains + Get in
//         IsExplored (`return m->Contains(tx, ty) && m->Get(tx, ty) != 0;`),
//         and ByteMap Contains in IsSeen with the if/else `r` form. Its seen
//         arm compares against W1 in esi where the original reads memory;
//         switching IsSeen to `m->size.Contains` brings back the W1 spill and
//         rotates the prologue (63.1%).
//     The open question is which combination keeps bit spilled while the seen
//     arm's Contains stays a memory compare and main's first test is not CSE'd.
//  1. Without the lever the spill is invariant: 1500 random combinations of 12
//     LOS-block dimensions (helpers taking Los* or ByteMap*, Contains and Get as
//     MapSize or ByteMap methods, free inlines or direct reads, early or late
//     seen-arm return, x/y and bit scope and order), the same 1500 again in the
//     128-extern front-end state, all 5040 declaration orders of
//     ok/origin/y0/x0/cols/x/y, hgt's union reused as bit, bit through a
//     pointer, reference or union, `x ? x : x` on W1, bit and y, and
//     int/char/signed player all keep W1 spilled. Use counts alone do not
//     decide it: CSE-ing both explored-arm reads into W1 (4 uses) still spills
//     W1 while bit is a parameter.
//  2. THE EXPLORED ARM'S TWO `mov ecx, esi` ARE IL CSE USES OF W1, not codegen
//     register tracking: with W1 kept in a register (harness with a constant
//     bit) MSVC 5 still reloads [los+0x80] for method-form reads. MapSize-level
//     `size.Contains` reads are never CSE'd with W1 (memory compare, as in the
//     original's seen arm); ByteMap-level Contains and direct reads are. Plain
//     `data[w * ty + tx]` with a CSE'd w reassociates to `(w * ty + data) + tx`;
//     only Get through a ByteMap method keeps the original's index-then-data
//     order.
//  3. FRONT-END STATE: the original's cell-pointer fold (`movsx eax, [esp+0x46];
//     imul eax, [ebp+0x14233]`) appears with 121+ unused `extern int` after the
//     includes, 44+ prototypes or 20+ one-member structs. It saves 2 bytes, so it
//     scores 70-71% until the LOS block (2 bytes longer in the original) is
//     right. No padding kind or count (0 to 640) moves the LOS block, the cols
//     slot, the two guard lea SIB bytes or the loop's mask SIB.
//  4. /Gi fixes the cell fold and the loop's mask SIB but rotates the prologue
//     (40 diff blocks against 21, 54.5%) at every padding count, and it turns the
//     matched neighbour 0x47d820 into 67.9% at every padding count, so this TU is
//     not /Gi. msvc5-rtm is byte-identical. Defining the preceding 0x47d0e0
//     (unannotated) above this function is 82.2% with the LOS block unchanged.
//  5. IsSeen written `if (Contains(tx, ty)) return (...) != 0; return 0;` scores
//     88.7% (20 blocks), but only by moving the shared `ok = 0` block: the
//     original's seen arm has the `jb compute; xor eax, eax; jmp` order that the
//     current `if (!Contains) return 0;` form produces, so it is not taken.
//
// DeepSeek V4.1 Flash pass (from 88.5%, no improvement). tools/stackcmp.py shows
// the same single unused slot (max5b/hgt at +0x14) and no relocated local;
// tools/permute.py 3 min / 3 jobs, 2266 candidates (12 compile failures): 88.5%
// -> 88.5%, best size 1339. Targeted variants, all at 1339 bytes: the sibling
// 0x47d0e0's width-pointer form at the cell statement (88.5), with `index`
// after the cell statement instead of before (88.5), `index` after alone (88.5),
// a `word` local for the vis test (88.5), `bit` first in the vis test (88.5),
// and IsExplored_ with no `bit` parameter (87.9). The LOS-block allocation
// (bit in esi and width spilled to [esp+0x4c], against the original's width in
// esi and bit spilled) is unchanged and remains the whole residual.
//
// Space Bunny Free pass #2 (#4566, from 88.5%, no improvement, ~90 check runs).
// Baseline reproduced exactly: 1339 of 1339 bytes, 88.5%, 91.1% ignoring the
// three jump-target-only lines. The body below is unchanged and is still the
// best measured. What this pass bought is a jump-target-blind differ and an
// isolation harness, which turn the "wall of diff" into ONE coupled allocation
// and then say which two source shapes reach the original's shape and at what
// cost. All of it is in build/scratch/0x47d2e0/: jd.py (the jump-target-blind
// differ), probe.py and mk.py (batch variants of the real file, with our LOS
// block printed next to the original's), dsweep.py (the parallel
// declaration-order sweep) and iso/ (the compile-only isolation
// harness, with its own sweep.py).
//
//  1. THE RESIDUAL IS ONE DECISION, NOT A LIST. jd.py diffs the original
//     against ours with every address inside the function replaced by a label,
//     so a branch target compares equal however the bytes around it moved. That
//     turns the 88.5% residual from a wall into 64 lines, and they collapse to
//     ONE thing: 37 of the 64 lines are the LOS block and its two arms, and
//     every other site (the `cols` home slot, the two guard `lea` SIB bytes,
//     the cell-index fold, the `min6` store position, the mask pointer's
//     ecx/edx) is downstream of the same register rotation. Sites worth their
//     own lines: 3 + 2 + 4 + 5 + 2 = 16, and all of them are consequences of
//     which register the LOS width lands in.
//
//  2. WHAT THE LOS BLOCK ACTUALLY DECIDES, spelled out. The original spills
//     `bit` to the dead `los` slot and reloads it five instructions later:
//         mov ebx, g_game / mov esi, 1 / xor ebp, ebp / mov cl, [ebx+0x2a43]
//         shl esi, cl / mov ecx, [ebx+0x14273] / mov [esp+0x4c], esi
//         mov esi, [edi+0x80] / mov ebx, esi / imul ebx, eax / add ebx, edx
//         mov bp, [ecx+ebx*2] / mov ebx, [esp+0x4c] / test ebp, ebx / je
//     and we common-express the width instead, into ebp, spill it to the SAME
//     slot, and keep `bit` in esi:
//         mov ebx, g_game / mov ebp, [edi+0x80] / mov esi, 1
//         mov [esp+0x4c], ebp / mov cl, [ebx+0x2a43] / shl esi, cl ...
//         xor ebp, ebp / mov bp, [ecx+ebx*2] / test esi, ebp / je
//     Everything downstream follows: with the width in esi the IsExplored arm
//     reuses it (`mov ecx, esi / cmp edx, ecx`, `mov ecx, esi / imul ecx, eax`)
//     and the IsSeen arm consumes it (`imul esi, eax`), which is why the
//     original's two arms have their own failure blocks and ours merge
//     IsExplored's failure with IsSeen's. So the two arms are not a second bug.
//     There are 7 registers and the block needs 8 live values, so one value
//     must be spilled; the original spills `bit` (used once here, once in the
//     IsSeen arm) and we spill the width.
//
//  3. THE BRIEF'S ITEM 18 (the lever that closed the sibling 0x47d0e0 at 96.1%)
//     IS INERT HERE. Eight variants, all 88.5% and byte-identical to the file:
//     `int w = g_game->width; int* pw = &w;` at the cell statement, the plain
//     `int w` alone, each of those with the cell statement before and after
//     `int index = 0;`, and a plain local in place of the pointer. The plain
//     local alone does nothing here exactly as the sibling recorded, but the
//     pointer does nothing either, so the lever does not transfer from a
//     function whose multiply sits in its first basic block. Also measured and
//     flat: the LOS width through a pointer to an unmodified local (69.6%, so
//     it does something, just the wrong thing), the LOS width as a plain local,
//     `&g_game->width` taken directly (45.6%), the stride as a `const int&`
//     (45.6%), a `Cell* cells` local (22.0%), the index through a named int
//     (42.6%), `cell` read through `&cell` (88.5%), a `short cy` local (63.2%).
//
//  4. AN ISOLATION HARNESS NAMES THE TWO SHAPES THAT REACH IT, AND BOTH COST
//     30 POINTS ON THE REAL FILE (build/scratch/0x47d2e0/iso/). A ~200-byte
//     cut-down body reproduces the same choice, so it iterates in about three
//     seconds instead of a minute, and the original's own block is read out of
//     the exe as the target (so `d` there is a real distance, but only `check`
//     scores the function).
//     a) THE HELPERS TAKE `(tx, ty)`, not `(pos, hgt)`. The original DEMANDS
//        this: the bit spill at 0x47d3df writes four bytes at [esp+0x4c], which
//        covers the high word of `hgt` at [esp+0x4e], and neither arm rereads
//        it, so the arms cannot be receiving `&hgt`. With this signature the
//        harness emits `xor ebp, ebp` EARLY, where the original has it (the
//        file emits it late), and folds the width into the imul. On the real
//        file: 57.6%, because pos and hgt die early and the whole prologue
//        rotates (`mov edi, g_game` for `mov ebp, g_game`, `mov si` for
//        `mov di`, `movsx ebp, ax` for `movsx esi, ax`, and the `cols` home
//        moves to [esp+0x10]). Re-swept on top of it, as the brief's item 17
//        asks, nine ways: x0/y0 order 58.1, pos/hgt at function scope 57.6,
//        `cols` early 56.5, `cols` removed 57.0, all of it byte-identical or
//        worse. So this is a real fix to the block and a real loss to the
//        prologue, and the two are one decision again.
//     b) THE CONTAINS GUARD HAND-WRITTEN as `x >= w || y >= h` (or the negated
//        `!(x < w && y < h)`) instead of the member call. In the harness this
//        keeps the width in a REGISTER across the multiply with a reg,reg
//        `imul ecx, ebx`, which is the original's `mov ebx, esi / imul ebx,
//        eax` shape, and the `xor` lands late as ours does. On the real file:
//        55.6% (identical for all three spellings), again because the prologue
//        rotates. The cross of (a) and (b) is 56.7%.
//     So both halves of the original are reachable from source, and neither is
//     reachable together with the prologue this file has. That is the finding:
//     the residual is one allocation decision that no single edit here moves.
//
//  5. MEASURED FLAT AT 88.5% ON TOP OF THE FILE BELOW (byte-identical unless a
//     number is given). The two guard `lea` SIB operand orders: the original
//     has `lea ecx, [esi+edx]` (base = origin.x) and `lea edi, [ecx+eax]`
//     (base = origin.y), so both sums should be spelled origin-first, and
//     `origin.x + x0` / `origin.y + y0`, each alone and both together, and both
//     together with x0/y0 swapped, are all byte-identical. So the SIB byte is
//     NOT expression-driven here, which settles the open question in the notes
//     below rather than repeating it. The vis test: the index through a named
//     int, through a `unsigned short* vis` local, through a `unsigned short v`
//     local, the index computed before the bit, `bit & word`, `0 == (word &
//     bit)`, `!(word & bit)`: all 88.5%. The bit: `int`, `unsigned long`, split
//     into `unsigned int b = 1; b <<= g_game->player;`, through a `static inline
//     unsigned int Bit_(unsigned char)` helper (87.9), through a pointer to an
//     unmodified local (88.5), declared at the top of the LOS block and
//     assigned after the test (88.5), assigned before the Contains test (86.9).
//     Local set: x and y declared in the LOS block, x and y removed with the
//     expressions inlined, `cols` removed, `cols` declared next to the loop
//     locals, `index` after the cell statement, `ok` declared in the LOS block
//     (87.9), `hgt` at function scope (87.7), `hgt` and `bit` both at function
//     scope (87.7), one extra unused `int` (88.5), `&cols` taken and never
//     dereferenced (byte-identical: the address is optimised away). `&ok` taken
//     is 63.8 and is the clearest sign this file's allocation is already at a
//     local optimum. The zero-initialisers: `min6`, `max5`, `max5b`,
//     `found80` declared without an initialiser and assigned after the cell
//     statement, each alone and all together, in the order the original emits
//     them and in source order: 87.4 to 88.2, and moving `max5` or `max5b`
//     alone is 70.0 and 70.8. That is the fourth site: the original's
//     `or dl, 0xff` and its `mov byte [esp+0x14], dl` sit after the two leas
//     because the leas take edx there, and ours takes ecx, so edx is free and
//     the store moves up. Writing the initialisers as late statements does move
//     it and costs 0.6, because the leas then take ecx anyway.
//
//  6. THE `static inline` PREDICATE RETURN TYPE, MEASURED PER FUNCTION AS THE
//     BRIEF SAYS (item 31: three functions, three answers, never carry it
//     across). Around the Contains guard, taking `(MapSize*, unsigned, unsigned)`:
//     `int` is 87.7% and `bool` is 69.8%. Around the vis test, taking
//     `(vis, w, tx, ty, bit)`: `int` is 87.9% and `bool` is 70.1%, the `bool`
//     form materialising into `mov ebx, ebp / and ebx, esi / neg / sbb / neg /
//     test bl, bl / je` where the original has `test ebp, ebx / je`. So `int` is
//     the better return type in both places here, by 0.6 and 0.6 points, and
//     neither reaches the block. That is a fourth data point for the rule.
//
//  7. DECLARATION ORDER: MEASURED, AND THE AXIS IS LIVE HERE, unlike 0x459200
//     (24 orders) and 0x448c70 (151). A random sweep of the nine declarations
//     between the guard and the footprint loop, seed 4566, 500 permutations all
//     scored with check.py: the range is 67.6% to 88.5%, so order matters a
//     great deal on this function, and THE FILE'S OWN ORDER IS THE BEST OF
//     ALL 500 (identity, 88.5%, 1339 bytes). 35 permutations tie at 88.5%, the
//     next best is 88.2% and then 87.9%. This is the one sweep in the file that
//     had never been run at full size, and it closes that axis.
//
//  8. `tools/permute.py`, re-run against this 88.5% body (15.0 min, 7445
//     candidates, 48 did not compile, 36 duplicates, 36 mutation families):
//     88.5% -> 88.5%, score 1572 -> 1572, best_size 1339. Its `include` family
//     is the only one that finds anything at all (2 improvements out of 468, and
//     the best reverts), and its `move_decl`, `move_stmt`, `negate_if`,
//     `temp_inline` and `extract_helper` families each found one or two hill
//     climbs that did not survive. Its best.cpp is the file below. So the
//     permuter's mutation space does not contain the LOS fix, which is
//     consistent with item 3: the pointer form is not a spelling it generates.
//     Confirmed afterwards that permute restored src/unsorted/0x47d2e0.cpp, by
//     hashing it against the scratch copy taken before the run.
//
// WHAT THE NEXT PASS SHOULD LOOK AT, in priority order:
//  * the LOS width's home. Everything else in the residual follows from it.
//    Both source shapes that reach it (the `(tx, ty)` helpers and the
//    hand-written guard) rotate the prologue, so the useful question is what
//    keeps `mov ebp, g_game`, `mov di` and `movsx esi, ax` in the prologue
//    while giving the width a register. The prologue's three registers are
//    decided before the LOS block is reached, so a lever that adds pressure
//    only inside the LOS block cannot be the answer; something that changes the
//    prologue's own choice has to be found, and nothing in this file's local
//    set does.
//  * the `cols` home slot, [esp+0x18] against [esp+0x04]. Both files put min6
//    at 0x04 and max5 at 0x08, and the original's 4-byte `cols` copy lands in
//    the 0x08 hole with max5 while ours lands in the 0x04 hole with min6. The
//    declaration-order sweep above permutes min6, max5 and max5b freely and
//    never moves it, so the slot is not chosen by declaration order here.
//  * the two guard `lea` SIB bytes are settled: not source-expressible.
//  * the `min6` store position is settled: it follows from which register the
//    two leas take, which follows from the LOS rotation.
//
// Can a unit's footprint stand on the map cell `cell`? The guards are the map
// bounds, then the two visibility tests (seen on the shared bit mask, or on the
// player's explored byte map when flag 2 of g_game+0x14281 is set), then a walk
// of the footprint cells that accumulates the build cost into DAT_0051e688 and
// the height envelope into the returned DAT_0051e684.
//
// PARTIAL 88.5% (1339 of 1339 bytes, exact size; 82.0% before the pass below,
// and unchanged by the second pass at the top, which bought precision rather
// than points).
//
// space-bunny-free pass (#4566, from 82.0%): the file's own note called the
// prologue's esi/edi choice "a colour tie-break inside MSVC 5's LCL, not a
// source-order effect" and made it the headline residual. That was wrong, and
// finding out why is worth the pass: it was the header set. Two levers, both
// re-checkable in one compile each.
//
// WHAT THIS PASS FOUND, in the order it paid:
//  1. THE PROLOGUE'S esi/edi TIE-BREAK WAS FRONT-END STATE, NOT THE SOURCE.
//     The note below called it "a colour tie-break inside MSVC 5's LCL" and
//     measured it as the dominant item. It is not: it is the header set.
//     `#include <string.h>` + `#include <math.h>` in front of the file (nothing
//     from either is used) makes `mov di, word ptr [esp+0x46]` appear where the
//     original has it, and with it origin.x in esi, `los` in edi, g_game in ebp
//     and every later use of those three, which is what the whole 82% plateau
//     was made of. 82.0 -> 86.9 with the header alone. tools/headers.py (all
//     1536 sets) puts four sets at that 86.9 and no set higher; <stdio.h>
//     <stdlib.h>, <string.h> <math.h>, <stdio.h> <string.h> <memory.h> and
//     <string.h> <math.h> <memory.h> <minmax.h> are the four. This is the same
//     front-end-state lever the permuter result recorded as "#include <math.h>
//     with nothing from math.h used, worth 6%", and the same one 0x47d820's
//     notes describe as a symbol-hash coin flip. <windows.h> is NOT it: it makes
//     the frame 1392 bytes. With the header in place the residual is small and
//     specific (below), unlike the register-allocation fog it was before.
//  2. `bit` INITIALISED AFTER THE Contains TEST, not before it. The old notes
//     say "bit declared BEFORE the Contains test, worth several points through
//     register allocation only", and that is true of the 82% baseline; with the
//     header set it inverts. Declaring `unsigned int bit = 1 << g_game->player;`
//     after the `if (!los->explored.size.Contains(x, y)) return 0;` puts the
//     shift after the test, which is where the original has it, and stops bit
//     living in a register across the whole block: 86.9 -> 88.5. Declaring it at
//     the top of the function and assigning it there scores the same 88.5, and
//     so does putting it after the `& bit` test, so the order of the two tests
//     is free.
//  3. The header sweep on THIS file: `<string.h>` alone and `<math.h>` alone
//     both reach 88.5 and are byte-identical to the pair, so the two are the
//     same front-end state; `<memory.h>`, `<crtdbg.h>`, `<cstring>` (87.7),
//     `<exception>` (87.4) and every other real header in the VC5 include
//     directory is equal or worse, including `<windows.h>` (1392 bytes). tools/
//     headers.py on this file: four sets at 88.5, no set higher. Both the
//     declaration-count probe from 0x47d820 (N = 0..64 unused `extern int`,
//     `extern void __cdecl f(void)`, `extern int __cdecl f(int,int)`, `typedef`,
//     `static int` and one `struct` per line) and the symbol-hash probe (the
//     two LOS helpers, the two terrain helpers and Cell/Los/ByteMap/MapSize
//     each renamed, the helpers swapped, the terrain helpers moved above them,
//     the extern globals reordered) are NEGATIVE for this function: 103
//     declaration-count variants and 20 renaming variants are 88.5 or worse, so
//     unlike 0x47d820 and 0x47d0e0 this file's residual is not front-end state.
//  4. Measured flat at 88.5 or worse on top of the file below (each one is
//     byte-identical unless a number is given): the header sets <stdio.h>
//     <string.h> (86.4), <memory.h> (81.5), <string.h> (81.2), <minmax.h>
//     (81.2); the declaration order of x0/y0 (both orders), x0/y0/cols before
//     `Point origin` (81.5-81.7), cols before y0/x0 (80.4), `int ok` after the
//     guard (81.5), `ok = 1` before the guard or initialised at the top
//     (88.2), `unsigned int ok` (64.6), `ok = 1` after the LOS block (45.8);
//     the DAT pair chained, after the guard (87.4), `if (los)` for
//     `if (los != 0)`; pos, hgt, bit, x/y moved into or out of the LOS block
//     (58.7 if bit is initialised in the top block); all 15 permutations of
//     the nine loop locals' declaration order that keep the initialisers with
//     their declarations; min6/max5/max5b declared in the top block (41-44),
//     as `char` (43.3), `int` (63.1), `short` (65.3), comma-separated or in
//     a different order; the cell index as `cell.x + cell.y * width`,
//     `width * cell.y`, `cells + y*w + x`, an `int` index local, `(int)` and
//     `(unsigned)` casts, a `Game*` local or an `int mw` width local (43.6);
//     the vis test through an `unsigned short*` local, `!(v & bit)`, a `v`
//     local, `y * width + x`, a `MapSize*` local, a `w` local; the LOS arms
//     as a ternary (56.4), with the two tests swapped (86.9), with IsSeen
//     first (60.4), with `(flags & 2) != 2` (86.9), with a flag local;
//     the helpers taking the bit by pointer, `hgt` by value, `pos` by value,
//     `los` by reference, a `ByteMap*` (66.8), a `MapSize*` plus data (63.2),
//     or `(data, width, height)` by value (50.7), as static members of a
//     wrapper struct (87.9) or of Game (82.0); `Contains` as a free function
//     (55.1), `Get` returning `int`, `Contains` with a redundant `0 <=`;
//     `Bit()` and `Clamp()` helpers, a third unused inline helper; `static
//     unsigned int bit` (71.0, frame 1360), `unsigned short bit` (69.5),
//     `int bit`, bit used twice; and the braces-around-a-statement lever on the
//     Contains test and the vis test.

// WHAT IS LEFT, four independent items, in rough order of diff lines. (The
// second pass at the top of this file keeps the same four, but shows with a
// jump-target-blind differ that they are ONE decision rather than four, and
// settles the third one's SIB half outright: both expression orders of the two
// guard sums compile byte-identically, so `lea ecx, [esi+edx]` against
// `[edx+esi]` is not source-expressible. It also runs the declaration-order
// sweep at full size for the first time, 500 permutations, and the file's own
// order is the best of all of them.)
//  * The home slot of the `cols` copy of origin.x. The original stores esi
//    (origin.x) to [esp+0x18], frame offset 8, and reloads it from there after
//    the LOS block; we store to [esp+0x14], frame offset 4. Offset 4 belongs to
//    min6 and offset 8 to max5 in BOTH files, so this is not a different set of
//    slots, it is which byte local MSVC lets the 4-byte cols copy share: theirs
//    shares with max5 (declared after it, at 0x08), ours with min6 (0x04). A
//    declaration-order sweep that moves min6's slot did not move this, because
//    every attempt also moved the `min6 = 0xff` store out of the block the
//    original has it in.
//  * The LOS block keeps the map width in a register: ours hoists
//    `los->explored.size.width` into ebp before the vis index and spills it to
//    the dead `hgt` slot [esp+0x4c], so the slot holds the width and `bit`
//    stays in esi; the original spills `bit` to that slot and reloads it five
//    instructions later (`mov ebx, [esp+0x4c]; test ebp, ebx`), which forces
//    esi to be free, which is why its second index multiply reuses esi
//    (`imul esi, eax`) where ours uses eax off the spilled width.
//  * The cell pointer: the original folds the width into the multiply
//    (`movsx eax, [esp+0x46]; imul eax, [ebp+0x14233]`), we load the width
//    into eax first and register-multiply (`mov eax, [ebp+0x14233]; imul
//    eax, ecx`). 0x47d0e0 has this exact eight-instruction block and settles
//    the cause with an isolation harness: what decides the fold is WHICH
//    REGISTER the allocator gives the sign-extended short. Here it is ecx and
//    there it is eax, so this item, the SIB operand order of the two guard
//    `lea`s (`lea ecx, [esi+edx]` vs `[edx+esi]`, both operands are already in
//    registers there, and both expression orders compile identically) and the
//    ecx/edx choice for the byte temp and the mask pointer in the loop are one
//    allocator decision, not four source bugs. 0x47d0e0 records that ~90 index
//    spellings and two permuter runs do not move it, and this pass is the same
//    result for the same expression (15 spellings here, all byte-identical or
//    worse).
//  * The inner loop's byte temp and mask pointer get ecx in the original and
//    edx here, so `unit` gets edx there and ecx here, and the mask load's SIB
//    byte comes out `[edi+ecx]` (index in the base slot) here as
//    `[edx+edi]`. That is the exact residual 0x47d820 is stuck on, in the same
//    family and with the same `unit->mask[i++]` source, so it is likely the
//    same one-instruction SIB coin flip rather than a spelling.
//
// CARRIED OVER FROM THE EARLIER PASSES, still load bearing:
//  * The ground height is NOT pos.y. The original stores
//    `FUN_00485010(&cell) << 16` into the dead `los` home slot [esp+0x4c] and
//    reads it back with `movsx word [esp+0x4e]`, so it is a separate `Fix`
//    local declared beside the Pos, and both inline helpers take it as a third
//    `Fix*` argument.
//  * Both inline helpers take the player bit as a fourth argument, which stops
//    MSVC re-deriving `1 << g_game->player` inside IsSeen (69.3 -> 80.2), and
//    read the position through a six-short `Position` cast.
//  * `los` is the neighbours' Map: `explored` = ByteMap {data,
//    MapSize{width, height}} with MapSize::Contains and ByteMap::Get.
//  * The loop's rr/terrain tests are early-return inline helpers (Blocked_,
//    Terrain_), which reproduces the `xor ecx,ecx; jmp join` ladders exactly.
//  * The second visibility test is NOT folded to ok = 1 when it goes through an
//    inline helper that re-derives tx/ty from &pos (IsSeen_/IsExplored_) while
//    the first test is written by hand (Contains, then
//    `(vis[w*y+x] & bit) == 0`); a helper taking (los, x, y) is folded again.
//  * The bounds guard is one combined `if` with `short y0` and `short x0` read
//    off the by-value Point, which is what puts `movsx edx, cx` before the
//    width load.
//  * The footprint loop is the rotated form with an explicit outer guard and a
//    POSITIVE bottom test. A plain `for` gives 80.9 (`cmp ecx, edx / jg`),
//    `while (1) { ... if (origin.y <= row) break; }` 81.5, a bare `do/while`
//    57%.
//
// The permuter reached 91.0% on the 82% base; every one of its wins was a
// one-instruction scheduling nudge that did not survive on its own, plus six
// one-line helpers that just return a member, `unsigned int ok`, a
// `goto skip0/skip1/skip2` ladder and `if (1) do {} while (1)`. Its useful
// content was the header, and that is now in the file below, which took its
// score. Re-run on the 88.5% file below (22 min, 9768 candidates, 81 compile
// failures): NO improvement, its best is the same 88.5% (and its best.cpp is
// only cosmetically different plus one more equivalent header), so 88.5% is a
// local optimum of the permuter's mutation space and not just of the spellings
// in the list above.
#include <string.h>
#include <math.h>

#pragma pack(push, 1)

union Fix_0047d2e0 {
    int v;
    struct { short lo; short hi; } p;
};

struct Point {

    short x;
    short y;
};

struct Cell_0047d2e0 {
    short field_0;
    char unknown_2[0x5 - 0x2];
    unsigned char field_5;
    unsigned char field_6;
    unsigned char field_7;
    short field_8;
    unsigned char field_a;
    unsigned char field_b;
    unsigned char field_c;
};

struct Unit_0047d2e0 {
    char unknown_0[0x14a];
    Point origin;
    unsigned char* mask;
    char unknown_152[0x1be - 0x152];
    short field_1be;
    short field_1c0;
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char field_228;
    char unknown_229[0x22c - 0x229];
    unsigned char field_22c;
};

struct MapSize_0047d2e0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};

struct ByteMap_0047d2e0 {
    unsigned char* data;
    MapSize_0047d2e0 size;
    int Contains(int x, int y) { return x < size.width && y < size.height; }
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Los_0047d2e0 {
    char unknown_0[0x7c];
    ByteMap_0047d2e0 explored;
};

struct Game_0047d2e0 {
    char unknown_0[0x2a43];
    unsigned char player;
    char unknown_2a44[0x14233 - 0x2a44];
    int width;
    int height;
    char unknown_1423b[0x14253 - 0x1423b];
    int field_14253;
    char unknown_14257[0x1426f - 0x14257];
    unsigned char* field_1426f;
    unsigned short* field_14273;
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;
    char unknown_14280[0x14281 - 0x14280];
    unsigned char losFlags;
    char unknown_14282[0x14287 - 0x14282];
    Cell_0047d2e0* cells;
};
#pragma pack(pop)

extern Game_0047d2e0* g_game;
extern int DAT_0051e684;
extern int DAT_0051e688;

int __stdcall FUN_00485010(Point* p);

struct Pos_0047d2e0 {
    Fix_0047d2e0 x, y, z;
};

struct Position_0047d2e0 {              // 16.16 fixed point, only high words read
    short xFrac;
    short x;
    short yFrac;
    short y;
    short zFrac;
    short z;
};

static inline int IsExplored_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    ByteMap_0047d2e0* m = &los->explored;
    if (m->Contains(tx, ty) && m->Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt, unsigned int bit)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    int r;
    if (!los->explored.size.Contains(tx, ty))
        r = 0;
    else
        r = (g_game->field_14273[los->explored.size.width * ty + tx] & bit) != 0;
    return r;
}

static int Blocked_0047d2e0(Cell_0047d2e0* c)
{
    unsigned short v = c->field_8;
    if (v == 0xffff)
        return 0;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 1;
        return (g_game->field_1426f[v * 0x100 + 0xfe] >> 6) & 1;
    }
    if (v != 0xfffe)
        return 1;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return (g_game->field_1426f[v2 * 0x100 + 0xfe] >> 6) & 1;
}

static unsigned char* Terrain_0047d2e0(
Cell_0047d2e0* c)
{
    if (c == 0)
        return 0;
    unsigned short v = c->field_8;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 0;
        return g_game->field_1426f + v * 0x100;
    }
    if (v != 0xfffe)
        return 0;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return g_game->field_1426f + v2 * 0x100;
}

// FUNCTION: 0x47d2e0
int __stdcall FUN_0047d2e0(Unit_0047d2e0* unit, Point cell, short type, Los_0047d2e0* los)
{
    int ok;
    DAT_0051e684 = 0;
    DAT_0051e688 = 0;
    Point origin = unit->origin;
    short y0 = cell.y;
    short x0 = cell.x;
    if (x0 < 1 || y0 < 1 || x0 + origin.x >= g_game->width ||
        y0 + origin.y >= g_game->height)
        return 0;
    int cols = origin.x;
    int x;
    int y;
    ok = 1;
    if (los != 0) {
        Pos_0047d2e0 pos;
        Fix_0047d2e0 hgt;
        pos.x.v = (origin.x + cell.x * 2) << 19;
        pos.z.v = (origin.y + cell.y * 2) << 19;
        hgt.v = FUN_00485010(&cell) << 16;
        x = pos.x.p.hi >> 5;
        y = (pos.z.p.hi - (hgt.p.hi >> 1)) >> 5;
        if (!los->explored.size.Contains(x, y))
            return 0;
        unsigned int bit = 1 << g_game->player;
        if ((g_game->field_14273[los->explored.size.width * y + x] & bit) == 0)
            return 0;
        if ((g_game->losFlags & 2) == 2)
            ok = IsExplored_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt);
        else
            ok = IsSeen_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt, bit);
    }
    unsigned char min6 = 0xff;
    unsigned char max5 = 0;
    unsigned char max5b = 0;
    int found80 = 0;
    int foundFE20 = 0;
    int index = 0;
    Cell_0047d2e0* c = &g_game->cells[cell.y * g_game->width + cell.x];
    int row;
    int col;
    row = 0;
    if (origin.y > row) {
        do {
            for (col = 0; col < cols; col++) {
                DAT_0051e688 += c->field_7;
                int m = unit->mask[index++];
                if (m & 8) {
                    if (c->field_6 < min6)
                        min6 = c->field_6;
                    if (c->field_5 > max5)
                        max5 = c->field_5;
                }
                if ((m & 0x10) && c->field_5 > max5b)
                    max5b = c->field_5;
                if ((m & 1) && (c->field_c & 2) && ok)
                    return 0;
                if ((m & 6) && c->field_0 != 0 && c->field_0 != type && ok)
                    return 0;
                if (m & 0x20) {
                    if (Blocked_0047d2e0(c) != 0)
                        return 0;
                }
                if (m & 0x40) {
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xff] & 2))
                        return 0;
                }
                if (m & 0x80) {
                    found80 = 1;
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xfe] & 0x20))
                        foundFE20 = 1;
                }
                ++c;
            }
            row++;
            c = (g_game->width - ((int)cols)) + c;
        } while (row < origin.y);
    }
    if (found80 && !foundFE20)
        return 0;
    unsigned char r;
    if (max5 < min6) {
        r = g_game->seaLevel - unit->field_22c;
    } else {
        if (max5 - min6 > unit->field_228)
            return 0;
        r = min6;
    }
    if (max5b > r)
        return 0;
    if (min6 < g_game->seaLevel - unit->field_1be)
        return 0;
    if ((max5 > max5b ? max5 : max5b) > g_game->seaLevel - unit->field_1c0)
        return 0;
    DAT_0051e684 = r;
    return 1;
}
