// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// wall: callee-saved register rotation. The original holds pos in edi and rad in
// ebx; we get pos in ebx and rad in edi, plus the i = 0 store sinks past the
// guard and materialises two fresh zeros for x2/y2. deepseek-v4.1-flash checked
// all four levers: (1) the argument list, callers' pushes and every callee ret N
// re-verified correct; (2) no inherited "original bug" survives re-derivation
// (the n < 0 path's loads from the counter slot are a known-zero rematerialisation,
// not a bug); (3) headers.py swept all 768 header sets, best is the current
// 54.0%; (4) grepping orig/TotalA.exe for the raw bytes of the construct
// (loop head 8B5C24185355 and the n<0 double load 8B74244C8B5C244C) finds exactly
// one hit each, this function, so there is no sibling to copy. 14 more targeted
// respellings (rad/step/index order and scope, unsigned rad/step/angle, a local
// pos copy, x2/y2 declaration moves, radius*0x10000) all scored 45.0% to 54.0%,
// none above the current 54.0%. Stop.
// PARTIAL, 54.0% (500 of 505 bytes; was 38.2% at hand-off). Up from 38.2% by
// five things, all confirmed by check.py, so do not re-sweep any of them:
//   a. `i = 0;` must be a STATEMENT after `int n = ...`, not an initialiser
//      on the declaration of i. That alone is worth 1.2 points (52.8% to
//      54.0%) because it is what puts the loop counter in the dead arg4 slot
//      at +0x4c instead of giving rad that slot.
//   b. The offsets must be negated into NAMED temporaries
//      (`int nx1 = -FUN_004b70ef(angle, rad);`), one per point. Spelling it
//      inline as `pos->x_frac - (-FUN_004b70ef(...))` makes MSVC fold the
//      double negation into an `add`; the named temporaries are what produce
//      the original's `mov esi, eax; neg esi; ... sub ecx, esi`.
//   c. The three pos components must be read through named temporaries in the
//      order x, z, y. Reading them inline reorders the loads.
//   d. The screen offsets must be read into `sx` and `sy` locals. Inline
//      `view->scroll_x` costs about 5 points (49.0% against 54.0%).
//   e. The loop has to be a do-while inside an explicit `if (i <= n)`, with
//      step and rad declared inside the guard. A plain `for` costs 13 points.
//      The guard spelling itself does not matter: `i <= n`, `0 < n`, `n >= 0`
//      and `!(n < 0)` all give the same 54.0%, so do not sweep those.
//
// DEAD LEVERS, ALREADY EXHAUSTED (do not spend runs on them):
//  * Headers and compiler state. This is the lever that took 0x4399f0 from
//    94.8% to a match, and it does NOTHING here. Swept with <stdlib.h> alone
//    (54.0%), <stdlib.h>+<math.h>+<memory.h> (52.2%), <windows.h>+<memory.h>
//    (52.2%), <windows.h> alone (52.2%), and
//    <windows.h>+<stdlib.h>+<math.h>+<memory.h> (54.0%, tied). With no headers
//    at all it will not compile, because <stdlib.h> is what supplies __max.
//    The N-unused-`extern int` calibration from 0x4399f0 is FLAT here: every
//    N from 0 to 312 in steps of 8 scores exactly 54.0%, so this function has
//    no declaration-count window and the reassociation effect that bit the
//    sibling does not reach it. Whatever is left is a real source difference,
//    not compiler state.
//  * Call-site arity and argument order. All four callees were checked
//    against ctx.py and against the original's push order and are CORRECT:
//    FUN_004be950 takes 6 args (surface first, cleans 0x18), FUN_004c14f0
//    takes 5 (surface, text, lx, ly + 4, -1), FUN_00485070 takes 1 by
//    reference, and both angle helpers are 2-arg __cdecl (angle, rad).
//  * The y-sum spelling. `p.z - view->cy - (p.y >> 1)`, 0x4399f0's order, and
//    the order here all compile to the same thing once sx and sy are locals;
//    the direct forms cost 4 points because they drop the sx/sy locals.
//
// What still differs, in order of size:
//  1. Register rotation. The original keeps the loop angle in ebp and pos in
//     edi, and reuses one ebp zero for the radius test, the two zero stores,
//     the counter's zero and the guard, so only rad is reloaded from its slot
//     at the loop head and nothing is spilled in the loop. Ours puts the
//     angle in esi, pos in ebx and rad in edi, reloads pos and rad at the loop
//     head, and spills the angle into arg5's dead slot at +0x50. The tell is
//     the loop tail: the original computes y1 in edx, a scratch, so its ebp
//     stays free for the angle; ours computes y1 in ebp, which is what takes
//     the angle's register away.
//  2. Zero registers. Ours materialises `xor edi,edi; xor esi,esi` for
//     x2 = 0 and y2 = 0 where the original makes no extra zero at all: it
//     emits i = 0 into the dead arg4 slot BEFORE the guard, and on the n < 0
//     path copies that known zero out of the slot straight into the x2 and y2
//     registers. Ours sinks the i = 0 store PAST the guard test, so the zero is
//     not available there and two fresh zeros appear instead. Seeding x2 and y2
//     from i, chained assignment, and moving the initialisers all round have
//     been tried and none removes the two xors; the fix has to make the
//     counter's store land before the branch, not to make the values share.
//  3. Second point's store order. The original stores p2's fields y, x, z
//     (0x34, then 0x30 after the push, then 0x38). Spelling it that way in the
//     source scores 51.0% against 54.0% for y, z, x, so the source order here
//     is a deliberate trade: y, z, x loses fewer bytes overall because the
//     mismatch is absorbed further down. Worth one more look by someone who can
//     fix items 1 and 2 first, since the two interact.
//
// What is already right: the 0x2c frame, the 16.16 Pos_00438ea0 layout, every
// frame slot (ly +0x10, lx +0x14, rad +0x18, step +0x1c, n +0x20, p1 +0x24,
// p2 +0x30), the loop counter in the dead arg4 slot, index *= 3 in arg7's
// slot, the negated offsets kept in callee-saved registers, the x, z, y load
// order in both inlined point copies, the __max double call for the terrain
// height, and both draw calls.
// Claude Sonnet 5.5 pass (#705), nothing beat 54.0% and 500 bytes. Ruled out on
// top of the list below (all scored, none counted as runs): the declaration-count
// sweep to N = 400 (two states only, 52.2% and 54.0%, both 500 bytes) and all 128
// header sets of headers.py (54.0% at best); an inline `RingPoint(out, pos, angle,
// rad)` helper for the two point computations (53.4%); about 75 respellings of
// the outer structure and the loop body: declaration order and scope of x2, y2, i,
// angle, lx and ly (53.3 to 54.0%), `radius <<= 16` instead of `rad` (51.0 to
// 52.8%), an `a2` copy of the angle (52.8%), guard forms, `++i <= n` and early
// returns (same bytes), the draw-call argument and x1/y1/x2/y2 order (same), a
// local copy of `pos` (same), p1 and p2 store orders (48.7 to 54.0%).
// Leads: (1) on the n < 0 path the original loads x2 and y2 from the counter's
// stack slot inline (`mov esi,[esp+0x4c]; mov ebx,[esp+0x4c]`), which is what
// UNINITIALISED x2 and y2 give, but that version is 506 bytes and 45.0% (the block
// moves after `ret`, it loads `index` or `color` instead of `i`, and the roles
// shift: radius in ebp, zero in ebx). (2) The roles are swapped against the
// original: its rad is in ebx and its zero/angle in ebp, ours the other way round;
// its tail uses `test reg, reg` because no zero register survives there.
// Draws a ring of n + 1 line segments around a 16.16 map position, where
// n = radius * pi / 4, and the label of the segment number index * 3 under it.
#include <stdlib.h>

