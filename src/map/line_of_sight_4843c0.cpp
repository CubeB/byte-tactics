// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)

struct Cell {
    unsigned char lo;                  // +0x0
    unsigned char hi;                  // +0x1
};

struct Grid {
    Cell* cells;                       // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int count;                         // +0xc
};

struct PlayerGrid {
    unsigned char* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    char pad[0x14b - 0xc];
};

struct MapInfo {
    char pad0[0x24];
    Grid* grid;                        // +0x24
    char pad1[0x38 - 0x28];
    int width;                         // +0x38
    int height;                        // +0x3c
    char pad2[0x78 - 0x40];
    unsigned short* visibilityMask;    // +0x78
};

union FlagWord {
    unsigned short raw;
    struct {
        unsigned short bit0 : 1;
        unsigned short bit1 : 1;       // mask 2
        unsigned short rest : 14;
    } bits;
};

struct Game {
    char pad0[0x1bdf];
    PlayerGrid players[10];            // +0x1bdf, stride 0x14b
    char pad1[0x2a43 - (0x1bdf + 10 * 0x14b)];
    unsigned char playerIndex;         // +0x2a43
    char pad2[0x141fb - 0x2a44];
    MapInfo info;                      // +0x141fb
    char pad3[0x14281 - (0x141fb + 0x7c)];
    FlagWord flags;                    // +0x14281
    char pad4[0x1431f - 0x14283];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
};

#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4843c0
void BuildFogTiles(void)
{
    MapInfo* info = &g_game->info;
    Grid* grid = info->grid;
    PlayerGrid* pg = &g_game->players[g_game->playerIndex];
    unsigned int bit = 1 << g_game->playerIndex;

    memset(grid->cells, 0, grid->count * 2);

    // Interleaved: each scroll load is followed by its modulo (y0, ry, x0, rx).
    int y0 = g_game->scrollY;
    int ry = y0 % 32;
    int x0 = g_game->scrollX;
    int rx = x0 % 32;
    if (rx < 16) x0 = x0 / 32 - 1; else x0 = x0 / 32;
    if (ry < 16) y0 = y0 / 32 - 1; else y0 = y0 / 32;

    int xEnd = x0 + grid->width;
    int yEnd = y0 + grid->height;

    for (int y = y0; y < yEnd; y++) {
        for (int x = x0; x < xEnd; x++) {
            if (x < pg->width && y < pg->height) {
                if (pg->cells[y * pg->width + x] == 0 && (g_game->flags.raw & 2)) {
                    if (x - x0 < grid->width && y - y0 < grid->height)
                        grid->cells[(y - y0) * grid->width + x - x0].hi |= 1;
                    if (x - x0 - 1 < grid->width && y - y0 < grid->height)
                        grid->cells[(y - y0) * grid->width + x - x0 - 1].hi |= 2;
                    if (x - x0 < grid->width && y - y0 - 1 < grid->height)
                        grid->cells[(y - y0 - 1) * grid->width + x - x0].hi |= 4;
                    if (x - x0 - 1 < grid->width && y - y0 - 1 < grid->height)
                        grid->cells[(y - y0 - 1) * grid->width + x - x0 - 1].hi |= 8;
                }
                if ((unsigned short)(bit & info->visibilityMask[info->width * y / 2 + x]) == 0) {
                    if (x - x0 < grid->width && y - y0 < grid->height)
                        grid->cells[(y - y0) * grid->width + x - x0].lo |= 1;
                    if (x - x0 - 1 < grid->width && y - y0 < grid->height)
                        grid->cells[(y - y0) * grid->width + x - x0 - 1].lo |= 2;
                    if (x - x0 < grid->width && y - y0 - 1 < grid->height)
                        grid->cells[(y - y0 - 1) * grid->width + x - x0].lo |= 4;
                    if (x - x0 - 1 < grid->width && y - y0 - 1 < grid->height)
                        grid->cells[(y - y0 - 1) * grid->width + x - x0 - 1].lo |= 8;
                }
            }
        }
    }

    if (y0 < 0) {
        for (unsigned int i = 0; i < grid->width; i++) {
            if (g_game->flags.bits.bit1) {
                if (grid->cells[i].hi & 4) grid->cells[i].hi |= 1;
                if (grid->cells[i].hi & 8) grid->cells[i].hi |= 2;
            }
            if (grid->cells[i].lo & 4) grid->cells[i].lo |= 1;
            if (grid->cells[i].lo & 8) grid->cells[i].lo |= 2;
        }
    }

    if (yEnd > info->height / 2) {
        for (unsigned int i = 0; i < grid->width; i++) {
            if (g_game->flags.bits.bit1) {
                if (grid->cells[(grid->height - 2) * grid->width + i].hi & 1)
                    grid->cells[(grid->height - 2) * grid->width + i].hi |= 4;
                if (grid->cells[(grid->height - 2) * grid->width + i].hi & 2)
                    grid->cells[(grid->height - 2) * grid->width + i].hi |= 8;
            }
            if (grid->cells[(grid->height - 2) * grid->width + i].lo & 1)
                grid->cells[(grid->height - 2) * grid->width + i].lo |= 4;
            if (grid->cells[(grid->height - 2) * grid->width + i].lo & 2)
                grid->cells[(grid->height - 2) * grid->width + i].lo |= 8;
        }
    }

    if (x0 < 0) {
        for (unsigned int i = 0; i < grid->height; i++) {
            if (g_game->flags.bits.bit1) {
                if (grid->cells[i * grid->width].hi & 8)
                    grid->cells[i * grid->width].hi |= 4;
                if (grid->cells[i * grid->width].hi & 2)
                    grid->cells[i * grid->width].hi |= 1;
            }
            if (grid->cells[i * grid->width].lo & 8)
                grid->cells[i * grid->width].lo |= 4;
            if (grid->cells[i * grid->width].lo & 2)
                grid->cells[i * grid->width].lo |= 1;
        }
    }

    if (xEnd > info->width / 2) {
        // Guarded do-while, not a for: the counter init follows the guard.
        if (grid->height > 0) {
            unsigned int i = 1;
            do {
                if (g_game->flags.bits.bit1) {
                    if (grid->cells[i * grid->width - 2].hi & 4)
                        grid->cells[i * grid->width - 2].hi |= 8;
                    if (grid->cells[i * grid->width - 2].hi & 1)
                        grid->cells[i * grid->width - 2].hi |= 2;
                }
                if (grid->cells[i * grid->width - 2].lo & 4)
                    grid->cells[i * grid->width - 2].lo |= 8;
                if (grid->cells[i * grid->width - 2].lo & 1)
                    grid->cells[i * grid->width - 2].lo |= 2;
                i++;
            } while (i - 1 < grid->height);
        }
    }
}
