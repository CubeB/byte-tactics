// Decompiled by Opus. Names are provisional.
// Adds two integer 3-vectors and returns the sum by value.

struct Vec3_0040f790 {
    int x;
    int y;
    int z;
};

// FUNCTION: 0x40f790
Vec3_0040f790 __stdcall FUN_0040f790(const Vec3_0040f790& a, const Vec3_0040f790& b)
{
    Vec3_0040f790 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}
