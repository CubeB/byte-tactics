// Decompiled by Space Bunny Free. Names are provisional.
// Virtual method (slot 6 of the vtable at 0x4fd5f8): stores the two points it
// is given, the vector between them scaled to a length of 32768 (2^31/len as
// 16.16), and the two arguments it passes on, then updates itself.
// Three source shapes are needed for the code to line up: the difference must
// come from an inline `operator-` returning the struct by value, the length
// must convert each component into its own double local, and the scaling must
// be a `Scale` member, so the three components are reached through the
// vector's address instead of through `this`.
#include <math.h>

struct Vec3_00474760 {
    int x;                          // 16.16 fixed point
    int y;
    int z;

    int Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
    void Scale(int s)               // s is 16.16 fixed point
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
};

static inline Vec3_00474760 operator-(const Vec3_00474760& p, const Vec3_00474760& q)
{
    Vec3_00474760 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

class Class_00471d70 {
public:
    char unknown_0[4];
    int field_4;                    // +0x04

    void FUN_00471d70(int param_1);
};

class Class_00474760 {
public:
    virtual void unused0();                      // slot 0
    virtual void unused1();                      // slot 1
    virtual void unused2(int);                   // slot 2
    virtual int unused3();                       // slot 3
    virtual void FUN_00474880();                 // slot 4

    int field_4;                                 // +0x04
    char unknown_8[0x1c - 0x08];
    int field_1c;                                // +0x1c
    Vec3_00474760 pos_a;                         // +0x20
    Vec3_00474760 pos_b;                         // +0x2c
    Vec3_00474760 dir;                           // +0x38
    int field_44;                                // +0x44

    virtual void FUN_00474760(Vec3_00474760* a, Vec3_00474760* b, int param_3,
                              int param_4, int param_5);
};

// FUNCTION: 0x474760
void Class_00474760::FUN_00474760(Vec3_00474760* a, Vec3_00474760* b, int param_3,
                                  int param_4, int param_5)
{
    ((Class_00471d70*)this)->FUN_00471d70(param_4);
    field_1c = param_3;
    pos_a = *a;
    pos_b = *b;
    dir = pos_b - pos_a;
    // The divisor is twice the length, with no test for zero: two identical
    // points raise a divide exception here (0x474811, _alldiv).
    int scale = 0x100000000 / (int)(((__int64)dir.Length() * 0x20000) >> 16);
    dir.Scale(scale);
    field_44 = param_5;
    FUN_00474880();
}
