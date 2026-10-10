// Decompiled by Space Bunny Free, finished by space-bunny-free, confirmed by deepseek-v4.1-flash, re-checked by deepseek-v4.1-flash, second pass by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// Ground height under a unit's footprint: walks the rectangle of map cells the
// unit covers and keeps the lowest cell floor over the cells whose footprint
// mask has bit 3 set, plus the highest floor over the cells with bit 3 and the
// highest floor over the cells with bit 4 set. The bit 4 maximum is never read
// (see the bug note), so the result is the low value, or the water surface
// minus the type's draft when no mask bit was set at all.
// The 24 unused extern declarations below stay: the declaration count in front
// of the function decides the operand order of the footprint mask load.
extern int pad47d820_0;
extern int pad47d820_1;
extern int pad47d820_2;
extern int pad47d820_3;
extern int pad47d820_4;
extern int pad47d820_5;
extern int pad47d820_6;
extern int pad47d820_7;
extern int pad47d820_8;
extern int pad47d820_9;
extern int pad47d820_10;
extern int pad47d820_11;
extern int pad47d820_12;
extern int pad47d820_13;
extern int pad47d820_14;
extern int pad47d820_15;
extern int pad47d820_16;
extern int pad47d820_17;
extern int pad47d820_18;
extern int pad47d820_19;
extern int pad47d820_20;
extern int pad47d820_21;
extern int pad47d820_22;
extern int pad47d820_23;
#pragma pack(push, 1)

#include "../util/vec3.h"

struct Cell {
    char unknown_0[0x5];
    unsigned char high;                 // +0x5, highest floor
    unsigned char low;                  // +0x6, lowest floor
    char unknown_7[0xd - 0x7];          // 13 bytes per cell
};

struct Unit_0047d820 {
    char unknown_0[0x14a];
    Point16 origin;                     // +0x14a, footprint in map cells
    unsigned char* mask;                // +0x14e, one byte per footprint cell
    char unknown_152[0x22c - 0x152];
    unsigned char draft;                // +0x22c
};

struct Game {
    char unknown_0[0x14233];
    int mapWidthTiles;                  // +0x14233
    int mapHeightTiles;                 // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell* heightMap;                    // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

// Stays in a file of its own: in the merged file the symbol count changes the
// operand order of the footprint mask load.
// FUNCTION: 0x47d820
int __stdcall GetFootprintHeight(Unit_0047d820* unit, Point16 cell)
{
    // Declared in this order: y before the footprint copy before x.
    short y = cell.y;
    Point16 fp = unit->origin;
    short x = cell.x;
    if (x < 1 || y < 1 || x + fp.x >= g_game->mapWidthTiles || y + fp.y >= g_game->mapHeightTiles)
        return 0;
    int width = g_game->mapWidthTiles;
    Cell* c = &g_game->heightMap[y * width + x];
    unsigned char low = 0xff, high = 0, high2 = 0;
    int i = 0;
    for (int row = fp.y; row > 0; row--) {
        for (int col = fp.x; col > 0; col--) {
            int f = unit->mask[i++];
            if (f & 8) {
                if (c->low < low)
                    low = c->low;
                if (c->high > high)
                    high = c->high;
            }
            if (f & 0x10) {
                if (c->high > high2)
                    high2 = c->high;
            }
            c++;
        }
        c += width - fp.x;
    }
    unsigned char r;
    if (high < low)
        r = g_game->seaLevel - unit->draft;
    else
        r = low;
    return r;
}
