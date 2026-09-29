// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#pragma pack(push, 1)

struct Cell_004848e0 {
    unsigned char level0;              // +0x0
    unsigned char level1;              // +0x1
};

struct Grid_004848e0 {
    Cell_004848e0* cells;              // +0x0
    int width;                         // +0x4
    int height;                        // +0x8
    int count;                         // +0xc
};

struct Rect_004848e0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Flags_004848e0 {
    unsigned short damagebars : 1;
    unsigned short antiAlias : 1;
    unsigned short shadows : 1;
    unsigned short vehicleShadows : 1;
    unsigned short featureShadows : 1;
    unsigned short shading : 1;
    unsigned short ditheredFog : 1;
    unsigned short unused7 : 1;
    unsigned short switchAlt : 1;
};

struct Game_004848e0 {
    char unknown_0[0xdcb];
    unsigned char colors[16];          // +0xdcb
    char unknown_ddb[0x1421f - 0xddb];
    Grid_004848e0* grid;               // +0x1421f
    char unknown_14223[0x14281 - 0x14223];
    unsigned short flags_14281;        // +0x14281
    char unknown_14283[0x1431f - 0x14283];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x1485b - 0x14327];
    void* fog;                         // +0x1485b
    void* black[4];                    // +0x1485f
    void* gray[4];                     // +0x1486f
    char unknown_1487f[0x37e27 - 0x1487f];
    Rect_004848e0 rect;                // +0x37e27
    char unknown_37e37[0x37f06 - 0x37e37];
    Flags_004848e0 flags_37f06;        // +0x37f06
};
#pragma pack(pop)

extern Game_004848e0* g_game;

void FUN_004843c0();
void* __stdcall FUN_004b7f30(void* table, int index);
void __stdcall FUN_004b7f90(void* surface, void* bmp, int x, int y);
void __stdcall FUN_004b86e0(void* surface, void* bmp, int x, int y);
void __stdcall FUN_004b88d0(void* surface, void* bmp, int x, int y, int color);
void __stdcall FUN_004bf6f0(void* surface, Rect_004848e0* rect, int color);
void __stdcall FUN_004bfe10(void* surface, Rect_004848e0* rect);
void __stdcall FUN_004bff20(void* surface, Rect_004848e0* rect, int color);

// The parity sum at 0x484943 is written with one operand read back through
// g_game (g_game->scrollX) instead of the local. That read is CSE'd with the
// earlier local, so no reload appears, but it makes MSVC emit the LEA with
// ebx as base and edi as index, which is the original's encoding. Writing the
// sum from the two locals alone always gives the mirrored [edi+ebx].
//
// FUNCTION: 0x4848e0
void __stdcall FUN_004848e0(void* surface)
{
    if (!(g_game->flags_14281 & 8)) {
        FUN_004843c0();
        g_game->flags_14281 |= 8;
    }

    Grid_004848e0* grid = g_game->grid;
    int scrollY = g_game->scrollY;
    int scrollX = g_game->scrollX;
    int q = (scrollY + 16) / 32 + (scrollX + 16) / 32;
    int parity = (g_game->scrollX + scrollY) & 1;
    int rX = scrollX % 32;
    int rY = scrollY % 32;
    int fx;
    if (rX < 16) fx = -16 - rX;
    else fx = 16 - rX;
    int fy;
    if (rY < 16) fy = -16 - rY;
    else fy = 16 - rY;
    Rect_004848e0 r;

    for (unsigned int j = 0; j < (unsigned int)grid->height; j++) {
        for (unsigned int i = 0; i < (unsigned int)grid->width; i++) {
            Cell_004848e0* cell = &grid->cells[j * grid->width + i];
            r.left = g_game->rect.left + fx + (i << 5);
            r.top = g_game->rect.top + fy + (j << 5);
            r.right = r.left + 31;
            r.bottom = r.top + 31;
            if (cell->level0 == 0xf) {
                FUN_004bf6f0(surface, &r, g_game->colors[0]);
            } else {
                if (cell->level1 != 0) {
                    if (cell->level1 != 0xf) {
                        void* bmp = FUN_004b7f30(g_game->gray[(i + j + q) & 3], cell->level1 - 1);
                        if (g_game->flags_37f06.ditheredFog) {
                            FUN_004b88d0(surface, bmp, r.left, r.top, parity);
                        } else {
                            FUN_004b86e0(surface, bmp, r.left, r.top);
                        }
                    } else {
                        if (g_game->flags_37f06.ditheredFog) {
                            FUN_004bff20(surface, &r, parity);
                        } else {
                            FUN_004bfe10(surface, &r);
                        }
                    }
                }
                if (cell->level0 > 0) {
                    void* bmp = FUN_004b7f30(g_game->black[(i + j + q) & 3], cell->level0 - 1);
                    FUN_004b7f90(surface, bmp, r.left, r.top);
                }
            }
        }
    }
}
