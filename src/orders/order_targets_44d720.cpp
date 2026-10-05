// Decompiled by Opus. Names are provisional.
// Virtual method of the ring area class (compare 0x44d2c0, which writes the
// same centre): writes the centre as 16.16 world coordinates, then moves it
// back towards the owner by the mean of the two radii at +0xc and +0x10.
// The mean needs its own inline helper to load +0xc first.

struct Point_0044d720 {
    short x;
    short y;
};

struct Vec3_0044d720 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 2)
struct Inner_0044d720 {
    char unknown_0[0x6a];
    Vec3_0044d720 pos;                 // +0x6a
    char unknown_76[0x7e - 0x76];
    Point_0044d720 cell;               // +0x7e
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Owner_0044d720 {
    char unknown_0[0xe];
    Inner_0044d720* inner;             // +0xe
};
#pragma pack(pop)

int __stdcall GetHeadingBetween(Vec3_0044d720* from, Vec3_0044d720* to);

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

static inline void ToWorld(Vec3_0044d720* out, Point_0044d720 a, Point_0044d720 b)
{
    out->x = (a.x * 2 + b.x) << 19;
    out->z = (a.y * 2 + b.y) << 19;
}

// Inlined copy of FUN_004103a0.
static inline Vec3_0044d720 Direction(short angle, int scale)
{
    Vec3_0044d720 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

class Class_0044d720 {
public:
    char unknown_0[4];
    Owner_0044d720* owner;             // +0x4
    Point_0044d720 pos;                // +0x8
    int radius1;                       // +0xc
    int radius2;                       // +0x10

    int FUN_0044d720(Vec3_0044d720* out);
};

static inline int MeanRadius(Class_0044d720* c)
{
    return (c->radius1 + c->radius2) / 2;
}

// FUNCTION: 0x44d720
int Class_0044d720::FUN_0044d720(Vec3_0044d720* out)
{
    Inner_0044d720* inner = owner->inner;
    ToWorld(out, pos, inner->cell);
    short angle = GetHeadingBetween(&inner->pos, out);
    Vec3_0044d720 d = Direction(angle, MeanRadius(this) << 16);
    out->x -= d.x;
    out->y -= d.y;
    out->z -= d.z;
    return 1;
}
