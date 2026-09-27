// Decompiled by Opus. Names are provisional.

struct Vec3f_004b6f70 {
    float x;
    float y;
    float z;
};

// Cross product a x b, passed and returned by value. The (float) casts on the
// differences emit nothing, but without them MSVC copies r to the return
// buffer with the stores interleaved (three registers instead of four).
// FUNCTION: 0x4b6f70
Vec3f_004b6f70 __stdcall FUN_004b6f70(Vec3f_004b6f70 a, Vec3f_004b6f70 b)
{
    float yz = a.y * b.z;
    float zy = a.z * b.y;
    float zx = a.z * b.x;
    float xz = a.x * b.z;
    float xy = a.x * b.y;
    float yx = a.y * b.x;
    Vec3f_004b6f70 r;
    r.z = (float)(xy - yx);
    r.x = (float)(yz - zy);
    r.y = (float)(zx - xz);
    return r;
}
