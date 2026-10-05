// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct WeaponDef_00408920 {
    char unknown_0[0x111];
    unsigned int unknown_bits : 30;    // +0x111
    unsigned int flag30 : 1;
    unsigned int unknown_bit31 : 1;
};
#pragma pack(pop)

struct Weapon_00408920 {
    WeaponDef_00408920* def;           // +0x0
    char unknown_4[0x18];
};

struct Unit {
    char unknown_0[0x10];
    Weapon_00408920 weapons[3];        // +0x10
    char unknown_64[0x110 - 0x64];
    unsigned int flags;                // +0x110
};

int* __stdcall FUN_0049d120(Unit* unit, unsigned int weapon);
int __stdcall FUN_0040b7b0(Unit* unit, unsigned int weapon, int param_3);
void __stdcall FUN_0048a060(Unit* unit, int param_2, unsigned int weapon);
void __stdcall FUN_0048a0a0(Unit* unit, int* param_2, unsigned int weapon);
void __stdcall FUN_0048a0f0(Unit* unit, unsigned int weapon);

// FUNCTION: 0x408920
void __stdcall FUN_00408920(Unit* unit, unsigned int weapon)
{
    if (unit->weapons[weapon].def->flag30) {
        int* p = FUN_0049d120(unit, weapon);
        if (p)
            FUN_0048a0a0(unit, p + 1, weapon);
        else
            FUN_0048a0f0(unit, weapon);
    } else if ((unit->flags & 0x300000) == 0x200000) {
        int r = FUN_0040b7b0(unit, weapon, 1);
        if (r)
            FUN_0048a060(unit, r, weapon);
        else
            FUN_0048a0f0(unit, weapon);
    }
}
