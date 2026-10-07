// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>

struct Point16_004239c0 {
    short x;
    short z;
};

struct Vec3_004239c0 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Cell_004239c0 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Feature_004239c0 {
    char unknown_0[0x94];
    Point16_004239c0 footprint;        // +0x94
    char unknown_98[0xe4 - 0x98];
    void* burnt;                       // +0xe4
    char unknown_e8[0xfb - 0xe8];
    unsigned char spreadChance;        // +0xfb
    unsigned char unknown_fc;          // +0xfc
    unsigned char unknown_fd;          // +0xfd
    // An unsigned short bitfield, not unsigned char: changes the flag test code.
    unsigned short flag0 : 1;          // +0xfe
    unsigned short bits1 : 3;
    unsigned short flammable : 1;
    unsigned short bits5 : 11;
};

struct Game {
    char unknown_0[0x1426f];
    Feature_004239c0* features;        // +0x1426f
    char unknown_14273[0x37ecc - 0x14273];
    int windX;                         // +0x37ecc
    int windY;                         // +0x37ed0
    int windZ;                         // +0x37ed4
};
#pragma pack(pop)

extern Game* g_game;

Cell_004239c0* __stdcall GetMapCell(int x, int y);
int __stdcall RandomInt(int range);
void __stdcall StartFeatureBurning(int x, int z, int flag);
int __stdcall GetGroundHeight(Vec3_004239c0* pos);
void __stdcall ApplyAreaDamageAt(void* owner, Vec3_004239c0* pos);

// Inlined copy of GetFootprintCentre: the 16.16 world position of the centre of a
// feature footprint whose corner is at map cell `cell`.
static inline Vec3_004239c0 FootprintCentre_00421eb0(Point16_004239c0* cell, Feature_004239c0* def)
{
    Point16_004239c0 f = def->footprint;
    Point16_004239c0 c = *cell;
    Vec3_004239c0 p;
    p.x = (f.x + c.x * 2) << 19;
    p.z = (f.z + c.z * 2) << 19;
    p.y = GetGroundHeight(&p) << 16;
    return p;
}

struct Fixed_004239c0 {
    unsigned short frac;
    short whole;
};

union Coord_004239c0 {
    int value;
    Fixed_004239c0 f;
};

// A burning feature at `cell` sets fire (StartFeatureBurning) to flammable features
// within 3 cells, then to up to five cells downwind (the wind vector at
// +0x37ecc/+0x37ed4 times 2.0 in 16.16), and leaves its burnt remains
// (ApplyAreaDamageAt) at its centre.
// FUNCTION: 0x4239c0
void __stdcall SpreadFire(Feature_004239c0* f, Point16_004239c0* cell)
{
    for (int z = cell->z - 3; z <= cell->z + 3; z++) {
        for (int x = cell->x - 3; x <= cell->x + 3; x++) {
            if (x != cell->x || z != cell->z) {
                Cell_004239c0* c = GetMapCell(x, z);
                if (c && c->feature < 0xfffb && !(c->flags & 1)) {
                    Feature_004239c0* g = &g_game->features[c->feature];
                    // Its own `if`, not part of the `&&` chain.
                    if (g->flammable) {
                        if (RandomInt(100) < g->spreadChance)
                            StartFeatureBurning(x, z, 0);
                    }
                }
            }
        }
    }
    Coord_004239c0 px;
    Coord_004239c0 pz;
    px.value = cell->x << 16;
    // Computed from its own read of cell->z, before lastZ copies it.
    pz.value = cell->z << 16;
    int lastX = cell->x;
    int lastZ = cell->z;
    for (int i = 5; i; i--) {
        px.value += (int)(((__int64)g_game->windX * 0x20000) >> 16);
        pz.value += (int)(((__int64)g_game->windZ * 0x20000) >> 16);
        int x = px.f.whole;
        int z = pz.f.whole;
        if (x != lastX || z != lastZ) {
            Cell_004239c0* c = GetMapCell(x, z);
            if (c && c->feature < 0xfffb && !(c->flags & 1)) {
                Feature_004239c0* g = &g_game->features[c->feature];
                if (g->flammable) {
                    if (RandomInt(100) < g->spreadChance)
                        StartFeatureBurning(x, z, 0);
                }
            }
            lastX = x;
            lastZ = z;
        }
    }
    if (f->burnt) {
        Vec3_004239c0 pos = FootprintCentre_00421eb0(cell, f);
        ApplyAreaDamageAt(f->burnt, &pos);
    }
}
