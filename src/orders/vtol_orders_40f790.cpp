// Decompiled by Opus. Names are provisional.
// Adds two integer 3-vectors and returns the sum by value.

struct Vec3_0040f790 {
    int x;
    int y;
    int z;
};

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x40f790
Vec3_0040f790 __stdcall AddVec3(const Vec3_0040f790& a, const Vec3_0040f790& b)
{
    Vec3_0040f790 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}
