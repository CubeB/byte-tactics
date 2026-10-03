// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free,
// claude-opus-5-5 (#4373), GPT-6.1-sol, mimo-v2.6-pro, space-bunny-free (#4489)
// and space-bunny-free (#4539), space-bunny-free (#4652), Space Bunny Free (#4688),
// finished by DeepSeek V4.1 Flash (issue 4875), finished by GPT-6.
// Names are provisional.
// GPT-6 retry (#4977): best remains 89.8%. classify.py routes this as slots
// with six register differences. /Gi and a grid-y pointer local are byte-flat.
// DeepSeek V4.1 Flash pass (issue 4875): best stays 89.8% (6 differing bytes,
// 0x4ac910/0x4ac913 in the prologue and 0x4ac926/0x4ac92f/0x4ac936/0x4ac945 in
// the loop). Measured with a byte scorer over about 60 fresh variants
// (build/scratch/0x4ac8c0/harness.py, batch.py, run_pro*.py, run_loop.py,
// run_cond.py), all new, do not repeat:
//  - the 24 rect store orders still hoist the ebx-derived pair; only
//    left,top,right,bottom and its three rotations keep 6 bytes, the rest 9 to 11.
//  - member forms (right = p.r.left + 7, bottom = p.r.top + 7), right/bottom
//    temps, 7 + y, swapped sums, (x - x) dependencies, int/short gy temps,
//    merged `gadgets->y + grid->y` and `gadgets[index].y` forms, pointer-cast
//    loads, and prologue statement orders (gx before/after s, surface direct,
//    surface after x0, y before the x0 load, add-before-surface, both adds
//    reversed) are all inert at 6 bytes or a size shift (166 to 181).
//  - the COND on the two x stores stays the only flip: conditions edi, col, row,
//    y, x + 1, !col, col == 0, and x alone all keep a real branch, 180 to 194
//    bytes; only the folded W(a,b) = a ? b : b reaches the esi hoist, and it
//    still moves the surface reload to rank 1 (state B, 18 bytes).
//  - tools/permute.py default seed and seed 123, 3 minutes and 3 jobs each
//    (2562 and 2590 candidates): 89.8% to 89.8%, empty best.diff.
// Still differs: grid->y goes into eax where the original uses edx, and the loop
// hoists (y + 7, bottom) where the original hoists (x + 7, right). No spelling
// tried reaches both.
// Space Bunny Free pass (issue 4688): best stays 89.8% (167/167 bytes, 6 differing
// bytes). Nothing beat it. The useful new result is the exact byte map of the six
// bytes (dbytes.py prints them): 0x050 and 0x053 are the destination register of
// the prologue's grid->y load and of the add that consumes it, and 0x066, 0x06f,
// 0x076 and 0x085 are, in the loop, the lea ModRM byte, the hoisted store's stack
// displacement, the second lea's ModRM byte and the second store's displacement.
// So the loop hunk is exactly "which of the two +7 values the scheduler puts in
// the hoisted (lea + store) pair", and nothing else: the instruction order, the
// eax/ecx choice for the two leas, the three pushes and the three plain stores
// are already in the original's slots.
// Measured with a split metric (prologue hunk vs loop hunk, build/scratch/0x4ac8c0/
// h.py score_split and sw.py), all new, do not repeat:
//  - THE LOOP'S TIE HAS TWO INDEPENDENT HALVES, and a deliberately wrong diagnostic
//    separates them. With `p.r.right = y + 7; p.r.bottom = x + 7;` (a lie about
//    the values, only a probe) the loop hunk falls from 4 bytes to 2: the store
//    displacements then land right (right at 0x1c, bottom at 0x20) because the
//    hoisted pair belongs to the `right` statement, and only the two lea ModRM
//    bytes are left. Conclusion: which STATEMENT the hoisted pair belongs to
//    follows the two +7 statements, and which VALUE it computes is always the one
//    whose operand sits in ebx. With the honest values the hoist therefore always
//    lands in the `bottom` slot, so while x stays in esi and y in ebx the loop
//    hunk cannot go below 4 bytes. Matching needs the compiler to hoist the esi
//    operand, and only the separate-rect shape does that, at a cost of 18 bytes of
//    prologue. This also re-confirms, in one run, that the hoist follows ebx in
//    all 24 store orders (only 4 of the 24 reach 6 bytes; the best are left, top,
//    right, bottom and its three rotations that keep that relative order).
//  - THE RECT'S LOCALITY IS THE SWITCH THAT FLIPS BOTH TIES, not the surface's.
//    Isolated this pass: `Pair p` with the `void* s` temp and `Pair p` with
//    `p.surface = gadgets->surface` compile byte-identically (6 bytes), and
//    `void* surface; Rect r;` with and without `s` are byte-identical too (34), so
//    earlier passes' "the surface in a local of its own" is really about the rect.
//    Five shapes measured at the SAME frame offsets (surface 0x10, &r 0x14):
//    the Pair aggregate, surface in a one-member struct with the rect separate,
//    rect in a one-member struct with the surface separate, two separate locals,
//    and the base. The aggregate is 6 (pro 2, loop 4); all four others are 34
//    (pro 18, loop 16), so there is no intermediate: rect separate gives the esi
//    hoist and the other prologue at the same time.
//  - THE PROLOGUE'S grid->y REGISTER IS MOVABLE, but only together with that
//    load's rank, which is why earlier passes called it immovable. Dropping the
//    `int gx = gadgets->x;` temp (writing `x0 += gadgets->x;`), or moving
//    `p.surface = s;` ahead of the gx temp, puts `movsx edx, word ptr [eax + 0x15]`
//    in the original's register, and in every such shape the scheduler then emits
//    that load BEFORE `add esi, ecx` instead of after it: prologue 14, total 30.
//    24 statement orders of {gx temp, p.surface = s, x0 += gx, y += grid->y} and
//    24 of {p.surface = s, x0 += gadgets->x, y += grid->y} were compiled: exactly
//    one order in the first group (`p.surface = s;` first, g_pgey) moves the
//    register, and no order holds the register while holding the rank.
//  - TWIN TEST, SHARP FORM, AND IT IS DECISIVE NEGATIVE: with every immediate and
//    displacement masked out (opcode, ModRM, SIB and register bytes exact), the
//    original's 38-byte inner loop (0x4ac925..0x4ac94b) and its 18-byte post-call
//    block each match exactly ONE place in the whole of .text: the function
//    itself (twin.py). So there is no matched sibling anywhere in the exe to read
//    the source spelling off, and this is not a shape the compiler emits twice.
//  - 24 spellings of `y += grid->y` all keep grid->y in eax (y = grid->y + y,
//    y = y + grid->y, gadgets[index].y, a short or int gy temp, (*grid).y,
//    *(short*)grid, *(short*)((char*)grid + 0x15), (short) casts, y += grid->y + 0,
//    y *= 1 forms, and the same with x0 unsigned or long); only the merged
//    one-statement sums move it, and those are 46 bytes.
//  - all 6 bytes, do not repeat: x+7 against 7+x, x+4+3, x+8-1; the member forms
//    right = p.r.left + 7 and bottom = p.r.top + 7, either one alone and both;
//    named temps for right and for both; col + row * 16 and a 0x10 loop bound and
//    an unsigned row; x recomputed as x0 + col * 8 or x0 + (col << 3) instead of
//    an induction variable; int x at function scope assigned at the top of the
//    outer body (the bottom-of-body form is 134); a Rect& bound to p.r; a Rect*
//    per iteration; the four stores in a nested block; one comma statement;
//    unsigned x and y; an inline Plus7 helper; int x = x0 with col in the inner
//    for-init; for (int row = 0, x = x0; ...); x = x + 8; x += 8, col++; a
//    do-while and a while inner loop; for (int row = 16; row--; ). The Pair
//    declared in the outer body is 94 and in the inner body 121, so function
//    scope is what this one needs.
//  - deleting locals: `&gadgets[FUN_0049fdf0(gadgets, "COLS", 6)]` with no index
//    variable is byte-identical; one merged `int x0 = grid->x + gadgets->x;` is
//    97. The MATCHed sibling 0x4ac970 needs its CellX helper to be DEFINED even
//    when the sum is written out inline, and that effect does not reproduce here:
//    CellX called is 81, CellX merely defined and uncalled is 6, as is CellY.
//  - the faithful "the other operand first" spellings, `int y = grid->y; y +=
//    gadgets->y;` and `int x0 = gadgets->x; x0 += grid->x;` crossed both ways and
//    with and without the gx temp, are 106 to 119 bytes.
//  - tools/headers.py re-run this pass: 256 header sets, 0 compile failures, all
//    89.8%, so the older "all 128 header sets stay 89.8%" notes are right in
//    substance (the tool now tries 256, not 128).
//  - tools/permute.py against a scratch copy of the file: seed 41 (8 minutes, 1401
//    candidates), seed 57 (12 minutes, 5152) and seed 91 (10 minutes), all 89.8%
//    to 89.8% with an empty build/permute/0x4ac8c0/best.diff, which now makes ten
//    permuter runs on this file across the project.
// Harness in build/scratch/0x4ac8c0: h.py scores a source file by differing bytes
// (d=0 means byte-identical, NOT that check.py reports MATCH; dbytes.py prints the
// exact offsets), sw.py runs a list of variants in parallel with the prologue and
// loop hunks split at offset 0x58, twin.py is the masked twin search, and t1 to
// t13 are this pass's sweeps. About 190 free-scored variants, no check.py run was
// needed to rank them.
//  - the object-identity lead is CLOSED, not open: writing the rect through an
//    address cast, `(Rect*)((char*)&p + 4))->left = x`, and through `(&p.r)->left`
//    are both byte-identical to `p.r.left` (6 bytes, state A). A real
//    `Rect* rp = &p.r;` local is 186 bytes and reloads the pointer every access.
//    So no C++ spelling makes the rect's stores a distinct object while the frame
//    stays one 20-byte object, which is the shape the original needs.
// What still differs, unchanged: the prologue loads grid->y into eax where the
// original uses edx, and the loop hoists the (y + 7, bottom) pair where the
// original hoists (x + 7, right). Both are one allocator or scheduler tie each, and
// the measurements above say the loop's tie needs the compiler to hoist the esi
// operand, which only a separate rect does, and that costs the prologue 18 bytes.
// So what is left is not a source spelling but a compiler state: this file's
// surface-plus-rect aggregate, its 7-statement prologue and this loop body all
// produce state A, and no combination measured here reaches the original's third
// combination of rank 4 reload with the esi hoist.
// space-bunny-free pass (issue 4652): best stays 89.8% (6 differing bytes, the same
// 2 in the prologue and 4 in the loop). Byte-level scorer plus a differ built at
// build/scratch/0x4ac8c0 (h.py scores a source file by differing bytes and prints a
// short signature of the loop window; sweep.py scores a list of variants in six
// parallel Wine compiles, about 1.2 s each; differ.py is the aligned OK/DIF
// listing). Measured here, all new, do not repeat:
//  - a self-conditional on the surface (S(s) = s ? s : s), on &p.r, on the colour,
//    on x or on y, with or without the W flip, and at the reload, at the call or
//    as the first statement of the loop body, is completely inert: 6 bytes with
//    the plain source and exactly the 18-byte state B with the flip. So the phi it
//    creates does not move the surface load's rank.
//  - a loop-invariant assignment as the first statement of the loop body (p.surface
//    = p.surface, p.surface = s, p.surface = gadgets->surface, p.r = p.r,
//    grid = grid, grid = &gadgets[index]) changes nothing when the value is already
//    in hand; the two that re-read a live memory or member object (gadgets->surface,
//    s) blow the frame up (178 and 181 bytes).
//  - the hoisted pair is the ebx-derived one in ALL 24 store orders (measured with
//    signatures, not just byte counts: every one of the 24 prints
//    "lea eax,[ebx+7] | lea edx,[esp+0x14] | mov [esp+0x20],eax | mov eax,[esp+0x10]").
//    It is not "the last store" and it does not follow the source order at all.
//  - 100 asymmetric spellings of the two +7 values (x+7, x+8-1, 7+x, x+4+3,
//    x+7+0, x-1+8, (x+8)-1, x+(3+4), x+7*1, x-(-7)) crossed on right and bottom
//    are all 6 bytes in state A.
//  - the full 10x10 matrix of self-conditional SPELLINGS on the two x stores splits
//    cleanly in three: only `0 ? v : v`, `!(v < v) ? v : v` and `v < v ? v : v`
//    leave state A, and only when BOTH stores use one of them; the other seven
//    (v ? v : v, v == 0 ? v : v, v != 0 ? v : v, v && v ? v : v, v || 0 ? v : v,
//    W(v,v), (v|0) ? v : v) all give the same byte-identical state B.
//  - the same is true for a self-conditional on the y pair (state A), and with the
//    flip plus a further self-conditional on the colour, on &p.r, on x or on y
//    (still state B).
//  - 24 store orders with the W flip on both x stores: 18 bytes when right is 1st,
//    2nd or 3rd, and 19 bytes when right is LAST, and in every single one of the
//    24 the surface load is scheduled FIRST. So under the flip the surface load is
//    never at rank 4, in any store order and with any spelling.
//  - helpers that hold the x pair (SetX(&p.r,x)), the whole rect
//    (SetRect(&p.r,x,y)), the call (Draw(s,r,c)) or the whole loop body
//    (Cell(&p,x,y,color)) all stay in state A; with the W flip inside SetX they give
//    state B. Plus7(v), a comma or a self-assignment on the x values, the colour as
//    col + row*16, (row << 4) + col, row*16+col in a temp, and 0 to 6 uncalled
//    file-scope byte-cast inline helpers: all 6 bytes, state A.
//  - the surface in a local of its own next to a separate rect local reaches the
//    SAME stack offsets (surf at esp+0x10, &r at esp+0x14) and produces the state
//    B schedule all by itself, with no ternary: 34 bytes, window "mov edx,[esp+0x10]
//    | lea eax,[esi+7] | lea ecx,[esp+0x14] | mov [esp+0x1c],eax | push edi".
//    Same for the surface in a one-member struct and the rect separate, and in
//    either declaration order. So the two properties of the missing state C are
//    tied to one thing: the surface shares an aggregate with the rect (reload at
//    rank 4) or it does not (reload at rank 1), and the hoist follows: aggregate
//    means the ebx (y) pair, separate locals mean the esi (x) pair. With the W flip
//    the aggregate gives the esi pair but drags the reload to rank 1 again.
//    Separate locals PLUS the W flip is a fourth state, 43 bytes:
//    "mov ecx,[esp+0x10] | lea eax,[esi+7] | mov [esp+0x1c],eax | lea eax,[ebx+7]
//    | mov [esp+0x20],eax | lea eax,[esp+0x14]" (the &r lea goes last, not second).
//    Reaching the rect through a pointer or a reference to the Pair (q->r, q.r, a
//    one-member Box struct) costs 93 to 94 bytes, so none of those is the shape.
//  - BIG ONE, measured, settles the prologue question: the order of the prologue's
//    seven statements really does decide the loop's hoist, so the schedule is a
//    whole-function decision, not a local one. All 7! = 5040 linear orders were
//    compiled (build/scratch/0x4ac8c0/e15.py). Only 234 of them keep the function
//    at exactly 167 bytes; of those the loop region scores 4 bytes for 66 orders
//    (every one of them state A, with prologue 2), 15 bytes for 138 and 16 for 30.
//    So NO order of these seven statements gives loop = 0 or loop = 2, and the 48
//    orders within 12 bytes are all state A. Two hand-picked orders (y_first, y_late)
//    do hoist the esi pair and reach a state the other shapes never reach, but they
//    cost 106 and 112 bytes.
//  - the same 5040 orders with the W flip on both x stores (e16.py) keep exactly the
//    same 234 orders at 167 bytes and the loop histogram only gets worse: 16 bytes
//    for 66 orders, 18 for 138, 25 for 30. So the flip never helps at any prologue
//    order either, and state C is not reachable from these seven statements.
//  - the copy-index probe (build/scratch/0x4ac8c0/probe.py, 32 copies of the same
//    body in one translation unit, each scored separately) gives 6 bytes and state A
//    for all 32, so the choice is deterministic per source, not a front-end coin flip
//    that some other spelling would win.
//  - the same stack layout reached through different IR (surface + rect in a nested
//    one-member Box, the rect as r[1], the rect inside a union member, the Box as an
//    array) compiles byte-identically in all four shapes, with and without the flip,
//    so the aggregate's IR shape is not a lever either.
//  - the outer loop's shape: int x = x0 as a declaration, as an assignment at the top
//    or at the bottom of the outer body, x declared outside, row/col declared
//    outside, for (row = 16; row--; ), for (col = 16; col--; ), y += 8 at the top of
//    the outer body, y as an outer local, and the colour as a running counter: only
//    the standard form is 6 bytes, everything else is 49 to 130.
//  - 22 prologue spellings and types (y += grid->y, y = grid->y + y,
//    y = gadgets->y + grid->y and the reverse, casts to int/short, int gy and short gy
//    temporaries, gadgets[index].y, grid as gadgets + index, (*grid).y,
//    *(short*)((char*)grid + 0x15), the surface store direct instead of through s,
//    three more statement orders, unsigned x0, long y, double y) all stay at 6 bytes
//    with grid->y in eax; only double y and the merged one-statement sums move it.
// Still differs, unchanged: the original loads grid->y into edx (we use eax) at
// 0x4ac90e, and it hoists the (x + 7, right) pair above the pushes with the surface
// reload second, where we hoist (y + 7, bottom). Both are the same tie as before,
// and the new measurements above say the surface reload's rank 4 is only ever
// reachable together with the ebx-derived hoist, and the esi-derived hoist only
// ever with the reload at rank 1. The next thing worth trying is a shape that puts
// the surface in a local of its own while keeping it at esp+0x10 and the rect at
// esp+0x14 (measured at 84.7% before, but never with the loop signature printed).
// space-bunny-free pass (issue 4539): best stays 89.8% (6 differing bytes, the same
// 2 in the prologue and 4 in the loop). The three loop states are confirmed again and
// the W(a,b) = a?b:b flip is the only lever found that reaches "right pair first",
// and it always brings the surface reload's lift with it. Measured here, do not
// repeat: the flip needs BOTH x-axis stores wrapped, and with only the right store
// wrapped the compiler rotates the loop instead (x + 7 lives in esi and left is
// esi - 7, 170 bytes). The full 16-subset matrix over the four rect stores, in all
// 24 store orders, and the fold forms a&&b?b:b / a|b?b:b / 0?b:b: only 0?b:b and the
// non-conditional injections (x|0, x*1, x^0, x<<0, x&-1, comma, x-(x-x),
// x|(x&&0), !(x<x)?x:x, x<x?x:x) leave state A; every conditional form flips to state
// B (18 bytes). Also flat at 6, all with and without the flip: a surface temp before
// or after the stores, a dead store in a folded branch (if (t) p.surface = 0,
// if (t) t = 1) with and without the flip, wrapping the surface or &p.r or the colour
// in a folded ternary (before or after the flip), stores through an inline
// Set(int&,int), ten unused locals of ten types in five positions, 0 to 8 uncalled
// file-scope inline helpers, an Identity wrapper on the colour, eight more prologue
// shapes, and all 222 linear extensions of the prologue's seven statements (the
// prologue's edx/eax tie never moves). tools/permute.py: seed 11, 8 minutes, 4 jobs,
// 5657 candidates, 89.8% -> 89.8% (build/permute/0x4ac8c0/best.diff is empty), which
// now agrees with the two default-seed runs. Byte-level scorer for free variant
// scoring, splitting the prologue and loop hunks, in build/scratch/0x4ac8c0/score.py
// and sweep.py (hunk.py prints the loop window next to the original); exp1 to exp8
// are this pass's sweeps.
// What still differs: the original's grid->y goes to edx (the register the surface
// load just freed) where we put it in eax, and the original hoists the (x + 7,
// right) pair while we hoist (y + 7, bottom). The lead for the next pass is that the
// flip and the lift are two effects of one COND node in the x-store path (the
// scheduler's tree order keeps the folded node's rank, so the COND that forces the
// right pair also drags the reload to rank 1 instead of 4). A flip from anything that
// is not a COND node on the left and right stores, or a way to keep the reload at
// rank 4 once the COND is there, would be the win; both are scheduler (SCHED tree
// order) decisions, not register allocation, which is why unused locals and file-scope
// helpers do nothing.
// Second measurement in the same pass: nothing can shift the reload's rank once the
// COND is there, and dead code cannot even reach SCHED. All of these are 6 bytes in
// state A and 18 in the flipped state, never in between: eight kinds of real dead
// store to a local scalar (dead = x, dead = x + y, dead = x + 7, dead = row*16+col,
// dead = p.surface != 0, dead = p.r.left, dead++, dead += x) in three positions each,
// two dead stores at once, a dead store in the outer loop, a duplicated (dead) store
// to p.r.left and p.r.right, and the folded-branch dead stores while (t), for (k = 0;
// k < 0; k++), if (t & 0), if (!p.surface); also an identity wrapper on the surface
// passed inline and through a temp, `*(&p.r.left) = x`, and a conditional whose arms
// are the assignments themselves. So the dead-store lever does not work here, unlike
// the function it fixed at 98.9%, and the surface's identity wrapper is as inert as
// the ternary. One more fact for the next pass: VC5 folds the conditional only when
// its condition is built from the same variable as its arms. `x > 0 ? x : x` and
// W(x, x) fold to 167 bytes, but `col ? x : x` keeps a real branch and costs 23
// bytes (190), so the COND's shape, not just its presence, is what ranks the block.
// Third measurement, same pass: every flip produces byte-identical state B (checked
// with hunk.py on the x > 0 ? x : x, the x < y ? x : x, the temp-and-ternary and the
// re-spelled-arm shapes: all emit mov edx,[esp+0x10] first, then lea eax,[esi+7],
// lea ecx,[esp+0x14], mov [esp+0x1c],eax), so the flip is one switch with no
// intermediate position to catch. Which conditionals fold is also fragile and not
// worth a rule: `x ? x : x`, `x == 0 ? x : x`, `x != 0 ? x : x`, `x && x ? x : x` and
// `x || 0 ? x : x` fold to 167 bytes, while `x > 0 ? x : x`, `x < 0 ? x : x` and
// `col ? x : x` keep a real branch (180 to 190 bytes), and a branch on the left store
// with `(x + 7) > 0` on the right folds while the same one with `x > 0` on both does
// not. The conditionals built from the same variable as their arms (x < x, !(x < x))
// fold before SCHED and leave state A alone. A named temp for x + 7, for x, or a
// re-spelled arm with a comma all just move the flip to state B (18 bytes).
// A sixth permuter run (seed 21, --minutes 9 --jobs 2, started 02:23) also wrote an
// empty build/permute/0x4ac8c0/best.diff, and its log.txt holds only the start line,
// so it looks like it exits early here rather than using its budget.
// The seed sweep is worth repeating for this address: tools/permute.py found nothing
// on seeds 11, 12 and 13 (8, 8 and 6 minutes, 4 jobs: 5657, 4887 and 1774 candidates,
// all 89.8% -> 89.8%, empty best.diff), which now makes five runs in total on this
// file.
// space-bunny-free pass (issue 4489): best stays 89.8% (6 differing bytes), but the
// loop hunk is now understood well enough to name the missing shape. The two
// remaining hunks are each one allocator tie, and both are strong attractors:
// this source always lands in state A (the pre-push lea + store pair comes from
// the outer loop coordinate y, surface reload second, in eax), and no source
// shape tried reaches state C (the same pair from the inner coordinate x, which
// is what the original has). The one shape that DOES flip the pair to x is a
// trivial inline ternary that the compiler folds away again:
//     inline int W(int a, int b) { return a ? b : b; }
// with `p.r.left = W(x, x);` and `p.r.right = W(x + 7, x + 7);` (both x-axis
// stores; adding the y-axis ones keeps state B, wrapping a single store changes
// nothing, and wrapping top/bottom instead rotates the loop and costs 3 bytes).
// That puts every store in the original's slot and hoists right = x + 7 first,
// exactly as the original does, but it also lifts the surface reload to the top
// of the block in edx (state B, 18 differing bytes):
//   A: lea eax,[ebx+7] / lea edx,[esp+0x14] / mov [esp+0x20],eax / mov eax,[esp+0x10]
//   B: mov edx,[esp+0x10] / lea eax,[esi+7]  / lea ecx,[esp+0x14] / mov [esp+0x1c],eax
//   original: lea eax,[esi+7] / lea edx,[esp+0x14] / mov [esp+0x1c],eax / mov eax,[esp+0x10]
// so the original is a third combination (x pair first AND the surface reload
// second) that neither attractor produces. State B is equally stable: a helper
// around the call, a colour temp, do-while and while inner loops, x = x + 8,
// a rect pointer, a surface temp, a color ternary, a surface ternary and a
// rect-address ternary all stay at 18. So the next attempt should look for a
// source that flips the pair without lifting the reload, not for another
// expression form of x + 7 / y + 7 (all 24 store orders, both axes, member
// forms, temps, 7 + x, x + 8 - 1, comma chains, an Identity() and a Plus7()
// wrapper, free inline helpers and Rect/Pair methods, separate Rect and surface
// locals, a union'd int[4] rect, 24 prologue declaration orders, a running
// colour counter, a swapped inner/outer loop, typed surface pointers and
// unsigned/long rect fields all stay at 6, and neither do nested blocks or a
// do-while(0) around the stores). tools/permute.py agrees on both sides: 2799
// candidates from this file and 4923 from the state B shape (8 minutes, 4 jobs)
// found nothing, so both are attractors of the mutation search too. The byte
// metric and ~350 free-scored variants are in build/scratch/0x4ac8c0 (h.py,
// e1.py to e23.py).
// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free and
// claude-opus-5-5 (#4373): still 89.8%. The only difference is that the original computes
// right = x + 7 (eax) before bottom = y + 7 (ecx) and loads grid->y into edx. All 24
// orders of the four rect stores, separate Rect/surface locals (79.7%), an i++ index
// (52.5%), x0 + col * 8 and a 10-minute permuter run (399 candidates) changed nothing.
// space-bunny-free, GPT-6.1-sol and finished by mimo-v2.6-pro.
// Space Bunny Free pass (issue 4471): best stays 89.8%, 167/167 bytes, 6 bytes
// differ. Built a byte-level variant scorer on top of check.py
// (build/scratch/0x4ac8c0/harness.py, driven by exp1.py to exp13.py, ~280
// variants, each compile scored by the number of differing bytes) and nothing
// beat the 6-byte floor. Measured, not assumed, this time:
//  - all 24 orders of the four rect stores (exp1): the pre-push (lea + store)
//    pair is always the one derived from the OUTER loop coordinate, i.e. from
//    ebx, whatever the source order; the other three stores then keep their
//    source order. 6 bytes for the left, top, right, bottom family, 9 or 11
//    for the rest, as before.
//  - a diagnostic variant with the fields swapped (exp7 g1, exp8 h2) shows
//    the hoisted pair follows the value held in ebx, not the field offset, so
//    the original hoisted its INNER loop coordinate (esi) instead. That choice
//    survives the +7 spelling (x + 4 + 3, x + 8 - 1, x + 7 + 0, member form,
//    temps, exp5), the loop header and nesting (exp6, exp10), the colour
//    expression (exp5), inlined helpers and inline methods on Rect and Pair
//    (exp4, exp10, exp12), a function pointer for the callee (exp10), and
//    comma/ternary sequencing inside the call's arguments (exp13).
//  - the prologue tie is equally firm: 14 shapes (grid pointer vs
//    gadgets[index] indexing, the found/index split, a CellX-style helper,
//    short and int y temps, unsigned coordinates, swapped sum operands and
//    declaration orders, exp2 and exp11) all keep grid->y in eax, the dying
//    &gadgets[index] temp, where the original puts it in edx, the register
//    the surface temp has just freed. Note the MATCHED sibling 0x4ac970 lands
//    its grid->y load in edx only because there eax is still busy with
//    gadgets->x, so it is not the same tie.
//  - rebuilding the function in the sibling's own style (int index, found;
//    short cellY; separate surface and rect locals; the CellX helper) is far
//    worse, 69 bytes and more (exp9), so the Pair aggregate and the s/gx
//    split in the prologue stay.
//  - tools/permute.py: 2799 candidates in 15 minutes, 89.8% -> 89.8%.
//  - the frame is the surface plus the rect (5 dwords, sub esp 0x14) and the
//    x0 spill reuses the __stdcall argument slot, which this source produces.
// Both remaining hunks still look like one allocator tie each: which free
// register a freshly loaded value coalesces into, and which of the two equal
// cost lea + store pairs is scheduled before the call's pushes.
// claude-opus-5-5, Codex, GPT-6.1-sol, deepseek-v4.1-flash, mimo-v2.6-pro,
// space-bunny-free and LongCat 2.5 Preview Free all worked on this file.
// Names are provisional.
// deepseek-v4.1-flash #4306 retry: 89.8% text score but only 6 differing
// bytes now (was 11). Two things learned, both measured at the byte level
// (build/scratch/0x4ac8c0/quick.py, an.py, sweepA.py):
// 1. check.py's ratio is a difflib ratio over normalised instruction text, so
//    it hides byte differences: the previous store order (right, left, top,
//    bottom) reported the same 89.8% but differed in 11 bytes, because the
//    three post-push stores came out in the wrong order. Measured over all 24
//    store orders, left, top, right, bottom (the order now in the file, same
//    for LTBR/LBTR/BLTR) is the minimum: 6 differing bytes, exactly the two
//    known hunks, 0x4ac910/0x4ac913 (prologue) and 0x4ac926/0x4ac92f/0x4ac936/
//    0x4ac945 (loop). Every other order is 9 or 11 bytes off.
// 2. A standalone mini model of this loop (build/scratch/0x4ac8c0/mini/run.py:
//    struct P { void* s; R r; } plus the same two nested loops and the same
//    call) reproduces our codegen instruction for instruction, and in it the
//    compiler always schedules the pair derived from the OUTER loop coordinate
//    (bottom = y + 7) first, whatever the source order of the four stores.
//    The original schedules the pair derived from the INNER coordinate
//    (right = x + 7) first. In the mini this only flips when the surface load
//    itself moves to edx and is scheduled first (a second surface member in
//    the aggregate, or surface and rect in separate structs), and those shapes
//    change the register assignment away from the original (which keeps the
//    surface load in eax after the hoisted store, exactly like ours). So both
//    remaining hunks look like the same allocator tie-break: which free
//    register (eax vs edx) a freshly loaded value coalesces into.
// Also tried here with the byte metric, all unchanged at 6 bytes: all 24 store
// orders, member forms (right = left + 7, bottom = top + 7), named temporaries
// for right/bottom, rect reference/pointer locals, comma statements, unsigned
// coordinates and fields, 7 + x / x + 8 - 1 / (x + 7) forms, prologue
// declaration orders, assignment chains, and inline helpers (a static inline
// helper is rejected by this compiler: C2267, use plain inline).
// #3120 retry by GPT-6.1-sol: best remains 89.8%; a separate gridY local fell to 86.4%, declaration-order swap was unchanged, and explicit while loops fell to 56.9%. Remaining diff is register allocation for grid y and the rectangle x/y+7 values.
// mimo-v2.6-pro retry: best stays 89.8%. Scripted sweeps under
// build/scratch/0x4ac8c0/ (sweep.py, sweep2.py, sweep3.py, sweep4.py, sweep5.py,
// sweep6.py) scored hundreds of shapes free of check runs and found the rule
// behind the loop diff: exactly one (lea + store) pair is scheduled before the
// call's argument pushes and the other three stores keep their source order
// after the pushes. This compiler always hoists the (y+7, bottom) pair; the
// original always has the (x+7, right) pair there and bottom's store last.
// That choice is invariant across all 24 rect store orders, 4 loop forms
// (nested row/col, counter do-while/while/for), member-form +7 values
// (right = left + 7 and bottom = top + 7), temp locals, one comma statement,
// free inline helpers holding the stores and/or the call, inline methods on
// Rect and Pair, and inline field accessors. Separate surface/rect locals fall
// to 84.7% (the shared aggregate keeps the prologue), sum forms to 72.9% and
// 79.7%, and short/int/late y locals to 88.1% because they swap the two y
// load order. All 128 header sets stay 89.8%. The prologue diff is one
// register: grid->y coalesces into the dying grid base in eax here, where the
// original lands it in edx (the register the surface temp just freed). No
// shape changed that coalescing either, so both remaining hunks look like
// scheduler/allocation ties of the same kind.
// finished by deepseek-v4.1-flash (89.8% retry).
// #2936 retry by GPT-6.1-sol: five checks retained the 89.8% best; declaration
// order and helper variants did not change the surface-access schedule.
// deepseek-v4.1-flash #2401 retry: raised 88.1% -> 89.8% (167/167 bytes) by
// splitting the surface assignment: `void* s = gadgets->surface;` then
// `int gx = gadgets->x;` then `p.surface = s;` then `x0 += gx;`. That makes
// the compiler emit the original order in the prologue: L gadgets->x (it
// hoists this load above the store), S p.surface, A x0. The prologue is now
// byte-identical except that grid->y is loaded into eax instead of edx. The
// loop regressed slightly: it now computes y+7 (bottom) first into eax and
// x+7 (right) second into ecx, where the original computes x+7 first; the
// surface reload is in the original slot (after the right store). Both
// remaining hunks are register/scheduler tie-breaks in the same y (ebx)
// node; all shapes tried here (separate locals, extra temps for both
// coordinates, reference to the grid, reindexing gadgets[index].y) stay at
// 89.8% or below. See the history below for the earlier 88.1% analysis.
// #1610 retry by Codex / GPT-6.1-sol: checkall reconfirmed 88.1% (167/167 bytes), no MATCH.
// The inline helper probe produced identical diffs; the surface access schedule remains different.
// Draws the 16x16 "COLS" colour grid of a gadget, each cell 8x8 pixels; the
// cell index (0..255) is the fill colour. Inverse of 0x4acbe0, sibling of
// 0x4ac970 (which draws one cell's frame).
// check.py: 88.1%, still not a match. Two differences, both the scheduler
// placing p.surface's memory access one slot too early: the store of
// p.surface in the prologue (the original has it between the load of
// gadgets->x and the add that uses it) and the reload of p.surface in the
// loop (the original has it after the store of p.r.right, which then lets it
// reuse eax for the surface and ecx for ebx + 7).
// space-bunny-free re-run: still 88.1%, 10 of 59 instructions differ, all in
// those two places. The first divergence is 0x4ac908, class (d) statement
// order: the original loads gadgets->x, THEN stores p.surface, then adds;
// ours stores p.surface, then loads gadgets->x, then adds, and it also pulls
// the load of grid->y above the add (original keeps it after). So MSVC hoists
// the two commutative loads one slot further than the original does.
// The calling convention is NOT the cause: ret 4 with one dword argument is
// __stdcall, and the file scores 88.1% unchanged under /Gz, /Gr, /Gd, /Gs,
// /Ob1 and /Ob2, with and without the declared __stdcall.
// Neither is the callee argument values: FUN_0049fdf0(gadgets, "COLS", 6)
// and FUN_004bf6f0(surface, &p.r, row * 16 + col) both have the right
// count and the right values (the push order colour, &r, surface matches).
// About 40 free-scored variants all land at 88.1% or below and none reach
// it, so this looks like a scheduler tie, not a missing construct:
// - surface and rect as two separate locals (i_sep, j_sep): 84.7%, and the
//   separate rect also reorders the two coordinate loads (esi/ebx swapped),
//   so the shared aggregate IS needed to get the prologue right;
// - y and x0 folded into one initialiser each, with the surface assigned
//   before, between or after them (q1..q5, a_pair_expr, p_swap_adds,
//   x4, x5, y2): 68% to 83%, always worse;
// - the four rect stores in all six orders (u1..u6, e_order_ltrb,
//   m_temps, z3, and right/top/left/bottom): 86.4% to 88.1%, never above;
// - the stores or the surface written through a T& / a pointer to the member
//   (t1, t3, r1, r3, r4, w4, z2), the pair read through a pointer (w3,
//   w4), an inline member doing the four stores and the call (w2), and a
//   dead self-assignment p.r = p.r (y1): all exactly 88.1%, so none of
//   them moves the scheduler tie;
// - the colour as a running counter (o_running, x1, x2) or as
//   (row << 4) + col (x3): 64% to 75%, the counter costs an extra local.
// An inline AddXAfterSurface helper was also tested; it scored the same
// 88.1% and emitted identical mismatches, so this best variant is retained.
// The prologue and the loop want opposite things from the same store: the
// original delays the store in the prologue and delays the reload in the
// loop, and no single source shape reproduced both.
// #1879 space-bunny-free pass: still 88.1% (10 of 59 instructions). All 128
// header sets score 88.1% (headers.py), so no include fixes it. The exact
// remaining diff, worked hunk by hunk:
//   prologue, ours emits  L surf / S surf / L g->x / L grid->y / A / A,
//           original has  L surf / L g->x / S surf / A / L grid->y / A.
//   loop,      ours emits  L surf / lea y+7 / lea &r / S right / ...
//           original has  lea y+7 / lea &r / S right / L surf / ...
// i.e. the one p.surface memory access is exactly one slot EARLIER in ours
// in both places, and everything else in the function is already right.
// Ten more shapes scored with --sym, none better: the surface and the rect
// as two separate locals in both declaration orders (84.7%, and the two
// coordinate loads swap registers), the comma statement
// (p.surface = gadgets->surface, x0 += gadgets->x), the pair declared after
// the coordinates, the pair reached through a pointer, a void*& to its
// surface member, a one-member wrapper struct for the rect, the surface
// copied to a local at the top of the outer loop (75.0%), the add before
// the store (76.3%), and the store first of all (79.7%) are all 88.1% or
// worse. The pair aggregate really is the right shape (it is what puts
// p.surface at esp+0x10 and &p.r at esp+0x14), so this stays a scheduler
// tie in the p.surface node that no source shape reached.
#pragma pack(push, 1)
struct Gadget_004ac8c0 {               // 0x15b bytes
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    char unknown_17[0xbc - 0x17];
    void* surface;                     // +0xbc
    char unknown_c0[0x15b - 0xc0];
};

