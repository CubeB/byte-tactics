// Decompiled by GPT-5.6-Terra, finished by Space Bunny Free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro. Names are provisional.
// Both includes change the register choice in the sea-level tests.
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

// The vector from point `a` to point `b`. Both operands and the result are by
// value, which is what makes the line-of-fire block below compile the way the
// original does: MSVC builds the two 12-byte argument copies as one setup unit
// instead of turning the first aggregate into a live pointer, so `unit2` stays in
// ebx across the call and the second distance tail needs no reload. Note the
// operands are named in the order the callers pass them (shooter, target) while
// the result is target - shooter, because the copy order, not the arithmetic, is
// what the register allocation is sensitive to here.
inline Vec3_0049abb0 operator-(Vec3_0049abb0 a, Vec3_0049abb0 b)
{
    Vec3_0049abb0 r;
    r.x = b.x - a.x;
    r.y.value = b.y.value - a.y.value;
    r.z = b.z - a.z;
    return r;
}

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

struct Unit {
    char unknown_0[0x10];
    Weapon_0049abb0 weapons[1];                     // +0x10, stride 0x1c
    char unknown_2c[0x6a - 0x2c];
    Vec3_0049abb0 pos;                              // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_0049abb0* def;                          // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int state;                             // +0x110
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char sea_level;                        // +0x1427f
};

#pragma pack(pop)

extern Game* g_game;

short __stdcall SolveLaunchAngle(int dx, int dy, int dz, int a, int b);

// Can the shooter hit the target? Both positions are taken by value, which is
// what puts the 12-byte copies in the frame while the units themselves are read
// where they are. The parameters are named for the order the caller passes them
// in, and the subtraction gives the target relative to the shooter.
static inline short LineOfFire_0049abb0(Vec3_0049abb0 to, Vec3_0049abb0 from, int s, int f)
{
    Vec3_0049abb0 d = from - to;
    return SolveLaunchAngle(d.x, d.y.value, d.z, s, f);
}

static inline int Dist2_0049abb0(Vec3_0049abb0* b, Vec3_0049abb0* a)
{
    int dz = a->z - b->z;
    int dx = a->x - b->x;
    return (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
}

// FUNCTION: 0x49abb0
int __stdcall WeaponCanReachUnit(Unit* unit1, Unit* unit2, unsigned char weapon)
{
    WeaponDef_0049abb0* w = unit1->weapons[weapon].def;

    if (w->flags.bit16) {
        if (!unit2->def->flags.bit19 && unit2->pos.y.parts.whole > g_game->sea_level)
            return 0;
        // Written (height >> 1) + whole: sets the add's operand order.
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
    // Nested ifs, not one && condition: the flag test codegen differs.
    if (w->flags.bit1) {
        if (LineOfFire_0049abb0(unit1->pos, unit2->pos, w->field_68, w->field_c8) == (short)0x8000)
            return 0;
    }
    return Dist2_0049abb0(&unit1->pos, &unit2->pos) <= w->range * w->range;
}
