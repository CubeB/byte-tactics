// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free and
// space-bunny-free and GPT-6.1-sol. Names are provisional.
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
            p.r.right = x + 7;
            p.r.left = x;
            p.r.top = y;
            p.r.bottom = y + 7;
            FUN_004bf6f0(p.surface, &p.r, row * 16 + col);
            x += 8;
        }
        y += 8;
    }
}
