// Decompiled by Opus. Names are provisional.
// Any header include matters here: with the larger symbol table MSVC loads
// the float parameter first (fld dx; fadd [ecx+4]) instead of the field.
#include <math.h>

class UnitResources {
public:
    char unknown_0[0x4];
    float x0;                          // +0x04
    float x1;                          // +0x08
    float x2;                          // +0x0c
    char unknown_10[0x1c - 0x10];
    float y0;                          // +0x1c
    float y1;                          // +0x20
    float y2;                          // +0x24
    int FUN_004011c0(float dx, float dy);
};

// FUNCTION: 0x4011c0
int UnitResources::FUN_004011c0(float dx, float dy)
{
    x0 += dx;
    y0 += dy;
    if (x2 <= 0.0f && y2 <= 0.0f) {
        x1 += dx;
        y1 += dy;
        return 1;
    }
    return 0;
}
