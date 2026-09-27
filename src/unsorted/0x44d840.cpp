// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <stdlib.h>

class Class_0044d840 {
public:
    char unknown_0[8];
    short x;                           // +0x8
    short y;                           // +0xa
    int inner;                         // +0xc
    int outer;                         // +0x10

    int FUN_0044d840(int px, int py);
};

// Approximate distance from (px, py) to the ring around (x, y) with the
// inner radius +0xc and the outer radius +0x10, 0 between the radii.
// Same 18 * major + 7 * minor metric as 0x44d350.
// FUNCTION: 0x44d840
int Class_0044d840::FUN_0044d840(int px, int py)
{
    int dx = abs(px - x);
    int dy = abs(py - y);
    int d;
    if (dx > dy)
        d = dy * 7 + dx * 18;
    else
        d = dx * 7 + dy * 18;
    if (d > outer)
        return d - outer;
    if (d < inner)
        return inner - d;
    return 0;
}
