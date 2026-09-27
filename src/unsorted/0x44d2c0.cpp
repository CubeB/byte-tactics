// Decompiled by Opus. Names are provisional.
// Virtual method of the circle area class (compare 0x44d290 and 0x44d310,
// which use the same centre at +0x8): writes the centre, offset by a point
// of the owner, as 16.16 world coordinates.

struct Point_0044d2c0 {
    short x;
    short y;
};

struct Inner_0044d2c0 {
    char unknown_0[0x7e];
    Point_0044d2c0 pos;                // +0x7e
};

#pragma pack(push, 1)
struct Owner_0044d2c0 {
    char unknown_0[0xe];
    Inner_0044d2c0* inner;             // +0xe
};
#pragma pack(pop)

static inline void ToWorld(int* out, Point_0044d2c0 a, Point_0044d2c0 b)
{
    out[0] = (a.x * 2 + b.x) << 19;
    out[2] = (a.y * 2 + b.y) << 19;
}

class Class_0044d2c0 {
public:
    char unknown_0[4];
    Owner_0044d2c0* owner;             // +0x4
    Point_0044d2c0 pos;                // +0x8

    int FUN_0044d2c0(int* out);
};

// FUNCTION: 0x44d2c0
int Class_0044d2c0::FUN_0044d2c0(int* out)
{
    ToWorld(out, pos, owner->inner->pos);
    return 1;
}
