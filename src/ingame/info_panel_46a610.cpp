// Decompiled by Space Bunny Free. Names are provisional.
// Kept its own file: in info_panel.cpp the declaration order swaps the first
// two shade loads (cell->shade and cell[1].shade).
// Draws one cell of the map/visibility grid: works out the blit position of
// the cell (the feature's footprint offset, the smoothed shading of the four
// cells of the 2x2 block, the cell's screen position and the scroll offset),
// then either blits the spot's animation, copies the spot's position and
// rotation into the local unit, or draws the feature's own frames (with the
// shadow layer when bit 4 of the draw flags is set).

#include <ddraw.h>

#include "../util/vec3.h"

#include "../util/angles.h"

#include "../graphics/handle.h"

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

// The unit's model instance state, the pointer at a unit's +0x9e; same view
// as unit_script.cpp's ObjectState. A live spot carries one at +0x4.
struct Unit;

struct ObjectState {
    char unknown_0[0xc];
    Unit* unit;                        // +0xc
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
            ObjectState* state;        // +0x4
            Vec3 pos;                  // +0x8
            Vec3 vel;                  // +0x14
        };
    };
    Angles16 rot;                  // +0x20
    char unknown_26[0x2f - 0x26];
    unsigned char spotFlags;           // +0x2f
};

struct Unit {
    char unknown_0[0x64];
    Angles16 angles;               // +0x64
    Vec3 pos;                          // +0x6a
    char unknown_76[0x9e - 0x76];
    ObjectState* state;                // +0x9e
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
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x37f06 - 0x14327];
    unsigned short drawFlags;          // +0x37f06
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetGafSequenceFrame(Handle* h);
int __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall DrawFrame(void* dest, short* frame, int x, int y);
void __stdcall DrawFrameBlended(void* dest, short* frame, int x, int y);
void __stdcall DrawUnit(void* dest, Unit* unit);

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
            DrawFrameBlended(dest, frame, x, y); \
        else \
            DrawFrame(dest, frame, x, y); \
    } while (0)

// FUNCTION: 0x46a610
void __stdcall BlitFeatureGaf(void* dest, Cell* cell, int ix, int iy)
{
    Feature* f = &g_game->features[cell->feature];
    int x = f->footprint.x * 16 / 2 + (ix + 8) * 16 - g_game->scrollX;
    // Second row indexed as cell[width], not through a next pointer: load order.
    int s = cell->shade;
    s += cell[1].shade;
    s += cell[g_game->width].shade;
    s += cell[g_game->width + 1].shade;
    int shade = s >> 3;
    int y = f->footprint.y * 16 / 2 - shade + (iy + 2) * 16 - g_game->scrollY;
    if (cell->flags) {
        FeatureSpot* spot = &g_game->spots[cell->spot];
        if (f->drawn) {
            if ((spot->spotFlags & 4) && (DrawFlags() & 0x10)) {
                short* frame = (short*)GetGafSequenceFrame(&spot->shadow);
                DrawFrame(dest, frame, x, y);
            }
            // The anim frame draw is written out here, not shared through one tail.
            {
                short* frame = (short*)GetGafSequenceFrame(&spot->anim);
                DrawFrame(dest, frame, x, y);
            }
        } else {
            Unit* unit = g_game->unit;
            ObjectState* st = spot->state;
            unit->state = st;
            st->unit = unit;
            unit->angles = spot->rot;
            unit->pos = spot->pos;
            DrawUnit(dest, unit);
        }
    } else {
        if (f->over) {
            if (f->shadowTable && (DrawFlags() & 0x10)) {
                short* frame = (short*)GetGafSequenceFrame(&f->shadowAnim);
                DrawFlip(f->flipShadow, dest, frame, x, y);
            }
            if (f->animTable) {
                short* frame = (short*)GetGafSequenceFrame(&f->anim);
                DrawFlip(f->flipAnim, dest, frame, x, y);
            }
        } else {
            if (f->shadowTable && (DrawFlags() & 0x10)) {
                short* frame = (short*)GetGafFrame(f->shadowTable, 0);
                DrawFlip(f->flipShadow, dest, frame, x, y);
            }
            if (f->animTable) {
                short* frame = (short*)GetGafFrame(f->animTable, 0);
                DrawFlip(f->flipAnim, dest, frame, x, y);
            }
        }
    }
}
