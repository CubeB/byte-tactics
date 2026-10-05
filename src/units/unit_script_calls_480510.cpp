// Decompiled by Opus. Names are provisional.
// Moves a 16-bit point one step towards a target on each axis.

struct Point_00480510 {
    short x;
    short y;
};

// FUNCTION: 0x480510
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
