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

int* __stdcall FindTargetableProjectile(Unit* unit, unsigned int weapon);
int __stdcall FindWeaponTarget(Unit* unit, unsigned int weapon, int param_3);
void __stdcall SetWeaponTargetUnit(Unit* unit, int param_2, unsigned int weapon);
void __stdcall SetWeaponTargetPos(Unit* unit, int* param_2, unsigned int weapon);
void __stdcall ClearWeaponTarget(Unit* unit, unsigned int weapon);

// FUNCTION: 0x408920
void __stdcall RetargetWeapon(Unit* unit, unsigned int weapon)
{
    if (unit->weapons[weapon].def->flag30) {
        int* p = FindTargetableProjectile(unit, weapon);
        if (p)
            SetWeaponTargetPos(unit, p + 1, weapon);
        else
            ClearWeaponTarget(unit, weapon);
    } else if ((unit->flags & 0x300000) == 0x200000) {
        int r = FindWeaponTarget(unit, weapon, 1);
        if (r)
            SetWeaponTargetUnit(unit, r, weapon);
        else
            ClearWeaponTarget(unit, weapon);
    }
}
