// Decompiled by Opus. Names are provisional.
// As in 0x4011c0, the header include decides whether MSVC loads the float
// parameter or the field first.
#include <math.h>

// The caller (0x41bd10) sets ecx to the resource block and also pushes it,
// so this is a __thiscall method whose body only uses the explicit pointer.
class Class_00401180 {
public:
    char unknown_0[0x4];
    float x0;                          // +0x04
    float x1;                          // +0x08
    float x2;                          // +0x0c

    int FUN_00401180(Class_00401180* r, float amount);
};

// FUNCTION: 0x401180
int Class_00401180::FUN_00401180(Class_00401180* r, float amount)
{
    r->x0 += amount;
    if (r->x2 > 0.0f)
        return 0;
    r->x1 += amount;
    return 1;
}
