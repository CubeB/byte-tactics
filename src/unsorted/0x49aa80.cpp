// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>   // only for the register allocation it nudges (60.1 -> 61.8)
//
// Partial. Offsets, branches, bit tests, the team/height check and both
// squared-distance multiplies are right; the first basic block does not match.
// The original keeps the weapon def in ebx and the two positions in esi/edi
// across both __allmul calls, spills the high half of dx (0x10 bytes of
// locals) and only reloads the positions in the last block. Our build keeps
// the whole of dx in registers, spills only dz (8 bytes), reloads the
// positions earlier and recomputes the z difference from stack temps.
// Source spellings tried without changing that allocation: int vs __int64
// locals, a named distance, an inlined SquaredDistance helper (both by value
// and by pointer), operator-/Square methods, the array access inlined at each
// use, and several std headers (best 61.8% with <windows.h>).
//
// PURPOSE (read from the callers): a unit can fire the weapon in slot
// (a4 & 0xff) at a point when the ground distance in 16.16 fixed point,
// (dx*dx >> 32) + (dz*dz >> 32), is within the weapon range squared, the
// weapon is not flagged out, the shooter's team/height test passes and the
// line-of-fire helper returns a real angle. Callers pass the unit in a1, the
// unit's own position at a1+0x6a, the target point in a2/a3 and 0 or a slot
// index in a4.

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

struct Unit_0049aa80 {
    char unknown_0[8];
    Weapon_0049aa80 weapons[3];        // +0x8, stride 0x1c
    char unknown_5c[0x92 - 0x5c];
    UnitDef_0049aa80* def;             // +0x92
};

struct Game_0049aa80 {
    char unknown_0[0x1427f];
    unsigned char field_1427f;         // +0x1427f
};

#pragma pack(pop)

extern Game_0049aa80* g_game;

short __stdcall FUN_0049a890(int dx, int dy, int dz, int a, int b);

// FUNCTION: 0x49aa80
int __stdcall FUN_0049aa80(Unit_0049aa80* a1, Vec3_0049aa80* a2, Vec3_0049aa80* a3, int a4)
{
    WeaponDef_0049aa80* wdef = a1->weapons[a4 & 0xff].def;

    __int64 dx = a3->x - a2->x;
    __int64 dz = a3->z - a2->z;
    if ((int)(dx * dx >> 32) + (int)(dz * dz >> 32) > wdef->range * wdef->range)
        return 0;

    if (wdef->flags.bit16)
        return 1;

    if (a2->y.parts.whole + a1->def->field_170 <= g_game->field_1427f)
        return 0;

    if (wdef->flags.bit1) {
        if (FUN_0049a890(a2->x - a3->x, a2->y.value - a3->y.value, a2->z - a3->z,
                         wdef->field_68, wdef->field_c8) == (short)0x8000)
            return 0;
    }
    return 1;
}
