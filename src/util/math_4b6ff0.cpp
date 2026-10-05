// Decompiled by Opus. Names are provisional.
#include <math.h>

struct Vec3f_004b6ff0 {
    float x;
    float y;
    float z;
};

// Returns v scaled to unit length. The vector is passed and returned by value
// (the results are spilled into v's own stack slots before being copied out).
// Keeping the squares in their own float locals is what makes MSVC schedule
// the x87 loads as in the original; an inline x*x + y*y + z*z hoists the
// loads for the divisions one step earlier.
// FUNCTION: 0x4b6ff0
Vec3f_004b6ff0 __stdcall NormalizeVector(Vec3f_004b6ff0 v)
{
    Vec3f_004b6ff0 sq;
    sq.x = v.x * v.x;
    sq.y = v.y * v.y;
    sq.z = v.z * v.z;
    float len = sqrt(sq.x + sq.y + sq.z);
    v.x /= len;
    v.y /= len;
    v.z /= len;
    return v;
}
