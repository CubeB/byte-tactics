// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, finished by Space Bunny Free.
// finished by Space Bunny Free.
// Names are provisional.
#include <windows.h>
#include <stdio.h>      // tools/headers.py: this header set on top of
                        // <windows.h> gives the original's operand order in the
                        // height check (math.h, memory.h, vector, map and list
                        // do too; stdio.h is the smallest). With <windows.h>
                        // alone the function is 5 instructions short there.

#pragma pack(push, 1)

union Fixed_0049aa80 {
    int value;
    struct { unsigned short fraction; short whole; } parts;
};

struct Vec3_0049aa80 {
    int x;                             // +0x0 (16.16 fixed point)
    Fixed_0049aa80 y;                  // +0x4
    int z;                             // +0x8
};

struct WeaponDef_0049aa80 {
    char unknown_0[0x68];
    int field_68;                      // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                      // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                         // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;         // tested here (line of fire)
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;        // tested here (skip the team check)
        unsigned int bit17_31 : 15;
    } flags;                           // +0x111
};

struct Weapon_0049aa80 {
    char unknown_0[8];
    WeaponDef_0049aa80* def;           // +0x8
    char unknown_c[0x1c - 0xc];
};

struct UnitDef_0049aa80 {
    char unknown_0[0x170];
    short field_170;                   // +0x170
};

struct Unit {
    char unknown_0[8];
    Weapon_0049aa80 weapons[3];        // +0x8, stride 0x1c
    char unknown_5c[0x92 - 0x5c];
    UnitDef_0049aa80* def;             // +0x92
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char field_1427f;         // +0x1427f
};

#pragma pack(pop)

extern Game* g_game;

short __stdcall SolveLaunchAngle(int dx, int dy, int dz, int a, int b);

// The subtraction of two points is a real by-value operator taking both
// operands by value and returning the point by value, and the line-of-fire test
// goes through a by-value helper, both exactly as in the matching sibling
// 0x49abb0 (whose MATCH this idiom produced). Those two boundaries are what put
// a2 in esi and a3 in edi for the whole function and keep the weapon def in
// ebx, which is the first block's register allocation (see the pass note at the
// top of the file).
inline Vec3_0049aa80 operator-(Vec3_0049aa80 a, Vec3_0049aa80 b)
{
    Vec3_0049aa80 r;
    r.x = b.x - a.x;
    r.y.value = b.y.value - a.y.value;
    r.z = b.z - a.z;
    return r;
}

static inline short LineOfFire_0049aa80(Vec3_0049aa80 to, Vec3_0049aa80 from, int s, int f)
{
    Vec3_0049aa80 d = from - to;
    return SolveLaunchAngle(d.x, d.y.value, d.z, s, f);
}

// FUNCTION: 0x49aa80
int __stdcall WeaponCanReachPos(Unit* a1, Vec3_0049aa80* a2, Vec3_0049aa80* a3, int a4)
{
    WeaponDef_0049aa80* wdef = a1->weapons[a4 & 0xff].def;

    // z difference first, as named int locals: the original's order.
    int dz = a3->z - a2->z;
    int dx = a3->x - a2->x;
    if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32) > wdef->range * wdef->range)
        return 0;

    if (wdef->flags.bit16)
        return 1;

    if (a2->y.parts.whole + a1->def->field_170 <= g_game->field_1427f)
        return 0;

    if (wdef->flags.bit1) {
        if (LineOfFire_0049aa80(*a2, *a3, wdef->field_68, wdef->field_c8) == (short)0x8000)
            return 0;
    }
    return 1;
}
