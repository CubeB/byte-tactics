// Decompiled by Space Bunny Free. Names are provisional.
// Draws one cell of the map/visibility grid: works out the blit position of
// the cell (the feature's footprint offset, the smoothed shading of the four
// cells of the 2x2 block, the cell's screen position and the scroll offset),
// then either blits the spot's animation, copies the spot's position and
// rotation into the local unit, or draws the feature's own frames (with the
// shadow layer when bit 4 of the draw flags is set).
//
// Two shapes were needed for the bytes to land. The mirrored blit is a macro,
// not an inlined function: the function form gives the two copies of the flip
// test the other way round from the original (and swaps x and y with them).
// The shading sum indexes the second row of the block as cell[width] instead
// of going through a next pointer: with a pointer MSVC loads the four shades
// as cell, next[1], cell[1], next, where the original loads them in source
// order. The two DrawFlip calls in the feature branch share one tail, so the
// anim frame is drawn by writing the body out in each branch.

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
    unsigned short drawn : 1;           // +0xfe
    unsigned short over : 1;
    unsigned short flipAnim : 1;
    unsigned short flipShadow : 1;
    unsigned short unknown_bits : 4;
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
    unsigned char flags : 1;           // +0xc
    unsigned char unknown_d : 7;
};

// A spot, 0x30 bytes: either a pair of animation handles or the live state of
// a moving feature (its state, position and velocity).
struct FeatureSpot {
    short next;                        // +0x0
    short prev;                        // +0x2
    union {
        struct {
            Handle anim;               // +0x4
            Handle shadow;             // +0x10
        };
        struct {
            SpotState* state;          // +0x4
            Vec3 pos;                  // +0x8
            Vec3 vel;                  // +0x14
        };
    };
    Rot16 rot;                         // +0x20
    char unknown_26[0x2f - 0x26];
    unsigned char spotFlags;           // +0x2f
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

// A frame is drawn mirrored when the feature says so. This has to be a macro:
// the same code as an inlined function allocates the flip test and the two
// blit arguments to the other registers.
#define DrawFlip(flip, dest, frame, x, y) \
    do { \
        if (flip) \
            FUN_004b8500(dest, frame, x, y); \
        else \
            FUN_004b7f90(dest, frame, x, y); \
    } while (0)

// FUNCTION: 0x46a610
void __stdcall FUN_0046a610(void* dest, Cell* cell, int ix, int iy)
{
    Feature* f = &g_game->features[cell->feature];
    int x = f->footprint.x * 16 / 2 + (ix + 8) * 16 - g_game->scroll_x;
    int s = cell->shade;
    s += cell[1].shade;
    s += cell[g_game->width].shade;
    s += cell[g_game->width + 1].shade;
    int shade = s >> 3;
    int y = f->footprint.z * 16 / 2 - shade + (iy + 2) * 16 - g_game->scroll_y;
    if (cell->flags) {
        FeatureSpot* spot = &g_game->spots[cell->spot];
        if (f->drawn) {
            if ((spot->spotFlags & 4) && (DrawFlags() & 0x10)) {
                short* frame = (short*)FUN_004b7ee0(&spot->shadow);
                FUN_004b7f90(dest, frame, x, y);
            }
            {
                short* frame = (short*)FUN_004b7ee0(&spot->anim);
                FUN_004b7f90(dest, frame, x, y);
            }
        } else {
            Unit* unit = g_game->unit;
            SpotState* st = spot->state;
            unit->state = st;
            st->owner = unit;
            unit->rot = spot->rot;
            unit->pos = spot->pos;
            FUN_0045ac20(dest, unit);
        }
    } else {
        if (f->over) {
            if (f->shadowTable && (DrawFlags() & 0x10)) {
                short* frame = (short*)FUN_004b7ee0(&f->shadowAnim);
                DrawFlip(f->flipShadow, dest, frame, x, y);
            }
            if (f->animTable) {
                short* frame = (short*)FUN_004b7ee0(&f->anim);
                DrawFlip(f->flipAnim, dest, frame, x, y);
            }
        } else {
            if (f->shadowTable && (DrawFlags() & 0x10)) {
                short* frame = (short*)FUN_004b7f30(f->shadowTable, 0);
                DrawFlip(f->flipShadow, dest, frame, x, y);
            }
            if (f->animTable) {
                short* frame = (short*)FUN_004b7f30(f->animTable, 0);
                DrawFlip(f->flipAnim, dest, frame, x, y);
            }
        }
    }
}
