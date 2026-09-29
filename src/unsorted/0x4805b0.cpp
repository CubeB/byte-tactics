// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5. Names are provisional.
// PARTIAL: 64.5%, 354 of 366 bytes (was 49.4% and 334 bytes).
// Claude Sonnet 5.5 pass (#554): the original calls nothing, because it inlines
// FUN_00480570 (max of the two absolute differences, Points passed by value) three
// times and inlines a "move one point towards another" helper twice. That is what
// the source is now:
//  - `Dist(Point a, Point b)` with the ternary body `return dx > dy ? dx : dy;`
//    (the if-form scored 44 to 50%, the ternary 60%); the `a` copy in stack slot
//    [esp+0x10] and its reuse for d1 come from the by-value parameter temp.
//  - `Toward(Point from, Point to, int d)` modifies its by-value `from` in place
//    and returns it, `*p = Toward(b, c, d1)`: the original tail (`mov eax,[esp+0x24];
//    mov edx,[esp+0x28]; cmp dx,ax; mov [esp+0x24],eax` then in-place word stores)
//    is reproduced instruction for instruction in shape, because the inline
//    parameter reuses the caller's dead argument slot.
//  - min is written `if (d1 > d3) d1 = d3;` (62.0 against 60.5 for `d3 < d1`).
// What still differs is only register allocation in the first two distance blocks:
// the original loads b.x into ebx and b.y into ebp with two `movsx` from the
// argument slots before anything else (member-wise), keeps c.x in ecx and spills
// c.y to [esp+0x18]; ours loads b and c as whole dwords (c: `mov ebx,[esp+0x28]`,
// then uses bx) and spills b.x. A named local `Point a = *p;` scores 63 to 64.5%
// but adds a local at [esp+0xc]; the unnamed `Dist(*p, b)` form (62.0%) has the
// right slots ([esp+0x10] and [esp+0x14] for the two `a` temps) but hoists the whole
// dword load of b. Tried without effect: Dist or Toward taking the second Point by
// const reference (44 to 62%), comparison direction in Toward (all the same), Dist
// with if-forms or swapped operands, d3 first (39.9%), d2 before d1 (49.5%), a
// separate result temp in the tail. The declaration-count probe (0 to 400 unused
// externs) is flat for the old file, and headers.py gives 64.5% for every set that
// compiles, so this is source shape, not compiler state.
#include <stdlib.h>

struct Point_004805b0 {
    short x;
    short y;
};

static inline int Dist(Point_004805b0 a, Point_004805b0 b)
{
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    return dx > dy ? dx : dy;
}

static inline Point_004805b0 Toward(Point_004805b0 from, Point_004805b0 to, int d)
{
    if (to.x < from.x) from.x -= d;
    else if (from.x < to.x) from.x += d;
    if (to.y < from.y) from.y -= d;
    else if (from.y < to.y) from.y += d;
    return from;
}

// FUNCTION: 0x4805b0
void __stdcall FUN_004805b0(Point_004805b0* p, Point_004805b0 b, Point_004805b0 c)
{
    Point_004805b0 a = *p;
    int d1 = Dist(a, b);
    int d2 = Dist(a, c);
    int d3 = Dist(b, c);
    if (d1 > d2) {
        if (d1 > d3) d1 = d3;
        *p = Toward(b, c, d1);
    } else {
        if (d2 > d3) d2 = d3;
        *p = Toward(c, b, d2);
    }
}