// A 16.16 world position: the frac/whole halves share one dword, so the code
// adds whole values through *(int*)&frac and reads the whole part back.
struct Pos_00438ea0 {
    unsigned short x_frac;               // +0x0
    short x;                             // +0x2
    unsigned short y_frac;               // +0x4
    short y;                             // +0x6
    unsigned short z_frac;               // +0x8
    short z;                             // +0xa
};

struct View_00438ea0 {
    char unknown_0[0x2c];
    int scroll_x;                        // +0x2c
    int scroll_y;                        // +0x30
};

extern double DAT_004fd2b0;              // 6.28318530717958
extern double DAT_004fd2b8;              // 0.125

int __cdecl FUN_004b70ef(int angle, int radius);
int __cdecl FUN_004b7123(int angle, int radius);
int __stdcall FUN_00485070(Pos_00438ea0* pos);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y, int maxWidth);

// FUNCTION: 0x438ea0
void __stdcall FUN_00438ea0(void* surface, View_00438ea0* view, Pos_00438ea0* pos, int radius,
                            int color, const char* text, int index)
{
    int angle = 0;
    if (radius) {
        int lx = 0;
        int ly = 0;
        int i;
        int n = (int)(radius * DAT_004fd2b0 * DAT_004fd2b8);
        i = 0;
        int x2 = 0;
        int y2 = 0;
        if (i <= n) {
            int step = 0x10000 / n;
            int rad = radius << 16;
            index *= 3;
            do {
                Pos_00438ea0 p1;
                int nx1 = -FUN_004b70ef(angle, rad);
                int nz1 = -FUN_004b7123(angle, rad);
                int vx1 = *(int*)&pos->x_frac;
                int vz1 = *(int*)&pos->z_frac;
                int vy1 = *(int*)&pos->y_frac;
                *(int*)&p1.x_frac = vx1 - nx1;
                *(int*)&p1.z_frac = vz1 - nz1;
                *(int*)&p1.y_frac = vy1;
                p1.y = __max(pos->y, FUN_00485070(&p1));
                angle += step;
                Pos_00438ea0 p2;
                int nx2 = -FUN_004b70ef(angle, rad);
                int nz2 = -FUN_004b7123(angle, rad);
                int vx2 = *(int*)&pos->x_frac;
                int vz2 = *(int*)&pos->z_frac;
                int vy2 = *(int*)&pos->y_frac;
                *(int*)&p2.y_frac = vy2;
                *(int*)&p2.z_frac = vz2 - nz2;
                *(int*)&p2.x_frac = vx2 - nx2;
                p2.y = __max(pos->y, FUN_00485070(&p2));
                int sx = view->scroll_x;
                int sy = view->scroll_y;
                int x1 = p1.x - sx + 0x80;
                int y1 = p1.z - (p1.y >> 1) - sy + 0x20;
                x2 = p2.x - sx + 0x80;
                y2 = p2.z - (p2.y >> 1) - sy + 0x20;
                FUN_004be950(surface, x1, y1, x2, y2, color);
                if (i == index) {
                    lx = x2;
                    ly = y2;
                }
                i++;
            } while (i <= n);
        }
        if (text) {
            if (lx == 0 && ly == 0) {
                lx = x2;
                ly = y2;
            }
            FUN_004c14f0(surface, text, lx, ly + 4, -1);
        }
    }
}
