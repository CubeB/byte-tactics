// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL. Air-movement counterpart of the ground mover 0x43cd20 (see
// 0x43dd20, which dispatches on unit->type->flags bit 11). The object is the
// same 0x2f-byte behaviour holder as 0x43dc00/0x43de30.
//
// Semantics recovered from the disassembly; what still differs is the frame
// and the register allocation of the x87 block: the original's floating point
// stack keeps f18, the heading step and the two scaled values live across the
// _hypot call, and the two dead stores at 0x43d4ae/0x43d4be into the delta
// Vec3 (overwritten at 0x43d6a1/0x43d6ab) are not reproducible from the
// expressions below, so the initial delta computation is still wrong.

#include <math.h>

#pragma pack(push, 1)

struct Vec3 {
    int x, y, z;
    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Short3 {
    short x, y, z;
};

struct UnitType_0043d290 {
    char unknown_0[0x192];
    int field_192;                              // +0x192
    char unknown_196[0x19a - 0x196];
    int field_19a;                              // +0x19a
    int field_19e;                              // +0x19e
    char unknown_1a2[0x1ba - 0x1a2];
    unsigned short max_turn;                    // +0x1ba
};

struct Unit_0043d290 {
    char unknown_0[0x64];
    Short3 f64;                                 // +0x64
    Vec3 pos;                                   // +0x6a
    char unknown_76[0x82 - 0x76];
    int field_82;                               // +0x82
    char unknown_86[0x92 - 0x86];
    UnitType_0043d290* type;                    // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags_0 : 16;                  // +0x110
    unsigned int moved : 1;                     // +0x110 bit 16
    unsigned int flags_17 : 15;
};

struct Game_0043d290 {
    char unknown_0[0x142b7];
    int field_142b7;                            // +0x142b7
};

class Class_0043d210 {
public:
    char unknown_0[8];
    Vec3 p1;                                    // +0x8, velocity
    Vec3 p2;                                    // +0x14
    int field_20;                               // +0x20, distance accumulator
    short field_24;                             // +0x24, heading step
    int field_26;                               // +0x26
    char unknown_2a[4];                         // +0x2a
    unsigned char mode : 2;                     // +0x2e bits 0-1
    unsigned char flag : 1;                     // +0x2e bit 2
    unsigned char rest : 5;

    void* obj;                                  // +0x0

    void FUN_0043d0d0(Unit_0043d290* unit, Vec3* v);
    void FUN_0043d290(Unit_0043d290* unit);
};

#pragma pack(pop)

// The path/steering object behind this->obj. Slot 4 (vtable +0x10) fills two
// Vec3-ish triples and a short heading.
class Iface_0043d290 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3(Vec3* a, Vec3* b, short* heading);
};

extern Game_0043d290* g_game;

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// FUNCTION: 0x43d290
void Class_0043d210::FUN_0043d290(Unit_0043d290* unit)
{
    if (mode != 2) {
        p1.x = 0;
        field_20 = 0;
        field_24 = 0;
        p1.y = 0;
        p1.z = 0;
        return;
    }

    Vec3 old = p1;
    Vec3 a;
    Vec3 b;
    short heading;
    ((Iface_0043d290*)obj)->v3(&a, &b, &heading);

    UnitType_0043d290* type = unit->type;
    const float eps = 1.52587890625e-05f;

    float f18 = (float)type->field_19e * eps;
    int q = (int)(((__int64)type->field_19e << 16) / type->field_192);
    int scale = 0x10000 - q;
    p1.x = (int)(((__int64)p1.x * scale) >> 16);
    p1.y = (int)(((__int64)p1.y * scale) >> 16);
    p1.z = (int)(((__int64)p1.z * scale) >> 16);

    float dist = (float)_hypot(p1.x, p1.z) * eps;
    float maxd = (float)type->field_19a * eps;
    if (dist > maxd) {
        int f = (int)((double)(maxd / dist) * 65536.0);
        p1.x = (int)(((__int64)p1.x * f) >> 16);
        p1.z = (int)(((__int64)p1.z * f) >> 16);
        int g = (int)((double)(dist - maxd) * 65536.0);
        short h = unit->f64.y;
        p1.x -= FUN_004b70ef(h, g);
        p1.z -= FUN_004b7123(h, g);
    }

    int dy = unit->pos.y - a.y;
    if (unit->field_82 != g_game->field_142b7) {
        int lim;
        if ((field_20 & 0xfffffffc) < 0x40000)
            lim = 0x10000;
        else
            lim = field_20 >> 2;
        if (dy <= -lim)
            p1.y = lim;
        else if (dy >= lim)
            p1.y = -lim;
        else
            p1.y = -dy;
    }

    float hd = (float)_hypot(unit->pos.x - a.x, unit->pos.z - a.z) * eps;
    short d = (short)(heading - unit->f64.y);
    if (d == 0) {
        field_24 = 0;
    } else {
        unsigned short max = type->max_turn;
        if (d >= (int)max)
            field_24 = max;
        else if (d <= -(int)max)
            field_24 = (short)-max;
        else
            field_24 = d;
        unit->f64.y = (short)(unit->f64.y + field_24);
        unit->moved = 1;
    }

    if (hd < 8.0f)
        hd = 8.0f;

    float k = -(float)sqrt((2.0f * f18) / hd);
    float vx = (float)(p1.x - b.x) * k * eps - (float)(unit->pos.x - a.x) * eps;
    float vz = (float)(p1.z - b.z) * k * eps - (float)(unit->pos.z - a.z) * eps;
    float mag = (float)_hypot(vx, vz);
    if (f18 < mag) {
        vx = vx * (f18 / mag);
        vz = vz * (f18 / mag);
    }

    p1.x += (int)((double)vx * 65536.0);
    p1.z += (int)((double)vz * 65536.0);
    field_20 = (int)sqrt((double)(p1.x * p1.x + p1.y * p1.y + p1.z * p1.z));

    Vec3 delta;
    delta.x = p1.x - old.x;
    delta.y = p1.y - old.y;
    delta.z = p1.z - old.z;
    FUN_0043d0d0(unit, &delta);
}
