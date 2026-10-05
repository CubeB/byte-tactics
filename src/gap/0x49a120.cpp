// Decompiled by Claude Opus 5.5. Names are provisional.
//
// 99.3% (gapcheck), same length as the original (1829 bytes). No /Op: the
// aligned frame comes from the double locals of the inlined Length.
//
// Fixed in the second attempt:
//  - The falloff square: the original consumes t at t * t (`fld st(1);
//    fmulp st(2); fmulp st(1)`) and keeps edge in memory. MSVC 5 pops an x87
//    value at its last use only when that use redefines it, so t is squared
//    in place and the whole falloff assigned back to t before `scale = t`.
//    Square(), `t * t` in the scale expression, or `scale = (1 - edge) * t +
//    edge` after `t *= t` keep t on the stack until the block ends; finishing
//    with `t = t * (1 - edge); scale = t + edge;` puts edge on the x87 stack
//    and drops its frame slot (about 75%).
//  - The weapon loop reads every field through `other` (no `weapons[j]`), so
//    the derived pointer (other + 0xc) is stepped after `other`, as in the
//    original.
//
// Fixed in the third attempt (97.7% to 99.3%):
//  - The upper-bound branch of each axis (`mov edx, [max]; add edi, edx`,
//    the sum in unit->pos's register). A named `int b = upos + max;` used in
//    both the test and the subtraction is lowered as two tuples (b = max;
//    b += upos), so b interferes with the upos temporary and can never get
//    its edi (c2prio --trace: with upos coloured first, b's allowed set loses
//    edi). A common subexpression is one three-operand tuple instead: C2
//    gives it upos's edi, and the code generator loads max into a scratch
//    register because the destination is also the second operand. So the sum
//    is written in the test and again in a block local used once (`hi`),
//    which C2 forwards into the subtraction: the two sums become one
//    temporary. Writing `p - (upos + max)` directly does not work, since the
//    front end reassociates it into two subtractions.
//
// What still differs:
//  - The spot address: the original computes idx * 48 in eax with the spots
//    base in edi (add order idx * 48 + spots); here spots + idx * 48. It
//    follows symbol ids, not spelling (casts, an index local, a spots local,
//    Cell defined after Game, inline helpers for the spot or the feature
//    definition, a Game member, `spots + idx`, a single-use spot local with
//    the address written again in the then-block: 58% to 71%). Diagnostic
//    dummy declarations (never committed): with N of them just before this
//    function, N in 15582 to 15635 or 64480 to 64520 gives a MATCH. N past
//    about 15330 gives cell's id bit 14 and the spot add flips, but the
//    features add (`features + feature * 256`) then flips too (91.6%, the
//    same as `<windows.h>` anywhere before the function, with or without
//    <ddraw.h>, <dsound.h>, <dplay.h>, <stdio.h>, <stdlib.h>, <list> or
//    <map>; <vector> or <string> on top bring back 99.3%). Shifting only
//    `feature` and the later locals (block-scope dummies) flips the features
//    add alone, so the two adds follow cell's and feature's ids; the
//    original's pair (spot idx-first, features base-first) needs a header
//    prefix no plausible real set reaches.
//  - g_game's id takes part as well (diagnostic dummies again): with 15400
//    before this function and 250 before `feature` (cell 16405, feature
//    16709) the region matches with g_game at 912 or 1012 but not at 1912 or
//    15912, and with cell at 16405 only feature ids from about 16640 to 16900
//    match. `<windows.h>` placed after g_game (before this function, alone or
//    with the DirectX headers) gives the spot add, but the features add stays
//    wrong for every shift of `feature` from 0 to 1100 symbols (91.6%), and
//    `<windows.h>` with WIN32_LEAN_AND_MEAN there gives 99.3%. So the
//    original seems to have had some 15400 symbols between g_game's
//    declaration and this function: the rest of its translation unit, which
//    is lost.
#include <math.h>
#include <string.h>

