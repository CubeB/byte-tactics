// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Removes the feature standing on a map cell: if the cell is part of a
// footprint (0xfffe), step back to the feature's origin cell first, release
// its spot, then clear the origin cell and every footprint cell of the
// feature, and tell RefreshAllPassMaps which area changed.
//
// The two clears are one inline helper (feature first, then flags). Written
// out by hand in the inner loop, MSVC either keeps the stores in source order
// with the wrong one first or moves the 0xfe constant into bl; only the
// helper gives the original's `and byte ptr [eax], 0xfe` before
// `mov word ptr [eax-4], bx`. `<windows.h>` is needed for the first block.
#include <windows.h>

#pragma pack(push, 1)
struct Point16_004246b0 {
    short x;
    short z;
};

union SpotField_004246b0 {
    struct {
        unsigned char offsetY;         // +0xa
        unsigned char offsetX;         // +0xb
    };
    unsigned short spot;               // +0xa
};

struct Cell_004246b0 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    SpotField_004246b0 sf;             // +0xa
    unsigned char flags;               // +0xc
};

struct Feature_004246b0 {
    char unknown_0[0x94];
    Point16_004246b0 footprint;        // +0x94
    char unknown_98[0xfe - 0x98];
    unsigned short flags;              // +0xfe
};

struct Spot_004246b0 {
    char unknown_0[4];
    void* state;                       // +0x4
    char unknown_8[0x30 - 8];
};

struct Game {
    char unknown_0[0x1420b];
    Spot_004246b0* spots;              // +0x1420b
    char unknown_1420f[0x1421b - 0x1420f];
    int list_1421b;                    // +0x1421b
    char unknown_1421f[0x14233 - 0x1421f];
    int width;                         // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    Feature_004246b0* features;        // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_004246b0* cells;              // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004232f0(int index, int* head);
void __stdcall FreeObjectState(void* state);
void __stdcall RefreshAllPassMaps(Point16_004246b0 a, Point16_004246b0 b);

static inline void ClearCell(Cell_004246b0* c)
{
    c->feature = 0xffff;
    c->flags &= 0xfe;
}

// FUNCTION: 0x4246b0
int __stdcall FUN_004246b0(Cell_004246b0* cell, int flag)
{
    if (cell->feature == 0xfffe)
        cell -= cell->sf.offsetY * g_game->width + cell->sf.offsetX;
    if (cell->feature >= 0xfffb)
        return 0;
    Feature_004246b0* f = &g_game->features[cell->feature];
    if (flag == 0 && (f->flags & 0x200))
        return 0;
    if (cell->flags & 1) {
        Spot_004246b0* spot = &g_game->spots[cell->sf.spot];
        if (!(f->flags & 1)) {
            FreeObjectState(spot->state);
            spot->state = 0;
        }
        FUN_004232f0(cell->sf.spot, &g_game->list_1421b);
    }
    ClearCell(cell);
    for (int y = 0; y < f->footprint.z; y++) {
        Cell_004246b0* row = &cell[y * g_game->width];
        for (int x = 0; x < f->footprint.x; x++) {
            if (row[x].feature == 0xfffe)
                ClearCell(&row[x]);
        }
    }
    int index = cell - g_game->cells;
    Point16_004246b0 p;
    p.x = index % g_game->width;
    p.z = index / g_game->width;
    RefreshAllPassMaps(p, f->footprint);
    return 1;
}
