// Decompiled by Opus. Names are provisional.

struct Vec3f_004b6eb0 {
    float x;
    float y;
    float z;
};

// Difference to - from, passed and returned by value.
// FUNCTION: 0x4b6eb0
Vec3f_004b6eb0 __stdcall FUN_004b6eb0(Vec3f_004b6eb0 from, Vec3f_004b6eb0 to)
{
    Vec3f_004b6eb0 r;
    r.x = to.x - from.x;
    r.y = to.y - from.y;
    r.z = to.z - from.z;
    return r;
}
