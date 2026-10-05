// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5, edited by deepseek-v4.1-flash and deepseek-v4.1, finished by opus. Names are provisional.
// The original calls its two real neighbours, ChebyshevDistance (the larger of the
// two absolute coordinate differences) and StepPointTowards (step a point towards
// a target), and /Ob2 inlines both. They are defined here as in their own
// matched files, with no FUNCTION line (static inline or not makes no
// difference). What earlier passes (66.2%) lacked was the combination of
// ChebyshevDistance's real body, the `Sub` helper returning a Diff struct, with the
// max written as a ternary, which matches 0x480570 too: Sub plus the if-form
// (`if (dx > dy) dy = dx; return dy;`) gives 51.3% here, and the ternary
// without Sub (`abs(a.x - b.x)`) 63.5%.
#include <stdlib.h>

struct Point_004805b0 {
    short x;
    short y;
};

struct Diff_00480570 { int dx; int dy; };

static inline Diff_00480570 Sub(Point_004805b0 a, Point_004805b0 b)
{
    Diff_00480570 d;
    d.dx = a.x - b.x;
    d.dy = a.y - b.y;
    return d;
}

Point_004805b0 __stdcall StepPointTowards(Point_004805b0 p, Point_004805b0 target, short step)
{
    if (target.x < p.x)
        p.x -= step;
    else if (target.x > p.x)
        p.x += step;
    if (target.y < p.y)
        p.y -= step;
    else if (target.y > p.y)
        p.y += step;
    return p;
}

int __stdcall ChebyshevDistance(Point_004805b0 a, Point_004805b0 b)
{
    Diff_00480570 d = Sub(a, b);
    int dx = abs(d.dx);
    int dy = abs(d.dy);
    return dx > dy ? dx : dy;
}

// FUNCTION: 0x4805b0
void __stdcall FUN_004805b0(Point_004805b0* p, Point_004805b0 b, Point_004805b0 c)
{
    int d1 = ChebyshevDistance(*p, b);
    int d2 = ChebyshevDistance(*p, c);
    int d3 = ChebyshevDistance(b, c);
    if (d1 > d2) {
        if (d1 > d3) d1 = d3;
        *p = StepPointTowards(b, c, d1);
    } else {
        if (d2 > d3) d2 = d3;
        *p = StepPointTowards(c, b, d2);
    }
}
