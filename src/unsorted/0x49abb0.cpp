// Decompiled by GPT-5.6-Terra. Names are provisional.
// Partial: logic and offsets match, but MSVC keeps the weapon and units in different
// registers and omits the original 0xc-byte local frame around both distance paths.
#pragma pack(push, 1)

struct WeaponDef {
    char unknown_0[0x68];
    int unknown_68;
    char unknown_6c[0xc8 - 0x6c];
    int unknown_c8;
    char unknown_cc[0xdc - 0xcc];
    int range;
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;
        unsigned int bit17 : 1;
        unsigned int bit18_31 : 14;
    } flags;
};

struct UnitDef {
    char unknown_0[0x170];
    short height;
    char unknown_172[0x241 - 0x172];
    struct {
        unsigned int bit0_11 : 12;
        unsigned int bit12 : 1;
        unsigned int bit13_18 : 6;
        unsigned int bit19 : 1;
        unsigned int bit20_31 : 12;
    } flags;
};

struct Unit {
    char unknown_0[0x10];
    struct WeaponSlot {
        WeaponDef *weapon;
        char unknown_4[0x1c - 4];
    } weapons[1];
    char unknown_2c[0x6a - 0x2c];
    union Position {
        struct {
            int x;
            int y;
            int z;
        } coords;
        struct {
            char unknown_6a[6];
            short altitude;
        } height;
    } position;
    char unknown_76[0x92 - 0x76];
    UnitDef *unit_def;
    char unknown_96[0x110 - 0x96];
    unsigned int state;
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char sea_level;
};

#pragma pack(pop)

extern Game *g_game;
extern short __stdcall FUN_0049a890(int, int, int, int, int);

// FUNCTION: 0x49abb0
int __stdcall FUN_0049abb0(Unit *unit1, Unit *unit2, unsigned char weapon)
{
    WeaponDef *weapon_def = unit1->weapons[weapon].weapon;

    if (weapon_def->flags.bit16) {
        if (!unit2->unit_def->flags.bit19 && unit2->position.height.altitude > g_game->sea_level)
            return 0;

        if (unit2->unit_def->flags.bit12 && unit2->position.height.altitude + (unit2->unit_def->height >> 1) > g_game->sea_level)
            return 0;

        __int64 dx = unit1->position.coords.x - unit2->position.coords.x;
        __int64 dz = unit1->position.coords.z - unit2->position.coords.z;
        return (int)(dx * dx >> 32) + (int)(dz * dz >> 32) <= weapon_def->range * weapon_def->range;
    }
    if (unit1->position.height.altitude + unit1->unit_def->height <= g_game->sea_level)
        return 0;
    if (unit2->position.height.altitude + unit2->unit_def->height <= g_game->sea_level)
        return 0;
    if (weapon_def->flags.bit17 && (unit2->state & 3) != 2)
        return 0;
    if (weapon_def->flags.bit1 && FUN_0049a890(unit1->position.coords.x - unit2->position.coords.x, unit1->position.coords.y - unit2->position.coords.y, unit1->position.coords.z - unit2->position.coords.z, weapon_def->unknown_68, weapon_def->unknown_c8) == (short)0x8000)
        return 0;

    __int64 dx = unit1->position.coords.x - unit2->position.coords.x;
    __int64 dz = unit1->position.coords.z - unit2->position.coords.z;
    return (int)(dx * dx >> 32) + (int)(dz * dz >> 32) <= weapon_def->range * weapon_def->range;
}