struct Vec3_0049a120 {
    int x;                             // +0x0 (16.16)
    int y;                             // +0x4
    int z;                             // +0x8
};

// A 16.16 fixed-point value and its whole part.
union Fixed_0049a120 {
    int raw;
    struct {
        unsigned short frac;
        short whole;
    } part;
};

struct FixedVec3_0049a120 {
    Fixed_0049a120 x;
    Fixed_0049a120 y;
    Fixed_0049a120 z;
};

struct CellPos_0049a120 {
    short x;
    short z;
};

#pragma pack(push, 1)
struct WeaponDef_0049a120 {
    char unknown_0[0xd6];
    unsigned short radius;             // +0xd6
    float edgeDamage;                  // +0xd8
    char unknown_dc[0x10a - 0xdc];
    unsigned char kind;                // +0x10a
    char unknown_10b[0x111 - 0x10b];
    union {
        unsigned int all;
        struct {
            unsigned int bits0_29 : 30;
            unsigned int detonatesWeapons : 1;
            unsigned int bit31 : 1;
        } bits;
    } flags;                           // +0x111
};

struct UnitDef_0049a120 {
    char unknown_0[0x15e];
    Vec3_0049a120 boxMin;              // +0x15e
    Vec3_0049a120 boxMax;              // +0x16a
};

struct Holder_0049a120;

struct Unit_0049a120 {
    char unknown_0[0x6a];
    Vec3_0049a120 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_0049a120* def;             // +0x92
    Holder_0049a120* holder;           // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char owner;               // +0xff
    char unknown_100[0x118 - 0x100];
};

struct Weapon_0049a120 {
    WeaponDef_0049a120* def;           // +0x0
    Vec3_0049a120 pos;                 // +0x4
    char unknown_10[0x28 - 0x10];
    Vec3_0049a120 field_28;            // +0x28
    char unknown_34[0x52 - 0x34];
    Unit_0049a120* attacker;           // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;              // +0x69
};

class Class_00406f50 {
public:
    void FUN_00406f50(Weapon_0049a120* weapon, int enemyDamage, int friendlyDamage);
};

struct Holder_0049a120 {
    char unknown_0[4];
    int playerId;                      // +0x4
    char unknown_8[0x74 - 8];
    Class_00406f50* object;            // +0x74
};

struct Cell_0049a120 {
    unsigned short unit;               // +0x0
    unsigned short unit2;              // +0x2
    char unknown_4[4];
    unsigned short feature;            // +0x8
    union {
        unsigned short spot;           // +0xa
        struct {
            unsigned char offsetZ;     // +0xa
            unsigned char offsetX;     // +0xb
        } origin;
    };
    unsigned char flags;               // +0xc
};

struct Spot_0049a120 {
    char unknown_0[8];
    Vec3_0049a120 pos;                 // +0x8
    char unknown_14[0x30 - 0x14];
};

struct FeatureDef_0049a120 {
    char unknown_0[0x100];
};

struct Packet_0049a120 {
    unsigned char type;
    Vec3_0049a120 pos;
    unsigned char kind;
};

struct Game_0049a120 {
    char unknown_0[0x141f3];
    int numWeapons;                    // +0x141f3
    Weapon_0049a120* weapons;          // +0x141f7
    char unknown_141fb[0x1420b - 0x141fb];
    Spot_0049a120* spots;              // +0x1420b
    char unknown_1420f[0x14233 - 0x1420f];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    FeatureDef_0049a120* features;     // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_0049a120* cells;              // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Unit_0049a120* units;              // +0x14357
};
#pragma pack(pop)

extern Game_0049a120* g_game;

// What one explosion has already hit, so nothing takes damage twice.
struct Hits_0049a120 {
    int numUnits;
    int numFeatures;
    Unit_0049a120* units[20];
    Cell_0049a120* features[64];

