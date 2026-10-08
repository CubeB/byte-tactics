// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// 0x49b090 is the projectile collision test. It looks up the map cell holding
// the projectile with GetMapCellAtPosition(&proj->pos); if there is none it stores the
// selected projectile's last position and sound, clears the selection, sets the
// dead flag and returns. Otherwise: (1) if the projectile is attached to a unit,
// the 64-bit squared distance is checked against type->radius^2 and the
// projectile is killed on contact; (2) proj->radius is set to
// (cell->radius + cell->ground) / 2; (3) the two unit indices in the cell are
// tested against proj->owner and the height window
// [type->low + elev, type->high + elev]; (4) the map-feature id is resolved,
// including the 0xfffe "read the neighbour cell" case, and if the feature is new
// for this cell the cell coordinates are stored; (5) the 0x8000 and 0x10000 flag
// rules, the g_game->limit ceiling and the netgame check are applied before the
// final kill.
//
// Original bug: 0x49b2c8 to 0x49b2d6 range-checks the map-feature id against
// g_game+0x14253, but the 0xfffe reload path at 0x49b2e7 to 0x49b30f re-tests
// only against 0xfffb and skips the count check, so a feature id read from the
// neighbouring cell indexes g_game->mapping unchecked.

// Only <stdio.h>: no other header set gives the feature block the original's
// register plan. Nothing here is used from it.
#include <stdio.h>

#pragma pack(push, 1)

struct Pos_0049b090 {
    int x;
    int y;
    int z;
};

// 13 bytes, the stride the cell arithmetic at +0x6e walks with `n * 13`.
struct Cell_0049b090 {
    unsigned short unit0;             // +0x0
    unsigned short unit1;             // +0x2
    unsigned char height;             // +0x4
    unsigned char radius;             // +0x5
    unsigned char ground;             // +0x6
    unsigned char unknown_7;          // +0x7
    unsigned short feature;           // +0x8
    unsigned char offY;               // +0xa
    unsigned char offX;               // +0xb
    unsigned char unknown_c;          // +0xc
};

// The mapping is an array of these, indexed as `mapping[f * 256]`.
struct MapFeature_0049b090 {
    char unknown_0[0xfa];
    unsigned char height;             // +0xfa
    char unknown_fb[0x100 - 0xfb];
};

// A 1-bit bitfield view is needed: only it produces the shr/test on bit 15.
union TypeFlags_0049b090 {
    unsigned int raw;
    struct {
        unsigned int low : 15;
        unsigned int b15 : 1;
        unsigned int high : 16;
    } b;
};

struct ProjType_0049b090 {
    char unknown_0[0xd6];
    unsigned short radius;             // +0xd6
    char unknown_d8[0xfe - 0xd8];
    unsigned short sound;              // +0xfe
    char unknown_100[0x111 - 0x100];
    TypeFlags_0049b090 flags;          // +0x111
};

struct UnitType_0049b090 {
    char unknown_0[0x162];
    int low;                           // +0x162
    char unknown_166[8];
    int high;                          // +0x16e
};

struct Unit {
    char unknown_0[4];
    Pos_0049b090 pos;                  // +0x4
    char unknown_10[0x6e - 0x10];
    int elev;                          // +0x6e
    char unknown_72[0x92 - 0x72];
    UnitType_0049b090* type;           // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char owner;               // +0xff
    char unknown_100[0x118 - 0x100];
};

union Flags_0049b090 {
    unsigned char value;
    struct {
        unsigned char b0 : 1;
        unsigned char dead : 1;
        unsigned char rest : 6;
    } bits;
};

// The world position is three ints at +0x4 and, in the same bytes, three shorts
// at +0x6, +0xa and +0xe: the map-cell coordinates. A union is the only way to
// get the two views, and it is the shorts the code reaches for the cell.
union WordPair_0049b090 {
    int i;
    struct {
        char lo[2];
        short hi;
    } s;
};

struct Proj_0049b090 {
    ProjType_0049b090* type;           // +0x0
    WordPair_0049b090 px;              // +0x4 (short at +0x6)
    WordPair_0049b090 py;              // +0x8 (short at +0xa)
    WordPair_0049b090 pz;              // +0xc (short at +0xe)
    char unknown_10[0x20 - 0x10];
    int field_20;                      // +0x20
    char unknown_24[0x56 - 0x24];
    Unit* unit;                        // +0x56
    short cellX;                       // +0x5a
    short cellZ;                       // +0x5c
    short radius;                      // +0x5e
    char unknown_60[0x66 - 0x60];
    unsigned char owner;               // +0x66
    char unknown_67[2];
    Flags_0049b090 flags;              // +0x69
};

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    MapFeature_0049b090* mapping;      // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char limit;               // +0x1427f
    char unknown_14280[0x142f7 - 0x14280];
    Proj_0049b090* selected;           // +0x142f7
    char unknown_142fb[0x1433f - 0x142fb];
    Pos_0049b090 lastPos;              // +0x1433f
    unsigned short lastSound;          // +0x1434b
    char unknown_1434d[0x14357 - 0x1434d];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x391e9 - 0x1435b];
    void* net;                         // +0x391e9
};

