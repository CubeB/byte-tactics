// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL: the tail (both move-one-axis-towards branches) and the three
// abs/max distance blocks match the original in shape, but the first two
// blocks differ in register allocation, which shifts everything after them.
// What still differs (ours vs original at 0x4805b0):
//   * the original loads b.x into ebx between `push esi` and `push edi` and
//     keeps b.x/b.y in callee-saved registers, spilling c.y instead; we spill
//     b.x and b.y and keep c.x/c.y in registers;
//   * the original keeps the copy of *p in ONE register (edi, read as `di` in
//     the second block) and materialises the 4-byte struct only for the `.y`
//     reads; we split it into two registers (a.x, a.y), which costs the extra
//     live value and flips the spills;
//   * consequently d1/d2 stay in registers for us while the original keeps d1
//     in the stack slot the struct temp just used.
// Tried and rejected: `abs(a.x - b.x)` operand order (loads a before b),
// dx/dy temporaries instead of the recompute, a static inline Dist helper
// (0x480570's shape), (*p).x / p->x and a `Point&` parameter, and computing
// the three distances in another order. The reversed operand order
// (`abs(b.x - a.x)`) is what gets the per-expression load order right.
#include <stdlib.h>

struct Point_004805b0 {
    short x;
    short y;
};

// Clamps one end of the segment b-c towards the other by the smaller of the
// two distances it is not at, i.e. by min(max(dist(a,b), dist(a,c)),
// dist(b,c)) with Chebyshev distance; *p supplies a and receives the result.
// FUNCTION: 0x4805b0
void __stdcall FUN_004805b0(Point_004805b0* p, Point_004805b0 b, Point_004805b0 c)
{
    Point_004805b0 a = *p;
    int d1 = abs(b.x - a.x);
    if (d1 <= abs(b.y - a.y))
        d1 = abs(b.y - a.y);
    int d2 = abs(c.x - a.x);
    if (d2 <= abs(c.y - a.y))
        d2 = abs(c.y - a.y);
    int d3 = abs(c.x - b.x);
    if (d3 <= abs(c.y - b.y))
        d3 = abs(c.y - b.y);
    if (d1 > d2) {
        if (d3 < d1) d1 = d3;
        if (c.x < b.x) b.x -= d1;
        else if (b.x < c.x) b.x += d1;
        if (c.y < b.y) b.y -= d1;
        else if (b.y < c.y) b.y += d1;
        *p = b;
    } else {
        if (d3 < d2) d2 = d3;
        if (b.x < c.x) c.x -= d2;
        else if (c.x < b.x) c.x += d2;
        if (b.y < c.y) c.y -= d2;
        else if (c.y < b.y) c.y += d2;
        *p = c;
    }
}
