// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 87.7%, exactly 1074 bytes like the original. Frame is the right
// 0x48 bytes and the mode!=2 early return matches (ebp/ebx are pushed inside
// the mode==2 arm, as the original does at 0x43d2c3).
//
// The single biggest win this session (83.7 -> 87.7, and 1082 -> 1074 bytes)
// was writing the steering delta through an inline `Vec3 operator-(const
// Vec3&)`: `Vec3 delta = p1 - old;`. Written as three `delta.x = p1.x -
// old.x;`-style assignments the compiler CSEs the just-stored p1.x/p1.z out
// of the two `p1.x += ...` / `p1.z += ...` statements across the inlined
// Length() and emits `mov ebp,edx` / `mov edx,ecx` keeps plus an extra spill
// (1082 bytes). The operator- form reloads both operands, matching the
// original's `add dword ptr [edi+0x10], eax` and fresh `[esi]` loads. This is
// the shared upstream cause the brief warns about: one construct fixed the
// whole tail.
//
// Also confirmed: the speed clamp must be `if (mag > f18)` (not `mag < f18`
// nor `f18 < mag`) to keep the _hypot result in st(0) and give the original
// `fcom [esp+0x28] / test ah,0x41 / jne`. Vec3::Scale must scale `x * s`
// (the sibling 0x43d0d0.cpp uses the same order), which makes two of the
// three inlined _allmul calls push ebp,ebx (scale) before edx,eax (component)
// as the original does.
//
// What still differs (43 instructions, LCS 315/358, all in four clusters):
//   * the THIRD inlined Scale multiply (p1.z, original 0x43d374) still pushes
//     edx,eax then ebp,ebx while the original pushes ebp,ebx then edx,eax;
//     writing Scale as three explicit statements, reversing the operand,
//     making scale `__int64`, or wrapping the f-scaling in a `ScaleXZ(int)`
//     method (which DOES fix that call's order) did not move the count.
//   * the f-scaling multiply (original 0x43d3f5) pushes component-then-f in
//     our build; the `ScaleXZ` method fixes the first of its two calls but
//     not the p1.z one.
//   * the four delta locals get different homes (ours dax 0x14, daz 0x5c,
//     dbx 0x20, dbz 0x1c; original dax 0x1c, daz 0x24, dbx 0x40, dbz 0x48)
//     and so every fild/fst operand and branch displacement in the leveling
//     and k blocks is off by a constant. `h` is a `short` local that spills
//     and is re-read (matches the original's slot reuse) rather than
//     re-reading unit->f64.y.
//   * the tail forwards p1.x into a callee-saved register across Length()
//     (`mov eax, ebp`) where the original reloads `mov eax, [esi]`.
// Previously tried and worse or neutral: `(__int64)s * x` in Scale (neutral),
// int/__int64 f (neutral), swapping the f-multiply operands (neutral),
// explicit three-statement scaling instead of the Scale method (66%),
// explicit double casts, integer hypot for the distance, assigning the sqrt
// result to k first.

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
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
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

    int dax = unit->pos.x - a.x;
    int day = unit->pos.y - a.y;
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