    // Each returns 0 if the thing was hit already, else records it (while
    // there is room) and returns 1.
    int AddUnit(Unit_0049a120* unit)
    {
        for (int k = 0; k < numUnits; k++) {
            if (units[k] == unit)
                return 0;
        }
        if (numUnits < 20) {
            units[numUnits] = unit;
            numUnits++;
        }
        return 1;
    }
    int AddFeature(Cell_0049a120* cell)
    {
        for (int k = 0; k < numFeatures; k++) {
            if (features[k] == cell)
                return 0;
        }
        if (numFeatures < 64) {
            features[numFeatures] = cell;
            numFeatures++;
        }
        return 1;
    }
};

Cell_0049a120* __stdcall FUN_00481550(int x, int y);
int __stdcall FUN_00499cd0(Weapon_0049a120* weapon, Unit_0049a120* target, float scale);
void __stdcall FUN_00499eb0(Weapon_0049a120* weapon, Unit_0049a120* unit);
int __stdcall FUN_0049a850(Vec3_0049a120* v);
Vec3_0049a120 __stdcall FUN_00421eb0(CellPos_0049a120* cell, FeatureDef_0049a120* def);
void __stdcall FUN_004244b0(Cell_0049a120* cell, int x, int z, WeaponDef_0049a120* def);
int __stdcall FUN_00451df0(int id, void* data, int size);

static inline int Length(Vec3_0049a120* v)
{
    double x = v->x;
    double y = v->y;
    double z = v->z;
    return (int)sqrt(x * x + y * y + z * z);
}

static inline Unit_0049a120* UnitFromId(unsigned short id)
{
    if (id == 0)
        return 0;
    return &g_game->units[id];
}

