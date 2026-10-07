// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>

struct Point16_00423c50 {
    short x;
    short z;
};

struct Vec3_00423c50 {
    int x, y, z;
};

struct Rot16_00423c50 {
    short x, y, z;
};

#pragma pack(push, 1)
struct Cell_00423c50 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    union {
        struct {
            unsigned char offsetZ;     // +0xa
            unsigned char offsetX;     // +0xb
        };
        unsigned short spot;           // +0xa
    };
    unsigned char flag0 : 1;           // +0xc
    unsigned char bits1 : 2;
    unsigned char owner : 4;
    unsigned char bit7 : 1;
};

struct Feature_00423c50 {
    char unknown_0[0x94];
    Point16_00423c50 footprint;        // +0x94
    void* object;                      // +0x98
    char unknown_9c[0xfe - 0x9c];
    unsigned short flag0 : 1;          // +0xfe
    unsigned short bits1 : 4;
    unsigned short bit5 : 1;
    unsigned short bits6 : 10;
};

struct Spot_00423c50 {
    char unknown_0[4];
    void* state;                       // +0x4
    Vec3_00423c50 pos;                 // +0x8
    char unknown_14[0x20 - 0x14];
    Rot16_00423c50 rot;                // +0x20
    unsigned short damage;             // +0x26
    Point16_00423c50 cell;             // +0x28
    unsigned short feature;            // +0x2c
    char unknown_2e;
    unsigned char flag0 : 1;           // +0x2f
    unsigned char bits1 : 7;
};

struct Pool_00423c50 {
    Spot_00423c50* entries;            // +0x0
    char unknown_4[4];
    int usedHead;                      // +0x8
    char unknown_c[4];
    int freeHead;                      // +0x10
};

struct Game {
    char unknown_0[0x1420b];
    Pool_00423c50 pool;                // +0x1420b
    char unknown_1421f[0x14233 - 0x1421f];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    Feature_00423c50* features;        // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_00423c50* cells;              // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall RemoveFeature(Cell_00423c50* cell, int flag);
void __stdcall MoveFeatureSpot(int index, int* head);
int __stdcall GetGroundHeight(Vec3_00423c50* pos);
void* __stdcall CreateObjectState(void* obj);
void __stdcall FUN_00472c50(Vec3_00423c50* p, short index);
void __stdcall RefreshAllPassMaps(Point16_00423c50 a, Point16_00423c50 b);

// Inlined copy of AllocFeatureSpot: takes a spot off the free list.
static inline int AllocSpot_004232a0()
{
    Pool_00423c50* p = &g_game->pool;
    int i = p->freeHead;
    if (i == -1) {
        return 0x800;
    }
    MoveFeatureSpot(i, &p->usedHead);
    p->entries[i].flag0 = 0;
    return i;
}

// Inlined copy of GetFootprintCentre: the 16.16 world position of the centre of a
// feature footprint whose corner is at map cell `cell`.
static inline Vec3_00423c50 FootprintCentre_00421eb0(Point16_00423c50* cell, Feature_00423c50* def)
{
    Point16_00423c50 f = def->footprint;
    Point16_00423c50 c = *cell;
    Vec3_00423c50 p;
    p.x = (f.x + c.x * 2) << 19;
    p.z = (f.z + c.z * 2) << 19;
    p.y = GetGroundHeight(&p) << 16;
    return p;
}

// Places feature `feature` on map cell `cell` (its footprint's corner): fails
// when the footprint leaves the map or a feature already there cannot be
// removed (RemoveFeature), takes a spot for features that keep state, marks the
// other footprint cells 0xfffe with their offsets from the corner, and returns
// the spot (0 when none).
// FUNCTION: 0x423c50
Spot_00423c50* __stdcall PlaceFeature(Cell_00423c50* cell, unsigned short feature,
                                      Vec3_00423c50* pos, Rot16_00423c50* rot,
                                      unsigned char owner)
{
    Spot_00423c50* spot = 0;
    if (feature == 0xffff)
        return 0;
    if (feature == 0xfffc) {
        cell->feature = feature;
        return 0;
    }
    // Computed before the cell index.
    Feature_00423c50* f = &g_game->features[feature];
    int n = cell - g_game->cells;
    int w = g_game->width;
    Point16_00423c50 at;
    at.x = n % w;
    at.z = n / w;
    if (at.x + f->footprint.x > w)
        return 0;
    if (at.z + f->footprint.z > g_game->height)
        return 0;
    // Both footprint loops walk an explicit `c++` pointer, not `row[x]`.
    for (int z = 0; z < f->footprint.z; z++) {
        Cell_00423c50* c = &cell[z * g_game->width];
        for (int x = 0; x < f->footprint.x; x++, c++) {
            if (c->feature != 0xffff && !RemoveFeature(c, 0))
                return 0;
        }
    }
    if (!f->flag0) {
        int i = AllocSpot_004232a0();
        if (i >= 0x800)
            return 0;
        spot = &g_game->pool.entries[i];
        spot->feature = feature;
        spot->damage = 0;
        spot->cell = at;
        if (pos)
            spot->pos = *pos;
        else
            spot->pos = FootprintCentre_00421eb0(&at, f);
        if (rot)
            spot->rot = *rot;
        else
            memset(&spot->rot, 0, sizeof(spot->rot));
        spot->state = CreateObjectState(f->object);
        cell->feature = feature;
        cell->spot = i;
        cell->flag0 = 1;
    } else {
        cell->feature = feature;
        cell->spot = 0;
        cell->flag0 = 0;
    }
    cell->owner = owner;
    for (int z2 = 0; z2 < f->footprint.z; z2++) {
        Cell_00423c50* c = &cell[z2 * g_game->width];
        for (int x = 0; x < f->footprint.x; x++, c++) {
            if (x != 0 || z2 != 0) {
                c->feature = 0xfffe;
                c->offsetX = x;
                c->offsetZ = z2;
                c->flag0 = 0;
            }
        }
    }
    if (f->bit5) {
        if (pos) {
            FUN_00472c50(pos, 4);
        } else {
            Vec3_00423c50 p = FootprintCentre_00421eb0(&at, f);
            FUN_00472c50(&p, 4);
        }
    }
    int index = cell - g_game->cells;
    Point16_00423c50 p2;
    p2.x = index % g_game->width;
    p2.z = index / g_game->width;
    RefreshAllPassMaps(p2, f->footprint);
    return spot;
}
