// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial: whole structure (frames, transform, loops and all four edge tails)
// lines up, but register allocation does not. Ours uses eax/edx where the
// original keeps values in ebp/edi/ecx, the xEnd/yEnd pair is scheduled in the
// opposite order, the `bit` and `yEnd` stack slots are swapped, and the tail
// blocks reload g_game into a different register. Layouts and offsets are all
// correct (grid at +0x23 relative to MapInfo, players stride 0x14b, etc.).
#include <string.h>

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

struct Game {
    char pad0[0x1bdf];
    PlayerGrid players[10];            // +0x1bdf, stride 0x14b
    char pad1[0x2a43 - (0x1bdf + 10 * 0x14b)];
    unsigned char playerIndex;         // +0x2a43
    char pad2[0x141fb - 0x2a44];
    MapInfo info;                      // +0x141fb
    char pad3[0x14281 - (0x141fb + 0x7c)];
    unsigned short flags;              // +0x14281
    char pad4[0x1431f - 0x14283];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
};

#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4843c0
void FUN_004843c0(void)
{
    MapInfo* info = &g_game->info;
    Grid* grid = info->grid;
    PlayerGrid* pg = &g_game->players[g_game->playerIndex];
    unsigned int bit = 1 << g_game->playerIndex;

    memset(grid->cells, 0, grid->count * 2);

    int x0 = g_game->scrollX;
    int y0 = g_game->scrollY;
    int ry = y0 % 32;
    int rx = x0 % 32;
    if (rx < 16) x0 = x0 / 32 - 1; else x0 = x0 / 32;
    if (ry < 16) y0 = y0 / 32 - 1; else y0 = y0 / 32;

    int xEnd = x0 + grid->width;
    int yEnd = y0 + grid->height;

    for (int y = y0; y < yEnd; y++) {
        for (int x = x0; x < xEnd; x++) {
            if (x < pg->width && y < pg->height) {
                if (pg->cells[y * pg->width + x] == 0 && (g_game->flags & 2)) {
                    if (x - x0 < grid->width && y - y0 < grid->height)
                        grid->cells[(y - y0) * grid->width + x - x0].hi |= 1;
                    if (x - x0 - 1 < grid->width && y - y0 < grid->height)
                        grid->cells[(y - y0) * grid->width + x - x0 - 1].hi |= 2;
                    if (x - x0 < grid->width && y - y0 - 1 < grid->height)
                        grid->cells[(y - y0 - 1) * grid->width + x - x0].hi |= 4;
                    if (x - x0 - 1 < grid->width && y - y0 - 1 < grid->height)
                        grid->cells[(y - y0 - 1) * grid->width + x - x0 - 1].hi |= 8;
                }
                if ((info->visibilityMask[info->width * y / 2 + x] & bit) == 0) {
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
            if (g_game->flags & 2) {
                if (grid->cells[i].hi & 4) grid->cells[i].hi |= 1;
                if (grid->cells[i].hi & 8) grid->cells[i].hi |= 2;
            }
            if (grid->cells[i].lo & 4) grid->cells[i].lo |= 1;
            if (grid->cells[i].lo & 8) grid->cells[i].lo |= 2;
        }
    }

    if (yEnd > info->height / 2) {
        for (unsigned int i = 0; i < grid->width; i++) {
            if (g_game->flags & 2) {
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
            if (g_game->flags & 2) {
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
        for (unsigned int i = 1; i - 1 < grid->height; i++) {
            if (g_game->flags & 2) {
                if (grid->cells[i * grid->width - 2].hi & 4)
                    grid->cells[i * grid->width - 2].hi |= 8;
                if (grid->cells[i * grid->width - 2].hi & 1)
                    grid->cells[i * grid->width - 2].hi |= 2;
            }
            if (grid->cells[i * grid->width - 2].lo & 4)
                grid->cells[i * grid->width - 2].lo |= 8;
            if (grid->cells[i * grid->width - 2].lo & 1)
                grid->cells[i * grid->width - 2].lo |= 2;
        }
    }
}
