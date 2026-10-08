// Decompiled by Opus, Sonnet, space-bunny-free, Claude Sonnet 5.5, deepseek-v4.1-flash, deepseek-v4.1, opus and Haiku. Names are provisional.

#include <stdlib.h>

struct Point_00480510 {
    short x;
    short y;
};

struct Point_00480570 {
    short x;
    short y;
};

struct Point_004805b0 {
    short x;
    short y;
};

struct Point_00480720 {
    short x;
    short y;
};

struct Diff_00480570 { int dx; int dy; };

// Direction (0-7) from one point to another; 0 also when they coincide.
// The dy < 0 case keeps its own return 0: merged with the fall-through, MSVC
// makes the last case branchless. <stdlib.h> sets the load order of to.x and
// from.x.
// FUNCTION: 0x480720
char __stdcall OctantBetween(Point_00480720 from, Point_00480720 to)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;
    if (dx > 0) {
        if (dy > 0)
            return 5;
        if (dy < 0)
            return 7;
        return 6;
    }
    if (dx < 0) {
        if (dy > 0)
            return 3;
        if (dy < 0)
            return 1;
        return 2;
    }
    if (dy > 0)
        return 4;
    if (dy < 0)
        return 0;
    return 0;
}

// Moves `value` towards `target` by `step` without overshooting.
// FUNCTION: 0x4804e0
int __stdcall StepTowards(int value, int target, int step)
{
    if (value < target) {
        value += step;
        if (value >= target) {
            value = target;
        }
    } else if (value > target) {
        value -= step;
        if (value <= target) {
            value = target;
        }
    }
    return value;
}

// Moves a 16-bit point one step towards a target on each axis.
// FUNCTION: 0x480510 ?StepPointTowards@@YG?AUPoint_00480510@@U1@0F@Z
Point_00480510 __stdcall StepPointTowards(Point_00480510 p, Point_00480510 target, short step)
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

static inline Diff_00480570 Sub(Point_00480570 a, Point_00480570 b)
{
    Diff_00480570 d;
    d.dx = a.x - b.x;
    d.dy = a.y - b.y;
    return d;
}

// FUNCTION: 0x480570 ?ChebyshevDistance@@YGHUPoint_00480570@@0@Z
int __stdcall ChebyshevDistance(Point_00480570 a, Point_00480570 b)
{
    Diff_00480570 d = Sub(a, b);
    int dx = abs(d.dx);
    int dy = abs(d.dy);
    if (dx > dy) dy = dx;
    return dy;
}

// The original calls its two real neighbours, ChebyshevDistance (the larger of the
// two absolute coordinate differences) and StepPointTowards (step a point towards
// a target), and /Ob2 inlines both. They are defined here as in their own
// matched files, with no FUNCTION line (static inline or not makes no
// difference). What earlier passes (66.2%) lacked was the combination of
// ChebyshevDistance's real body, the `Sub` helper returning a Diff struct, with the
// max written as a ternary, which matches 0x480570 too: Sub plus the if-form
// (`if (dx > dy) dy = dx; return dy;`) gives 51.3% here, and the ternary
// without Sub (`abs(a.x - b.x)`) 63.5%.
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
void __stdcall ClampPointToSegment(Point_004805b0* p, Point_004805b0 b, Point_004805b0 c)
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

class WeaponAimCobCb {
public:
    char unknown_0[4];
    int field_4;                       // +0x4

    void OnAimCobReturn(int enable);
};

// FUNCTION: 0x481490
void WeaponAimCobCb::OnAimCobReturn(int enable)
{
    if (enable) {
        field_4 = 1;
    }
}

// FUNCTION: 0x4814b0
int __stdcall AimCobStub(int, int, int, int)
{
    return 0;
}
