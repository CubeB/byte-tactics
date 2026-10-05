// Decompiled by Opus. Names are provisional.
// The length of an integer vector, truncated to an int.
#include <math.h>

struct Vec3i_0049a850 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

// FUNCTION: 0x49a850
int __stdcall VectorLength(Vec3i_0049a850* v)
{
    double x = v->x;
    double y = v->y;
    double z = v->z;
    return (int)sqrt(x * x + y * y + z * z);
}
