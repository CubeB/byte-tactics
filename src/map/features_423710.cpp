// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00423710 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Feature_00423710 {
    char unknown_0[0xf4];
    unsigned short field_f4;           // +0xf4
    unsigned short field_f6;           // +0xf6
    unsigned short field_f8;           // +0xf8
    char unknown_fa[0x100 - 0xfa];
};

struct Vec3_00423710 {
    int x, y, z;
};

struct Rot_00423710 {
    short x, y, z;
};

struct Spot_00423710 {
    char unknown_0[8];
    Vec3_00423710 pos;                 // +0x8
    char unknown_14[0x20 - 0x14];
    Rot_00423710 rot;                  // +0x20
    char unknown_26[0x2f - 0x26];
    unsigned char flags;               // +0x2f
};

struct Game {
    char unknown_0[0x1420b];
    Spot_00423710* spots;              // +0x1420b
    char unknown_1420f[0x1426f - 0x1420f];
    Feature_00423710* features;        // +0x1426f
};
#pragma pack(pop)

extern Game* g_game;

Cell_00423710* __stdcall FUN_00481550(int x, int y);
void __stdcall FUN_004246b0(Cell_00423710* cell, int flag);
void* __stdcall FUN_00423c50(Cell_00423710* cell, unsigned short feature,
                            Vec3_00423710* pos, Rot_00423710* rot,
                            unsigned char owner);

// FUNCTION: 0x423710
void __stdcall FUN_00423710(int x, int y, int flag)
{
    Cell_00423710* cell = FUN_00481550(x, y);
    if (cell == 0)
        return;
    if (cell->feature >= 0xfffb)
        return;
    Feature_00423710* f = &g_game->features[cell->feature];
    unsigned short v;
    if (flag != 0)
        v = f->field_f8;
    else
        v = f->field_f4;
    if (cell->flags & 1) {
        Spot_00423710* spot = g_game->spots + cell->spot;
        if (spot->flags & 2)
            v = f->field_f8;
        FUN_004246b0(cell, 0);
        FUN_00423c50(cell, v, &spot->pos, &spot->rot, 10);
        return;
    }
    FUN_004246b0(cell, 0);
    FUN_00423c50(cell, v, 0, 0, 10);
}
