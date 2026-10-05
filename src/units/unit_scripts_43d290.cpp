// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// Steering update for the mover object (Class_0043d210): in mode 2 it asks the
// path object for a target position `a`, a target velocity `b` and a heading,
// damps the velocity p1, clamps its horizontal speed (bleeding the excess off
// along the unit's heading), turns the unit towards the heading, then steers
// p1 towards the target: k * (pos - a) - (p1 - b), limited to the acceleration
// f18. Finally it stores the new speed and hands the velocity change on.
//
// What made it match (Claude Opus 5.5; earlier attempts sat at 91.9%):
//  - The steering term is (pos - a) * k - (p1 - b). Earlier versions had the
//    two deltas the other way round, which compiled to the same x87 shape
//    with the operands loaded from each other's slots.
//  - The two deltas are 12-byte Vec3 locals: dax/daz sit 8 bytes apart with
//    the clamp factor's high dword between them (da shares f's slot), and
//    dbx/dbz sit in the final delta's slot. That needs delta in its own
//    block, so the address-taken local can share a slot.
//  - The fixed-point helpers take the component by reference: MulFixed for
//    Scale and the speed clamp, AddFixed for the final x87 adds. Written
//    inline, the clamp's _allmul pushed the component before the factor and
//    the tail forwarded p1.x in a register instead of reloading it.
//  - da is assigned x, y, z in order, and <memory.h> is needed: without it
//    the |v| > f18 rescale keeps vx and vz in memory (98.2%). A dummy
//    declaration scan shows two states repeating every 512 symbols.
#include <math.h>
#include <memory.h>

#pragma pack(push, 1)

static inline void MulFixed(int& v, int f)
{
    v = (int)(((__int64)v * f) >> 16);
}

struct Vec3 {
    int x, y, z;
    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
    int Length() const {
        double a = x, b = y, c = z;
        return (int)sqrt(a * a + b * b + c * c);
    }
    Vec3 operator-(const Vec3& o) const {
        Vec3 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    Vec3& operator+=(const Vec3& o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    void Scale(int s) {
        MulFixed(x, s);
        MulFixed(y, s);
        MulFixed(z, s);
    }
};

struct Short3 {
    short x, y, z;
};

struct UnitType_0043d290 {
    char unknown_0[0x192];
    int field_192; // +0x192
    char unknown_196[0x19a - 0x196];
    int field_19a; // +0x19a, top speed
    int field_19e; // +0x19e, acceleration
    char unknown_1a2[0x1ba - 0x1a2];
    unsigned short max_turn; // +0x1ba
};

struct Unit {
    char unknown_0[0x64];
    Short3 f64; // +0x64
    Vec3 pos;   // +0x6a
    char unknown_76[0x82 - 0x76];
    int field_82; // +0x82
    char unknown_86[0x92 - 0x86];
    UnitType_0043d290* type; // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags_0 : 16; // +0x110
    unsigned int moved : 1;    // +0x110 bit 16
    unsigned int flags_17 : 15;
};

struct Game_0043d290 {
    char unknown_0[0x142b7];
    int field_142b7; // +0x142b7
};

class Class_0043d210 {
  public:
    void* obj;              // +0x0
    int field_4;            // +0x4
    Vec3 p1;                // +0x8, velocity
    Vec3 p2;                // +0x14
    int field_20;           // +0x20, speed
    short field_24;         // +0x24, heading step
    int field_26;           // +0x26
    char unknown_2a[4];     // +0x2a
    unsigned char mode : 2; // +0x2e bits 0-1
    unsigned char flag : 1; // +0x2e bit 2
    unsigned char rest : 5;

    void FUN_0043d0d0(Unit* unit, Vec3* v);
};

// data/symbols.csv knows this method by the name its caller 0x43dd20 uses.
class Class_0043d290 : public Class_0043d210 {
  public:
    void FUN_0043d290(Unit* unit);
};

#pragma pack(pop)

// The path/steering object behind this->obj. Slot 4 (vtable +0x10) fills two
// Vec3-ish triples and a short heading.
class Iface_0043d290 {
  public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(Vec3* a, Vec3* b, short* heading);
};

extern Game_0043d290* g_game;

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

static inline void AddFixed(int& v, float f)
{
    v += (int)((double)f * 65536.0);
}

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// FUNCTION: 0x43d290
void Class_0043d290::FUN_0043d290(Unit* unit) {
    if (mode != 2) {
        p1 = Vec3(0, 0, 0);
        field_20 = 0;
        field_24 = 0;
        return;
    }

    Vec3 old = p1;
    Vec3 a;
    Vec3 b;
    short heading;
    ((Iface_0043d290*)obj)->v4(&a, &b, &heading);

    UnitType_0043d290* type = unit->type;
    const float eps = 1.52587890625e-05f;

    float f18 = (float)type->field_19e * eps;
    int q = (int)(((__int64)type->field_19e << 16) / type->field_192);
    int scale = 0x10000 - q;
    p1.Scale(scale);

    float dist = (float)_hypot(p1.x, p1.z) * eps;
    float maxd = (float)unit->type->field_19a * eps;
    if (dist > maxd) {
        int f = (int)((double)(maxd / dist) * 65536.0);
        MulFixed(p1.x, f);
        MulFixed(p1.z, f);
        int g = (int)((double)(dist - maxd) * 65536.0);
        p1 += Offset(unit->f64.y, g);
    }

    Vec3 da = unit->pos - a;
    Vec3 db = p1 - b;

    if (unit->field_82 != g_game->field_142b7) {
        int lim;
        if ((field_20 & -4) < 0x40000)
            lim = 0x10000;
        else
            lim = field_20 >> 2;
        if (da.y <= -lim)
            p1.y = lim;
        else if (da.y >= lim)
            p1.y = -lim;
        else
            p1.y = -da.y;
    }

    float hd = (float)_hypot(da.x, da.z) * eps;
    short d = (short)(heading - unit->f64.y);
    if (d != 0) {
        unsigned short max = unit->type->max_turn;
        if (d >= (int)max)
            field_24 = max;
        else if (d <= -(int)max)
            field_24 = (short)-max;
        else
            field_24 = d;
        unit->f64.y = (short)(unit->f64.y + field_24);
        unit->moved = 1;
    } else {
        field_24 = 0;
    }

    if (hd < 8.0f)
        hd = 8.0f;

    float k = -sqrt((2.0f * f18) / hd);
    float vx = (float)da.x * k * eps - (float)db.x * eps;
    float vz = (float)da.z * k * eps - (float)db.z * eps;
    float mag = (float)_hypot(vx, vz);
    if (mag > f18) {
        vx = vx * (f18 / mag);
        vz = vz * (f18 / mag);
    }

    AddFixed(p1.x, vx);
    AddFixed(p1.z, vz);
    field_20 = p1.Length();

    {
        Vec3 delta = p1 - old;
        FUN_0043d0d0(unit, &delta);
    }
}
