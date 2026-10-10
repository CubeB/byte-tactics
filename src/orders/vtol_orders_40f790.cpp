// Decompiled by Opus. Names are provisional.
// Adds two integer 3-vectors and returns the sum by value.

#include "../util/vec3.h"

// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
struct Unit;

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x40f790
Vec3 __stdcall AddVec3(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}
