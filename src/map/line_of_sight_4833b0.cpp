// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Clears map features along the border of the playable view: first the last two
// columns of every row, then for every column the cells from the top and from
// the bottom until the cell's ground height passes the scan line, and finally,
// when the game is networked, every cell whose ground is at or below sea level.
//
// <windows.h> is included only for MSVC's register allocation: without a header
// the first cell lookup takes eax for the map width, and the whole function is
// 62.7% instead of 100%.
//
// The second scan's lower bound test is on the pixel row (`py >= 0`), not on the
// cell row, which is why it needs its own accessor; the cell row differs from
// the pixel row by the `>> 4` the loop never writes out.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_004833b0 {
    unsigned short spot;               // +0x0
    char unknown_2[2];
    unsigned char height;              // +0x4
    unsigned char high;                // +0x5
    unsigned char low;                 // +0x6
    char unknown_7;
    unsigned short feature;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Net_004833b0 {
    char unknown_0[0xd44];
    int field_d44;                     // +0xd44
};

struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223, map size in x
    int baseY;                         // +0x14227, map size in y
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell_004833b0* cells;              // +0x14287
    char unknown_1428b[0x391e9 - 0x1428b];
    Net_004833b0* net;                 // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

static inline Cell_004833b0* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// Same lookup, but the row's sign is tested on the 16.16 pixel row `py` that
// the caller keeps in parallel with the cell row `y`.
static inline Cell_004833b0* GetCellPixel(int x, int y, int py)
{
    if (x >= 0 && x < g_game->width && py >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

static inline void ClearFeature(Cell_004833b0* c)
{
    unsigned short f = c->feature;
    if (f == 0xffff || f == 0xfffe)
        c->feature = 0xfffd;
}

// FUNCTION: 0x4833b0
void FUN_004833b0()
{
    g_game->mapWidth = g_game->baseX - 0x20;
    g_game->mapHeight = g_game->baseY - 0x80;

    int col = g_game->width - 2;
    for (int row = 0; row < g_game->height; row++) {
        Cell_004833b0* c = GetCell(col, row);
        ClearFeature(c);
        ClearFeature(c + 1);
    }

    for (int x = 0; x < g_game->width; x++) {
        int y = 0;
        int y16 = 0;
        for (;; y++, y16 += 16) {
            Cell_004833b0* c = GetCellPixel(x, y, y16);
            if (y16 - (c->height >> 1) >= 0)
                break;
            ClearFeature(c);
        }
    }

    for (int x2 = 0; x2 < g_game->width; x2++) {
        int y2 = g_game->height - 1;
        int y16b = y2 * 16;
        for (;; y2--, y16b -= 16) {
            Cell_004833b0* c = GetCell(x2, y2);
            if (y16b - (c->height >> 1) <= g_game->mapHeight)
                break;
            Cell_004833b0* prev = c - g_game->width;
            ClearFeature(prev);
        }
    }

    if (g_game->net->field_d44 != 0) {
        Cell_004833b0* c = g_game->cells;
        Cell_004833b0* end = c + g_game->width * g_game->height;
        while (c < end) {
            if (c->low <= g_game->seaLevel)
                ClearFeature(c);
            c++;
        }
    }
}
