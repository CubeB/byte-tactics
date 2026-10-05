// Decompiled by Claude Opus 5.5. Names are provisional.
//
// 97.0% (gapcheck), same length as the original (1829 bytes). No /Op: the
// aligned frame comes from the double locals of the inlined Length. What
// still differs:
//  - The upper-bound branch of each axis in the unit distance: the original
//    loads boxMax into a register and adds it into the register holding
//    unit->pos (`mov edx, [max]; add edi, edx; cmp ecx, edi; sub ecx, edi`);
//    here the sum goes into boxMax's register (`add edx, edi`). Tried: an
//    inline helper with every parameter kind and order, a named or modified
//    origin (`o += max`, which turns into `add reg, [mem]`), b = origin; b +=
//    max, an Add() helper, unnamed sums (MSVC then reassociates p - (o + max)
//    into two subtractions), comparison spellings. Named per-axis locals
//    other than `p` reshuffle the whole register allocation (about 62%).
//  - The falloff square: the original consumes t (`fld st(1); fmulp st(2);
//    fmulp st(1)`). Square() gives `fmul st(1)` twice and a trailing pop;
//    a named `float t` (or a CSE'd expression) gives the original's tree
//    shape but keeps t until the block ends (`fmul st(0), st(2)`, `fxch`, and
//    a pop: 2 bytes longer, 91.8%).
//  - The spot address: the original computes idx * 48 in eax with the spots
//    base in edi and loads cell->flags in between; here idx * 48 lands in
//    edx (an expression temporary rotation difference).
//  - The weapon loop increments `other` before the derived pointer, here
//    after.
// build/scratch/gap/combo.py (alternative snippets, every combination
// compiled and scored, optionally on an address range) found most of this.
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

static inline float Square(float v)
{
    return v * v;
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
                    if (p < def->boxMin.x + unit->pos.x) {
                        d.x = def->boxMin.x + unit->pos.x - p;
                    } else {
                        int b = unit->pos.x + def->boxMax.x;
                        if (p > b)
                            d.x = p - b;
                        else
                            d.x = 0;
                    }
                }
                {
                    int p = pos->y;
                    if (p < def->boxMin.y + unit->pos.y) {
                        d.y = def->boxMin.y + unit->pos.y - p;
                    } else {
                        int b = unit->pos.y + def->boxMax.y;
                        if (p > b)
                            d.y = p - b;
                        else
                            d.y = 0;
                    }
                }
                {
                    int p = pos->z;
                    if (p < def->boxMin.z + unit->pos.z) {
                        d.z = def->boxMin.z + unit->pos.z - p;
                    } else {
                        int b = unit->pos.z + def->boxMax.z;
                        if (p > b)
                            d.z = p - b;
                        else
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
                        scale = (1.0f - edge) * Square((float)distance / radius - 1.0f) + edge;
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
        Weapon_0049a120* weapons = g_game->weapons;
        Weapon_0049a120* other = weapons;
        for (int j = 0; j < g_game->numWeapons; j++, other++) {
            if ((weapons[j].flags & 2) || other == weapon)
                continue;
            Vec3_0049a120* p = &weapons[j].pos;
            int dx = weapon->pos.x - p->x;
            int dy = weapon->pos.y - p->y;
            int dz = weapon->pos.z - p->z;
            int reach = weapon->def->radius;
            if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dy * dy) >> 32) + (int)(((__int64)dz * dz) >> 32)
                < reach * reach) {
                FUN_00499eb0(other, 0);
                Packet_0049a120 packet;
                packet.type = 0xe;
                packet.pos = weapons[j].field_28;
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