struct Net_0049b090 {
    char unknown_0[0xd48];
    int field_d48;
};
#pragma pack(pop)

extern Game* g_game;

Cell_0049b090* __stdcall GetMapCellAtPosition(Pos_0049b090* pos);
void __stdcall DetonateProjectile(Proj_0049b090* proj, Unit* unit);

// Stays in its own file: its feature block matches only at this file's symbol
// count, which the joined weapons.cpp moves.
// FUNCTION: 0x49b090
void __stdcall CheckProjectileCollision(ProjType_0049b090* type, Proj_0049b090* proj)
{
    // No `Game* g = g_game` local: g_game is read at each use to stay in edi.
    // Named pos local, used again after the lookup: gives the original's prologue.
    Pos_0049b090* pos = (Pos_0049b090*)&proj->px;
    Cell_0049b090* cell = GetMapCellAtPosition(pos);

    if (!cell) {
        if (proj == g_game->selected) {
            g_game->lastPos = *(Pos_0049b090*)&g_game->selected->px;
            g_game->lastSound = proj->type->sound;
            g_game->selected = 0;
        }
        proj->flags.bits.dead = 1;
        return;
    }
    if (proj->unit) {
        int dx = pos->x - proj->unit->pos.x;
        int dy = pos->y - proj->unit->pos.y;
        int dz = pos->z - proj->unit->pos.z;
        int r = proj->type->radius;
        int d = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dy * dy) >> 32)
            + (int)(((__int64)dz * dz) >> 32);
        if (d < r * r)
            DetonateProjectile(proj, 0);
    }
    proj->radius = (cell->radius + cell->ground) / 2;
    if (cell->unit0) {
        Unit* u = &g_game->units[cell->unit0];
        if (u->owner != proj->owner && proj->py.i < u->type->high + u->elev) {
            DetonateProjectile(proj, u);
            return;
        }
    }
    if (cell->unit1) {
        Unit* u = &g_game->units[cell->unit1];
        if (u->owner != proj->owner) {
            if (proj->py.i >= u->type->low + u->elev
                && proj->py.i <= u->type->high + u->elev) {
                DetonateProjectile(proj, u);
                return;
            }
        }
    }
    if (type->flags.raw & 0x4000)
        return;
    {
        short cx = proj->px.s.hi / 16;
        short cz = proj->pz.s.hi / 16;
        unsigned short f = cell->feature;
        MapFeature_0049b090* mf;
        // The `!(f == 0xfffe)` chain shape sets the block order.
        if (f < 0xfffb) {
            if (f < g_game->featureCount)
                mf = g_game->mapping + f;
            else
                mf = 0;
        } else if (!(f == 0xfffe)) {
            mf = 0;
        } else {
            // This index spelling and `unsigned short f2` merge the mapping tails.
            int n = cell->offX + g_game->width * cell->offY;
            unsigned short f2 = (cell - n)->feature;
            if (f2 >= 0xfffb) {
                mf = 0;
            } else {
                mf = g_game->mapping + f2;
            }
        }
        if (mf) {
            // Positive direction (sum > hi, else mf = 0): sets the compare operand order.
            if (mf->height + cell->ground > proj->py.s.hi) {
                if (proj->cellX == cx && proj->cellZ == cz) {
                    mf = 0;
                } else {
                    proj->cellX = cx;
                    proj->cellZ = cz;
                }
            } else {
                mf = 0;
            }
        }
        if (mf) {
            DetonateProjectile(proj, 0);
            return;
        }
    }
    if (cell->ground > proj->py.s.hi) {
        if (type->flags.b.b15) {
            proj->field_20 = -(proj->field_20 >> 2);
            return;
        }
    } else if (type->flags.raw & 0x10000) {
        return;
    } else if (proj->py.s.hi >= g_game->limit) {
        return;
    } else if (((Net_0049b090*)g_game->net)->field_d48) {
        return;
    }
    DetonateProjectile(proj, 0);
}
