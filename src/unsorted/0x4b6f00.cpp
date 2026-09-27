// Decompiled by Opus. Names are provisional.

struct Vec3f_004b6f00 {
    float x;
    float y;
    float z;
};

struct Vec3i_004b6f00 {
    int x;
    int y;
    int z;
};

// FUNCTION: 0x4b6f00
Vec3f_004b6f00 __stdcall FUN_004b6f00(Vec3i_004b6f00 from, Vec3i_004b6f00 to)
{
    Vec3f_004b6f00 r;
    r.x = (float)(to.x - from.x);
    r.y = (float)(to.y - from.y);
    r.z = (float)(to.z - from.z);
    return r;
}
