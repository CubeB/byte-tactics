// Decompiled by Opus. Names are provisional.
#include <stdlib.h>

struct Point_00480720 {
    short x;
    short y;
};

// Direction (0-7) from one point to another; 0 also when they coincide.
// The dy < 0 case keeps its own return 0: merged with the fall-through, MSVC
// makes the last case branchless. <stdlib.h> sets the load order of to.x and
// from.x.
// FUNCTION: 0x480720
char __stdcall FUN_00480720(Point_00480720 from, Point_00480720 to)
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
