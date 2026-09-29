// Decompiled by GPT-5.6-Terra, finished by Space Bunny Free. Names are provisional.
// Partial, 93.7% (576 of 568 bytes; up from 90.9%). Logic, offsets and every branch match.
// Two things moved it: `(height >> 1) + whole` (not `whole + (height >> 1)`) gives the
// original's `add edx, ecx` operand order in the half-height test, and the two includes
// below change MSVC's register choice in the sea-level tests (headers.py found them;
// without them the def pointer and the y word swap registers).
// What still differs, all in the line-of-fire block: the original copies unit2->pos
// (x to [esp+0x10], z to [esp+0x18], y kept in ebp) with `lea edx,[ebx+0x6a]`, keeping
// unit2 in ebx for the second distance tail; here MSVC does `add ebx,0x6a` and reloads
// ebx from the stack afterwards, so the tail differs by one reload (8 bytes).
// Tried: by-value and by-pointer Vec3 params in every order, plain-int Vec3, local
// copies (the copy is then optimised away, frame shrinks to 8), dx/dy/dz statement
// orders, def pointer locals; pointer params make MSVC merge the two distance tails.
#include <stdlib.h>
#include <math.h>
#pragma pack(push, 1)

union Fixed_0049abb0 {
    int value;                                      // 16.16 fixed point
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

struct Vec3_0049abb0 {
    int x;
    Fixed_0049abb0 y;
    int z;
};

struct WeaponDef_0049abb0 {
    char unknown_0[0x68];
    int field_68;                                   // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                                 // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                                      // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;                      // line of fire
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;                     // skip the ground test
        unsigned int bit17 : 1;                     // target must be landed
        unsigned int bit18_31 : 14;
    } flags;                                        // +0x111
};

struct Weapon_0049abb0 {
    WeaponDef_0049abb0* def;                        // +0x0
    char unknown_4[0x1c - 0x4];
};

struct UnitDef_0049abb0 {
    char unknown_0[0x170];
    short height;                                   // +0x170
    char unknown_172[0x241 - 0x172];
    struct {
        unsigned int bit0_11 : 12;
        unsigned int bit12 : 1;                     // half height counts
        unsigned int bit13_18 : 6;
        unsigned int bit19 : 1;                     // ignore sea level
        unsigned int bit20_31 : 12;
    } flags;                                        // +0x241
};

struct Unit_0049abb0 {
    char unknown_0[0x10];
    Weapon_0049abb0 weapons[1];                     // +0x10, stride 0x1c
    char unknown_2c[0x6a - 0x2c];
    Vec3_0049abb0 pos;                              // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_0049abb0* def;                          // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int state;                             // +0x110
};

struct Game_0049abb0 {
    char unknown_0[0x1427f];
    unsigned char sea_level;                        // +0x1427f
};

#pragma pack(pop)

extern Game_0049abb0* g_game;

short __stdcall FUN_0049a890(int dx, int dy, int dz, int a, int b);

// The target is passed by value: that is what puts the 12-byte copy of its
// position in the frame, while the shooter is read where it is.
static inline short LineOfFire_0049abb0(Vec3_0049abb0 from, Vec3_0049abb0 to, int s, int f)
{
    return FUN_0049a890(to.x - from.x, to.y.value - from.y.value, to.z - from.z, s, f);
}

static inline int Dist2_0049abb0(Vec3_0049abb0* b, Vec3_0049abb0* a)
{
    int dz = a->z - b->z;
    int dx = a->x - b->x;
    return (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
}

// FUNCTION: 0x49abb0
int __stdcall FUN_0049abb0(Unit_0049abb0* unit1, Unit_0049abb0* unit2, unsigned char weapon)
{
    WeaponDef_0049abb0* w = unit1->weapons[weapon].def;

    if (w->flags.bit16) {
        if (!unit2->def->flags.bit19 && unit2->pos.y.parts.whole > g_game->sea_level)
            return 0;
        if (unit2->def->flags.bit12 && (unit2->def->height >> 1) + unit2->pos.y.parts.whole > g_game->sea_level)
            return 0;
        return Dist2_0049abb0(&unit1->pos, &unit2->pos) <= w->range * w->range;
    }

    if (unit1->pos.y.parts.whole + unit1->def->height <= g_game->sea_level)
        return 0;
    if (unit2->pos.y.parts.whole + unit2->def->height <= g_game->sea_level)
        return 0;
    if (w->flags.bit17 && (unit2->state & 3) != 2)
        return 0;
    if (w->flags.bit1) {
        if (LineOfFire_0049abb0(unit2->pos, unit1->pos, w->field_68, w->field_c8) == (short)0x8000)
            return 0;
    }
    return Dist2_0049abb0(&unit1->pos, &unit2->pos) <= w->range * w->range;
}
