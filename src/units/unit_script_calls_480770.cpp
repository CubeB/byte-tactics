// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Slot 17 of UnitScript (vtable 0x4fd698), the derived class in
// src/units/units_485e30.cpp; the data at +0x540 holds the unit pointer at
// +0xc. The switch over the property id 1 to 20 matches the COB script "get"
// list (ACTIVATION, STANDINGMOVEORDERS, ..., ARMORED), with the property
// argument in the first stack parameter. The base class slot (0x4b0680)
// returns 0.
//
// <windows.h> plus <string> are load bearing for the compiler state, with no
// symbol from either used. Without them the two 16.16 packers (cases 7 and 9)
// compile `(x & 0xffff0000) + (z >> 16)` with the masked operand as the
// accumulator; the original has the shifted one there. `<windows.h>` alone and
// `<string>` alone do not flip it, `<windows.h>` + `<string>` does.
#include <windows.h>
#include <string>
#include <math.h>

#pragma pack(push, 1)
struct Def_00480770 {
    char unknown_0[0x16e];
    int field_16e;                      // +0x16e
    char unknown_172[0x1fa - 0x172];
    unsigned int maxHealth;             // +0x1fa
};

struct Unit {
    char unknown_0[0x66];
    short field_66;                     // +0x66
    char unknown_68[0x6a - 0x68];
    int posX;                           // +0x6a
    int posY;                           // +0x6e
    int posZ;                           // +0x72
    char unknown_76[0x92 - 0x76];
    Def_00480770* def;                  // +0x92
    char unknown_96[0x104 - 0x96];
    float field_104;                    // +0x104
    short health;                       // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char on : 1;               // +0x10e bit 0
    unsigned char on2 : 1;              // +0x10e bit 1
    unsigned char unknown_10e : 6;
    unsigned char bit0 : 1;             // +0x10f bit 0
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char unknown_10f : 4;
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Data_00480770 {
    int unknown_0;                      // +0x0
    int unknown_4;                      // +0x4
    int unknown_8;                      // +0x8
    Unit* unit;                         // +0xc
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                        // +0x14357
};
#pragma pack(pop)

struct Vec3_00480770 {
    int x;
    int y;
    int z;
};

extern Game* g_game;

Vec3_00480770 __stdcall GetPiecePosition(Unit* obj, int param);
int __stdcall GetGroundHeight(Vec3_00480770* pos);
int __cdecl FUN_004b715a(int a, int b);

static inline Unit* GetUnit(unsigned short id)
{
    if (id == 0)
        return 0;
    return &g_game->units[id];
}

class UnitScript {
public:
    char unknown_0[0x540];
    Data_00480770* data;                // +0x540

    int GetUnitValue(int which, int a, int b, int c, int d);
};

// FUNCTION: 0x480770
int UnitScript::GetUnitValue(int which, int a, int b, int c, int d)
{
    Unit* unit = data->unit;
    switch (which) {
    case 1:
        return unit->on;
    case 2:
        return (unit->flags >> 18) & 3;
    case 3:
        return (unit->flags >> 20) & 3;
    case 4:
        return unit->health * 100 / unit->def->maxHealth;
    case 5:
        return unit->bit0;
    case 6:
        return unit->bit1;
    case 7: {
        Vec3_00480770 v;
        v = GetPiecePosition(unit, a);
        return (v.x & 0xffff0000) + (v.z >> 16);
    }
    case 8: {
        Vec3_00480770 v;
        v = GetPiecePosition(unit, a);
        return v.y;
    }
    case 9: {
        Unit* u = GetUnit((unsigned short)a);
        if (u != 0 && (u->flags & 0x10000000))
            return (u->posZ >> 16) + (u->posX & 0xffff0000);
        break;
    }
    case 10: {
        Unit* u = GetUnit((unsigned short)a);
        if (u != 0 && (u->flags & 0x10000000))
            return u->posY;
        break;
    }
    case 11: {
        Unit* u = GetUnit((unsigned short)a);
        if (u != 0 && (u->flags & 0x10000000))
            return u->def->field_16e;
        break;
    }
    case 12: {
        int hi = a & 0xffff0000;
        int lo = a << 16;
        if (lo < 0)
            hi += 0x10000;
        return (unsigned short)(FUN_004b715a(hi, lo) - unit->field_66);
    }
    case 13: {
        int hi = a & 0xffff0000;
        int lo = a << 16;
        if (lo < 0)
            hi += 0x10000;
        return (int)_hypot((double)hi, (double)lo);
    }
    case 14:
        return FUN_004b715a(a, b) & 0xffff;
    case 15:
        return (int)_hypot((double)a, (double)b);
    case 16: {
        Vec3_00480770 v;
        v.x = a & 0xffff0000;
        v.z = a << 16;
        if (v.z < 0)
            v.x += 0x10000;
        return GetGroundHeight(&v) << 16;
    }
    case 17: {
        int result;
        if (unit->field_104 == 0.0f)
            result = 0;
        else
            result = 1 - (int)(unit->field_104 * -99.0f);
        return result;
    }
    case 18:
        return unit->bit2;
    case 19:
        return unit->bit3;
    case 20:
        return unit->on2;
    }
    return 0;
}
