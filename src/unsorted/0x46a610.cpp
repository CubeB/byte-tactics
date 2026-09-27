// Decompiled by space-bunny-free. Names are provisional.
// Draws one cell of the map/visibility grid: works out the blit position of
// the cell (the feature's footprint offset, the smoothed shading of the four
// cells of the 2x2 block, the cell's screen position and the scroll offset),
// then either blits the spot's animation, copies the spot's position and
// rotation into the local unit, or draws the feature's own frames (with the
// shadow layer when bit 4 of the draw flags is set).
//
// Still differs (38% of the bytes match). The register allocation of the
// prologue is the blocker: the original keeps g_game in ebx, the feature
// pointer in esi, x in ebp and y in edi, and evaluates the four-cell shade
// sum in edi (the y register) starting from cell->shade. This version gets
// the feature pointer into esi right, but puts g_game in ebp, x in ebx and
// the shade sum in edx, and folds the sum from the last term backwards.
// <ddraw.h> is included because the allocation only lands this way with it,
// and headers.py found no better set. The field at cell+0xc is tested with
// `mov dl, 1; test dl, al` here and `test al, 1` in the original.

#include <ddraw.h>

struct Vec3 {
    int x, y, z;
};

struct Point16 {
    short x, z;
};

struct Rot16 {
    short x, y, z;
};

// What FUN_004b7ee0 looks a frame up with: an index and the table it is in.
struct Handle {
    unsigned short index;
    char unknown_2[6];
    void* table;
};

#pragma pack(push, 1)
struct Feature {
    char name[0x94];
    Point16 footprint;                 // +0x94
    char unknown_98[0xac - 0x98];
    unsigned short* animTable;         // +0xac
    unsigned short* shadowTable;       // +0xb0
    char unknown_b4[0xcc - 0xb4];
    Handle anim;                       // +0xcc
    Handle shadowAnim;                 // +0xd8
    char unknown_e4[0xfe - 0xe4];
    unsigned char drawn : 1;           // +0xfe
    unsigned char over : 1;
    unsigned char flipAnim : 1;
    unsigned char flipShadow : 1;
    unsigned char unknown_bits : 4;
    char unknown_ff;
};

// A live spot: the unit whose state it belongs to, which owns it in turn.
struct SpotState {
    char unknown_0[0xc];
    void* owner;                       // +0xc
};

// A cell of the map grid, 13 bytes: which feature stands on it, which spot
// (for a moving feature) and the four-cell block's shading.
struct Cell {
    char unknown_0[4];
    unsigned char shade;               // +0x4
    char unknown_5[0x8 - 0x5];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct FeatureSpot {
    char unknown_0[4];
    union {
        SpotState* state;              // +0x4
        Handle anim;                   // +0x4
    };
    union {
        Vec3 pos;                      // +0x8
        struct {
            int unknown_8;             // +0x8
            Handle shadow;             // +0x10
            int unknown_1c;            // +0x1c
        } alt;
    };
    Rot16 rot;                         // +0x20
    char unknown_26[0x2f - 0x26];
    unsigned char spotFlags;           // +0x2f
    char unknown_30;
};

struct Unit {
    char unknown_0[0x64];
    Rot16 rot;                         // +0x64
    Vec3 pos;                          // +0x6a
    char unknown_76[0x9e - 0x76];
    SpotState* state;                  // +0x9e
};

struct Game {
    char unknown_0[0x1420b];
    FeatureSpot* spots;                // +0x1420b
    Unit* unit;                        // +0x1420f
    char unknown_14213[0x14233 - 0x14213];
    int width;                         // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x1431f - 0x14273];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
    char unknown_14327[0x37f06 - 0x14327];
    unsigned short drawFlags;          // +0x37f06
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004b7ee0(Handle* h);
int __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004b7f90(void* dest, short* frame, int x, int y);
void __stdcall FUN_004b8500(void* dest, short* frame, int x, int y);
void __stdcall FUN_0045ac20(void* dest, Unit* unit);

// Bit 4 of the draw flags word: shadows may be drawn.
static int DrawFlags()
{
    return *(unsigned char*)((char*)g_game + 0x37f06);
}

// A frame is drawn mirrored when the feature says so.
static void DrawFlip(bool flip, void* dest, short* frame, int x, int y)
{
    if (flip)
        FUN_004b8500(dest, frame, x, y);
    else
        FUN_004b7f90(dest, frame, x, y);
}

// FUNCTION: 0x46a610
void __stdcall FUN_0046a610(void* dest, Cell* cell, int ix, int iy)
{
    Feature* f = &g_game->features[cell->feature];
    Cell* next = cell + g_game->width;
    int shade = (cell->shade + cell[1].shade + next->shade + next[1].shade) >> 3;
    int x = f->footprint.x * 16 / 2 + (ix + 8) * 16 - g_game->scroll_x;
    int y = f->footprint.z * 16 / 2 - shade + (iy + 2) * 16 - g_game->scroll_y;
    if (cell->flags & 1) {
        FeatureSpot* spot = &g_game->spots[cell->spot];
        if (f->drawn) {
            if ((spot->spotFlags & 4) && (DrawFlags() & 0x10))
                FUN_004b7f90(dest, (short*)FUN_004b7ee0(&spot->alt.shadow), x, y);
            FUN_004b7f90(dest, (short*)FUN_004b7ee0(&spot->anim), x, y);
        } else {
            Unit* unit = g_game->unit;
            SpotState* st = spot->state;
            unit->state = st;
            st->owner = unit;
            unit->rot = spot->rot;
            unit->pos = spot->pos;
            FUN_0045ac20(dest, unit);
        }
    } else if (f->over) {
        if (f->shadowTable && (DrawFlags() & 0x10))
            DrawFlip(f->flipShadow, dest, (short*)FUN_004b7ee0(&f->shadowAnim), x, y);
        if (f->animTable)
            DrawFlip(f->flipAnim, dest, (short*)FUN_004b7ee0(&f->anim), x, y);
    } else {
        if (f->shadowTable && (DrawFlags() & 0x10))
            DrawFlip(f->flipShadow, dest, (short*)FUN_004b7f30(f->shadowTable, 0), x, y);
        if (f->animTable)
            DrawFlip(f->flipAnim, dest, (short*)FUN_004b7f30(f->animTable, 0), x, y);
    }
}
