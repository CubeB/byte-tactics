// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 91.3%, exactly 1074 bytes like the original. Frame is the right
// 0x48 bytes and the mode!=2 early return matches (ebp/ebx are pushed inside
// the mode==2 arm, as the original does at 0x43d2c3).
//
// Two wins this session, both pure register-allocation levers:
//
//   1. Declare `day` (the y delta) FIRST among the five deltas:
//        day = pos.y - a.y; dax = pos.x - a.x; daz = pos.z - a.z; dbx =
//        p1.x - b.x; dbz = p1.z - b.z;
//      The original loads pos.y into eax before dax is formed (0x43d48b) and
//      spells the two subtractions in the order dax-then-day at
//      0x43d480-0x43d49d. Putting the assignment of `day` first makes the whole
//      emitted delta block align: 88.3 -> 90.8. The order dax, daz, day is
//      88.3; the obvious dax, day, daz is 87.7.
//
//   2. Give Vec3::Scale explicit temporaries:
//        int a = x, b = y, c = z;
//        x = (int)(((__int64)a * s) >> 16);   (then b, then c)
//      This is what fixes the THIRD inlined _allmul: without it the p1.z
//      multiply pushes edx,eax then ebp,ebx while the original pushes ebp,ebx
//      then edx,eax (0x43d374). With the temporaries all three multiplies push
//      the scale pair first: 90.8 -> 91.3. (Only the p1.z case changes; the
//      x/y multiplies already matched.)
//
// Earlier wins (kept): writing the steering delta through an inline
// `Vec3 operator-(const Vec3&)`: `Vec3 delta = p1 - old;` (1082 -> 1074 bytes
// and 83.7 -> 87.7). Written as three `delta.x = p1.x - old.x;`-style
// assignments the compiler CSEs the just-stored p1.x/p1.z out of the two
// `p1.x += ...` / `p1.z += ...` statements across the inlined Length() and
// emits keeps plus an extra spill. The operator- form reloads both operands,
// matching the original's `add dword ptr [edi+0x10], eax` and fresh `[esi]`
// loads.
//
// Also confirmed: the speed clamp must be `if (mag > f18)` to keep the _hypot
// result in st(0) and give the original `fcom [esp+0x28] / test ah,0x41 / jne`.
//
// What still differs (38 instructions, one cause in each of three places):
//   * the four delta locals get different stack homes (ours dax 0x14, daz
//     0x5c, dbx 0x20, dbz 0x1c; original dax 0x1c, daz 0x24, dbx 0x40, dbz
//     0x48), so every fild/fst operand in the leveling and k blocks is off.
//     The original keeps fresh slots for dax/daz (E-0x3c/E-0x34) while ours
//     reuses the dead `dist` (E-0x44) and `maxd`/`g` (E+4) slots, and the
//     original's f.hi temporary then lands at E-0x38, ours at E-0x34. This is
//     the one cluster I could not move. Modelling the deltas as Vec3 copies,
//     as separate ints declared early, with an initialiser, as `unsigned`, or
//     in every assignment order all left the slots unchanged.
//   * the f-scaling multiply at 0x43d3f5 (_allmul(p1.x, f)) pushes
//     component-then-f in our build where the original pushes f-then-component,
//     for both p1.x and p1.z. A ScaleXZ(__int64) helper, explicit temporaries,
//     and swapping the operands in source were all neutral.
//   * the tail forwards p1.x into ebp across the inlined Length() (`mov eax,
//     ebp`) where the original reloads `mov eax, [esi]`.
// Previously tried and worse or neutral: moving the dax/day/daz assignments
// before the distance check (66%, changes code order), `(__int64)s * x` in
// Scale (neutral), the scale computation as one expression (neutral), int/
// __int64 f (neutral), swapping the f-multiply operands (neutral), explicit
// three-statement scaling instead of the Scale method (66%), integer hypot for
// the distance, assigning the sqrt result to k first, inlining `h` (86.2%),
// building the deltas as Vec3 subtractions (82.5-87.0%), declaring h before g
// (89.9%), `lim` hoisted out of its block (neutral), and giving ScaleXZ its
// own method (89.5%).

#include <math.h>

#pragma pack(push, 1)

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
    void Scale(int s) {
        int a = x, b = y, c = z;
        x = (int)(((__int64)a * s) >> 16);
        y = (int)(((__int64)b * s) >> 16);
        z = (int)(((__int64)c * s) >> 16);
    }
};

struct Short3 {
    short x, y, z;
};

struct UnitType_0043d290 {
    char unknown_0[0x192];
    int field_192; // +0x192
    char unknown_196[0x19a - 0x196];
    int field_19a; // +0x19a
    int field_19e; // +0x19e
    char unknown_1a2[0x1ba - 0x1a2];
    unsigned short max_turn; // +0x1ba
};

struct Unit_0043d290 {
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
    int field_20;           // +0x20, distance accumulator
    short field_24;         // +0x24, heading step
    int field_26;           // +0x26
    char unknown_2a[4];     // +0x2a
    unsigned char mode : 2; // +0x2e bits 0-1
    unsigned char flag : 1; // +0x2e bit 2
    unsigned char rest : 5;

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
    virtual void v3();
    virtual void v4(Vec3* a, Vec3* b, short* heading);
};

extern Game_0043d290* g_game;

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// FUNCTION: 0x43d290
void Class_0043d210::FUN_0043d290(Unit_0043d290* unit) {
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
        __int64 f = (int)((double)(maxd / dist) * 65536.0);
        p1.x = (int)(((__int64)p1.x * f) >> 16);
        p1.z = (int)(((__int64)p1.z * f) >> 16);
        int g = (int)((double)(dist - maxd) * 65536.0);
        short h = unit->f64.y;
        int r1 = -FUN_004b70ef(h, g);
        int r2 = -FUN_004b7123(h, g);
        p1.x += r1;
        p1.z += r2;
    }

    int day = unit->pos.y - a.y;
    int dax = unit->pos.x - a.x;
    int daz = unit->pos.z - a.z;
    int dbx = p1.x - b.x;
    int dbz = p1.z - b.z;

    if (unit->field_82 != g_game->field_142b7) {
        int lim;
        if ((field_20 & -4) < 0x40000)
            lim = 0x10000;
        else
            lim = field_20 >> 2;
        if (day <= -lim)
            p1.y = lim;
        else if (day >= lim)
            p1.y = -lim;
        else
            p1.y = -day;
    }

    float hd = (float)_hypot(dax, daz) * eps;
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
    float vx = (float)dbx * k * eps - (float)dax * eps;
    float vz = (float)dbz * k * eps - (float)daz * eps;
    float mag = (float)_hypot(vx, vz);
    if (mag > f18) {
        vx = vx * (f18 / mag);
        vz = vz * (f18 / mag);
    }

    p1.x += (int)((double)vx * 65536.0);
    p1.z += (int)((double)vz * 65536.0);
    field_20 = p1.Length();

    Vec3 delta = p1 - old;
    FUN_0043d0d0(unit, &delta);
}
