// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (93.8%), best found. `<windows.h>` is required: with it the first
// block matches exactly (`xor eax,eax / xor edx,edx / mov al,[edi+0xa] /
// mov dl,[edi+0xb] / imul eax,[width] / add eax,edx`) and the function reaches
// 93.8%; without it the first block is 82.9%. The one remaining difference is
// the order of the two clears in the inner footprint loop. The original emits
// `and byte ptr [eax],0xfe` (flags) then `mov word ptr [eax-4],bx` (feature),
// i.e. its source wrote the flags clear first. Writing that order (row[x].flags
// &= 0xfe; row[x].feature = none;) makes MSVC keep 0xfe in `bl` and store the
// feature as an immediate, which breaks the main-cell clear and the feature
// register (`and cl,0xfe` / `mov [edi+8],bx`) and drops to 81.2%. Every
// formulation tried (locals, casts, references, inline helpers, reordered main
// cell, global/const `none`, headers) either keeps 93.8% with feature-first
// stores or 81.2% with flags-first stores; the constant-register choice is
// global and flips with the order.
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
    short footprintX;                  // +0x94
    short footprintZ;                  // +0x96
    char unknown_98[0xfe - 0x98];
    unsigned char flags_fe;            // +0xfe
    unsigned char flags_ff;            // +0xff
};

struct Spot_004246b0 {
    char unknown_0[4];
    void* state;                       // +0x4
    char unknown_8[0x30 - 8];
};

struct Game_004246b0 {
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

extern Game_004246b0* g_game;

void __stdcall FUN_004232f0(int index, int* head);
void __stdcall FUN_0045aaa0(void* state);
void __stdcall FUN_00440a40(Point16_004246b0 a, Point16_004246b0 b);

// FUNCTION: 0x4246b0
int __stdcall FUN_004246b0(Cell_004246b0* cell, int flag)
{
    unsigned short none = 0xffff;
    if (cell->feature == 0xfffe)
        cell -= cell->sf.offsetY * g_game->width + cell->sf.offsetX;
    if (cell->feature >= 0xfffb)
        return 0;
    Feature_004246b0* f = &g_game->features[cell->feature];
    if (flag == 0 && (f->flags_ff & 2))
        return 0;
    if (cell->flags & 1) {
        Spot_004246b0* spot = &g_game->spots[cell->sf.spot];
        if (!(f->flags_fe & 1)) {
            FUN_0045aaa0(spot->state);
            spot->state = 0;
        }
        FUN_004232f0(cell->sf.spot, &g_game->list_1421b);
    }
    cell->feature = none;
    cell->flags &= 0xfe;
    for (int y = 0; y < f->footprintZ; y++) {
        Cell_004246b0* row = &cell[y * g_game->width];
        for (int x = 0; x < f->footprintX; x++) {
            if (row[x].feature == 0xfffe) {
                row[x].feature = none;
                row[x].flags &= 0xfe;
            }
        }
    }
    int index = cell - g_game->cells;
    Point16_004246b0 p;
    p.x = index % g_game->width;
    p.z = index / g_game->width;
    FUN_00440a40(p, *(Point16_004246b0*)&f->footprintX);
    return 1;
}