struct Holder_004ac8c0 {
    int unknown_0;
    Gadget_004ac8c0* gadgets;          // +0x4
};

struct Object_004ac8c0 {
    char unknown_0[0x18];
    Holder_004ac8c0* holder;           // +0x18
};
#pragma pack(pop)

struct Rect_004ac8c0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// The surface and the rectangle share one local: the surface is spilled before
// the loop and reloaded in it, and keeping both in a single aggregate is what
// reproduces the original's load order and its two stack slots.
struct Pair_004ac8c0 {
    void* surface;                     // +0x0
    Rect_004ac8c0 r;                   // +0x4
};

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004bf6f0(void* surface, Rect_004ac8c0* rect, int color);

// FUNCTION: 0x4ac8c0
void __stdcall FUN_004ac8c0(Object_004ac8c0* obj)
{
    Gadget_004ac8c0* gadgets = obj->holder->gadgets;
    int index = FUN_0049fdf0(gadgets, "COLS", 6);
    Gadget_004ac8c0* grid = &gadgets[index];
    Pair_004ac8c0 p;
    int y = gadgets->y;
    int x0 = grid->x;
    void* s = gadgets->surface;
    int gx = gadgets->x;
    p.surface = s;
    x0 += gx;
    y += grid->y;
    for (int row = 0; row < 16; row++) {
        int x = x0;
        for (int col = 0; col < 16; col++) {
            p.r.left = x;
            p.r.top = y;
            p.r.right = x + 7;
            p.r.bottom = y + 7;
            FUN_004bf6f0(p.surface, &p.r, row * 16 + col);
            x += 8;
        }
        y += 8;
    }
}
