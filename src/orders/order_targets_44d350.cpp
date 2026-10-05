// Decompiled by Opus. Names are provisional.
#include <stdlib.h>
#include <math.h>

class Class_0044d350 {
public:
    char unknown_0[0x8];
    short x;                           // +0x8
    short y;                           // +0xa
    int radius;                        // +0xc

    int FUN_0044d350(int px, int py);
};

// Approximate distance from (px, py) to the edge of the circle around
// (x, y), 0 when inside: 18 * major + 7 * minor axis distance.
// FUNCTION: 0x44d350
int Class_0044d350::FUN_0044d350(int px, int py)
{
    int dx = abs(px - x);
    int dy = abs(py - y);
    int d;
    if (dx > dy)
        d = dy * 7 + dx * 18;
    else
        d = dx * 7 + dy * 18;
    if (d < radius)
        return 0;
    return d - radius;
}
