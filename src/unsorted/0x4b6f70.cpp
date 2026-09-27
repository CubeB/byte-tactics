// Decompiled by Opus. Names are provisional.

struct Vec3f_004b6f70 {
    float x;
    float y;
    float z;
};

// Cross product a x b, passed and returned by value.
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
    r.z = xy - yx;
    r.x = yz - zy;
    r.y = zx - xz;
    return r;
}
