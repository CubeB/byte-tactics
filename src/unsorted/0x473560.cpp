// Decompiled by Opus. Names are provisional.
// Moves a position by its velocity (an inlined Vec3 operator+=) and steps a
// frame counter.

struct Vec3_00473560 {
    int x;
    int y;
    int z;

    Vec3_00473560& operator+=(const Vec3_00473560& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

class Class_00473560 {
public:
    int unknown_0;
    Vec3_00473560 pos;                 // +0x04
    char unknown_10[0x1c - 0x10];
    Vec3_00473560 vel;                 // +0x1c
    int count;                         // +0x28
    int index;                         // +0x2c

    void FUN_00473560();
};

// FUNCTION: 0x473560
void Class_00473560::FUN_00473560()
{
    pos += vel;
    index = (index + 1) % count;
}
