// Decompiled by space-bunny-free. Names are provisional.
// Sibling of 0x4742c0: the same base call, the same two position copies, the
// same difference of the two and the same trailing virtual call, but this one
// divides the difference by how many 327680-unit segments its length holds
// instead of multiplying it by a reciprocal id.
//
// The Vec3 helpers have to stay inlined methods. Written as free expressions
// the compiler loads the three components in the order y, z, x (three filds
// before the squares are folded) and gives the three divisions one base
// register; as methods with the components in locals of their own, the loads
// come out in source order and the first division keeps the pointer it already
// has while the other two go back through `this`.
//
// The scale is the high half of the 32-bit 16.16 quotient, so that quotient
// has to sit in memory: writing it into a union of the int and a pair of shorts
// and reading the upper one gives the original's `mov dword ptr [esp+0x18],
// eax` followed by `movsx ecx, word ptr [esp+0x1a]`. An int local keeps the
// quotient in a register and compiles to `mov ecx, eax` and `sar ecx, 0x10`.
#include <math.h>

struct Vec3_004736e0 {
    int x;
    int y;
    int z;

    Vec3_004736e0 operator-(const Vec3_004736e0& o) const
    {
        Vec3_004736e0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    int Length() const
    {
        float fx = x;
        float fy = y;
        float fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
};

// 16.16 fixed point seen as the short above the short below.
union Fix_004736e0 {
    int whole;
    short half[2];
};

class Class_00471d70 {
public:
    void SetLifetime(int param_1);
};

class Class_004736e0 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    char unknown_4[0x1c - 4];
    int unknown_1c;                 // +0x1c
    Vec3_004736e0 pos1;            // +0x20
    Vec3_004736e0 pos2;            // +0x2c
    Vec3_004736e0 dir;             // +0x38
    void FUN_004736e0(Vec3_004736e0* a, Vec3_004736e0* b, int c);
};

// FUNCTION: 0x4736e0
void Class_004736e0::FUN_004736e0(Vec3_004736e0* a, Vec3_004736e0* b, int c)
{
    ((Class_00471d70*)this)->SetLifetime(c);
    pos1 = *a;
    pos2 = *b;
    dir = pos2 - pos1;
    int dist = dir.Length();
    Fix_004736e0 scale;
    scale.whole = (int)(((__int64)dist << 16) / 327680);
    int step = scale.half[1];
    unknown_1c = step;
    dir.x /= step;
    dir.y /= step;
    dir.z /= step;
    v4();
}