static inline Vec3_0049a120 Sub(const Vec3_0049a120& a, const Vec3_0049a120& b)
{
    Vec3_0049a120 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

// Splash damage: every unit and feature within the weapon's radius of `pos`
// takes damage that falls off from the centre, and weapons in flight close
// enough are detonated too.
// FUNCTION: 0x49a120
void __stdcall FUN_0049a120(Weapon_0049a120* weapon, Vec3_0049a120* pos)
{
    Hits_0049a120 hits;
    memset(&hits, 0, 8);
    int radius = weapon->def->radius >> 1;
    int r = radius / 16 + 1;
    int cx = ((FixedVec3_0049a120*)pos)->x.part.whole;
    int x0 = cx / 16 - r;
    int x1 = cx / 16 + r;
    int cz = ((FixedVec3_0049a120*)pos)->z.part.whole;
    int z0 = cz / 16 - r;
    int z1 = cz / 16 + r;
    if (x0 < 0)
        x0 = 0;
    if (x1 > g_game->width)
        x1 = g_game->width;
    if (z0 < 0)
        z0 = 0;
    if (z1 > g_game->height)
        z1 = g_game->height;
    int enemyDamage = 0;
    int friendlyDamage = 0;

    for (int z = z0; z < z1; z++) {
        Cell_0049a120* cell = FUN_00481550(x0, z);
        for (int x = x0; x < x1; x++, cell++) {
            if (!cell)
                continue;
            for (int i = 0; i <= 1; i++) {
                Unit_0049a120* unit = i ? UnitFromId(cell->unit2) : UnitFromId(cell->unit);
                if (!unit || unit == weapon->attacker)
                    continue;
                if (!hits.AddUnit(unit))
                    continue;
                UnitDef_0049a120* def = unit->def;
                Vec3_0049a120 d;
                {
                    int p = pos->x;
                    if (p < unit->pos.x + def->boxMin.x) {
                        int lo = unit->pos.x + def->boxMin.x;
                        d.x = lo - p;
                    } else if (p > unit->pos.x + def->boxMax.x) {
                        int hi = unit->pos.x + def->boxMax.x;
                        d.x = p - hi;
                    } else {
                        d.x = 0;
                    }
                }
                {
                    int p = pos->y;
                    if (p < unit->pos.y + def->boxMin.y) {
                        int lo = unit->pos.y + def->boxMin.y;
                        d.y = lo - p;
                    } else if (p > unit->pos.y + def->boxMax.y) {
                        int hi = unit->pos.y + def->boxMax.y;
                        d.y = p - hi;
                    } else {
                        d.y = 0;
                    }
                }
                {
                    int p = pos->z;
                    if (p < unit->pos.z + def->boxMin.z) {
                        int lo = unit->pos.z + def->boxMin.z;
                        d.z = lo - p;
                    } else if (p > unit->pos.z + def->boxMax.z) {
                        int hi = unit->pos.z + def->boxMax.z;
                        d.z = p - hi;
                    } else {
                        d.z = 0;
                    }
                }
                Fixed_0049a120 dist;
                dist.raw = Length(&d);
                int distance = dist.part.whole;
                if (distance < radius) {
                    float edge = weapon->def->edgeDamage;
                    float scale;
                    if (distance) {
                        float t = (float)distance / radius - 1.0f;
                        t *= t;
                        t = (1.0f - edge) * t + edge;
                        scale = t;
                    } else {
                        scale = 1.0f;
                    }
                    int damage = FUN_00499cd0(weapon, unit, scale);
                    if (weapon->owner == unit->owner)
                        friendlyDamage += damage;
                    else
                        enemyDamage += damage;
                }
            }
            if (weapon->def->flags.all & 0x4000)
                continue;
            Cell_0049a120* origin = cell;
            int fx = x;
            int fz = z;
            if (cell->feature == 0xfffe) {
                fx -= cell->origin.offsetX;
                fz -= cell->origin.offsetZ;
                origin = FUN_00481550(fx, fz);
            }
            unsigned short feature = origin->feature;
            if (feature >= 0xfffb)
                continue;
            int index = cell - g_game->cells;
            CellPos_0049a120 cp;
            cp.x = index % g_game->width;
            cp.z = index / g_game->width;
            CellPos_0049a120 at = cp;
            Spot_0049a120* spot = &g_game->spots[cell->spot];
            int distance;
            if ((cell->flags & 1) && spot) {
                Vec3_0049a120 v = Sub(*pos, spot->pos);
                Fixed_0049a120 dist;
                dist.raw = FUN_0049a850(&v);
                distance = dist.part.whole;
            } else {
                Vec3_0049a120 p = FUN_00421eb0(&at, &g_game->features[feature]);
                Vec3_0049a120 v = Sub(*pos, p);
                Fixed_0049a120 dist;
                dist.raw = (int)sqrt((double)v.x * v.x + (double)v.y * v.y + (double)v.z * v.z);
                distance = dist.part.whole;
            }
            if (distance >= radius)
                continue;
            if (!hits.AddFeature(origin))
                continue;
            FUN_004244b0(origin, fx, fz, weapon->def);
        }
    }

    if (weapon->def->flags.bits.detonatesWeapons) {
        Weapon_0049a120* other = g_game->weapons;
        for (int j = 0; j < g_game->numWeapons; j++, other++) {
            if ((other->flags & 2) || other == weapon)
                continue;
            Vec3_0049a120* p = &other->pos;
            int dx = weapon->pos.x - p->x;
            int dy = weapon->pos.y - p->y;
            int dz = weapon->pos.z - p->z;
            int reach = weapon->def->radius;
            if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dy * dy) >> 32) + (int)(((__int64)dz * dz) >> 32)
                < reach * reach) {
                FUN_00499eb0(other, 0);
                Packet_0049a120 packet;
                packet.type = 0xe;
                packet.pos = other->field_28;
                packet.kind = other->def->kind;
                FUN_00451df0(weapon->attacker->holder->playerId, &packet, sizeof(packet));
                packet.type = 0xe;
                packet.pos = weapon->field_28;
                packet.kind = weapon->def->kind;
                FUN_00451df0(weapon->attacker->holder->playerId, &packet, sizeof(packet));
            }
        }
    }

    if (weapon->attacker)
        weapon->attacker->holder->object->FUN_00406f50(weapon, enemyDamage, friendlyDamage);
}
