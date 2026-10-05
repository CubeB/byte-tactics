// Decompiled by Opus. Names are provisional.

struct Vec3 {
    int x;
    int y;
    int z;
};

static inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

#pragma pack(push, 1)
struct Object {
    char unknown_0[0x6a];
    Vec3 pos;                        // +0x6a
};
#pragma pack(pop)

Vec3 __stdcall GetPieceOffset(Object* obj, int param);

// FUNCTION: 0x43e060
Vec3 __stdcall GetPiecePosition(Object* obj, int param)
{
    return obj->pos + GetPieceOffset(obj, param);
}
