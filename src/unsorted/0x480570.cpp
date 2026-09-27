// Decompiled by Sonnet. Names are provisional.
#include <stdlib.h>

struct Point_00480570 {
    short x;
    short y;
};

struct Diff_00480570 { int dx; int dy; };

static inline Diff_00480570 Sub(Point_00480570 a, Point_00480570 b)
{
    Diff_00480570 d;
    d.dx = a.x - b.x;
    d.dy = a.y - b.y;
    return d;
}

// FUNCTION: 0x480570
int __stdcall FUN_00480570(Point_00480570 a, Point_00480570 b)
{
    Diff_00480570 d = Sub(a, b);
    int dx = abs(d.dx);
    int dy = abs(d.dy);
    if (dx > dy) dy = dx;
    return dy;
}
