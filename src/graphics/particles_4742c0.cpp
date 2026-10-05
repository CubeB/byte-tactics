// Decompiled by space-bunny-free. Names are provisional.
// Sibling of 0x474d50 and 0x475150: same base call, same five virtuals, same
// trailing virtual call. It keeps the two points it is given, puts their
// difference in a third one and scales that by the 16.16 reciprocal of the id
// ((1 << 32) / (id << 16) is 65536 / id), so the offset ends up divided by it.

struct Vec3_004742c0 {
    int x;
    int y;
    int z;

    Vec3_004742c0 operator-(const Vec3_004742c0& o) const
    {
        Vec3_004742c0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }

    // s is 16.16 fixed point. MSVC 5 only orders the two __allmul arguments
    // the way the original does (and keeps the cdq that sign extends the
    // quotient) when the three components go through one inlined method.
    void Scale(int s)
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
};

class ParticleSystem {
public:
    void SetLifetime(int param_1);
};

class ThrustParticles {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    char unknown_4[0x1c - 4];
    int unknown_1c;                 // +0x1c
    Vec3_004742c0 unknown_20;       // +0x20
    Vec3_004742c0 unknown_2c;       // +0x2c
    Vec3_004742c0 unknown_38;       // +0x38
    void FUN_004742c0(Vec3_004742c0* p, Vec3_004742c0* q, int a, int b);
};

// FUNCTION: 0x4742c0
void ThrustParticles::FUN_004742c0(Vec3_004742c0* p, Vec3_004742c0* q, int a, int b)
{
    ((ParticleSystem*)this)->SetLifetime(b);
    unknown_1c = a;
    unknown_20 = *p;
    unknown_2c = *q;
    unknown_38 = unknown_2c - unknown_20;
    int scale = (int)(((__int64)1 << 32) / (b << 16));
    unknown_38.Scale(scale);
    v4();
}
