// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// A virtual method of the big class whose vtable starts at 0x4fd2f8 (it shares
// that vtable with 0x44e530, 0x44e3a0 and friends). It fetches a position from
// itself through vtable slot +0x20, measures the horizontal (x/z) distance to
// `arg`, converts it from 16.16 to pixels, then tests range/flags.
#include <math.h>
#include <stdlib.h>

struct Vec3_0044e5b0 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Object_0044e5b0 {
    char unknown_0[0x66];
    unsigned short heading;            // +0x66
    char unknown_68[2];
    Vec3_0044e5b0 pos;                 // +0x6a
};

class Class_0044e5b0 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8(Vec3_0044e5b0* out);   // vtable +0x20
    char unknown_4[4];                 // +0x4
    unsigned short flags;              // +0x8
    short field_a;                     // +0xa
    char unknown_c[0x1a - 0xc];        // +0xc
    Object_0044e5b0* target;           // +0x1a

    int FUN_0044e5b0(Object_0044e5b0* arg);
};
#pragma pack(pop)

// FUNCTION: 0x44e5b0
int Class_0044e5b0::FUN_0044e5b0(Object_0044e5b0* arg)
{
    Vec3_0044e5b0 pos;
    v8(&pos);
    float dist = (float)_hypot((double)(arg->pos.x - pos.x), (double)(arg->pos.z - pos.z)) * 1.52587890625e-05f;
    unsigned short flags = this->flags;
    if (flags & 0x10) {
        return (double)field_a > dist;
    }
    else {
        if (dist > 0.5f)
            return 0;
        if ((flags & 1) && target == 0)
            return 0;
        if ((flags & 4) && arg->heading != target->heading)
            return 0;
        if ((flags & 8) && abs(arg->pos.y - pos.y) > 0x10000)
            return 0;
        return 1;
    }
    return 0;
}
