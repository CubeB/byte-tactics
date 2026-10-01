// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free,
// claude-opus-5-5 (#4373), GPT-6.1-sol, mimo-v2.6-pro and finished by
// space-bunny-free (#4489). Names are provisional.
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