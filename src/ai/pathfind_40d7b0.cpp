// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Looks up the 2-bit state of map cell (x, y): 0 when the cell is off the map
// or its visibility cell is off the game grid, 2 when the player's bit is not
// set in that visibility cell, else the cell's stored value.
// The map's bounds check and cell read are inline methods (the cell read
// re-reads the width after the bounds check), `g_game->width >> 1` is written
// twice rather than held in a local, and <stdlib.h> is needed: without it the
// first argument lands in eax instead of edx.
#include <stdlib.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;    // +0x14273, one bit per player
};
#pragma pack(pop)

extern Game* g_game;

struct Map_0040d7b0 {
    char unknown_0[4];
    short originX;                     // +0x4
    short originY;                     // +0x6
    char unknown_8[0x10 - 0x8];
    unsigned int width;                // +0x10
    unsigned int height;               // +0x14
    unsigned int* cells;               // +0x18, 16 2-bit cells per dword

    int InBounds(unsigned int x, unsigned int y)
    {
        return x < width && y < height;
    }
    int Get(int x, int y)
    {
        return (cells[width * (y >> 4) + x] >> ((y & 0xf) << 1)) & 3;
    }
};

struct Class_0040d7b0 {
    char unknown_0[0x64];
    Map_0040d7b0* map;                 // +0x64
    char unknown_68[0x78 - 0x68];
    unsigned char player;              // +0x78

    int GetCellState(int x, int y);
};

// FUNCTION: 0x40d7b0
int Class_0040d7b0::GetCellState(int x, int y)
{
    if (!map->InBounds(x, y))
        return 0;
    int cx = (x >> 1) + (map->originX >> 2);
    int cy = (y >> 1) + (map->originY >> 2);
    if (cx >= (g_game->width >> 1) || cy >= (g_game->height >> 1))
        return 0;
    if (!((1 << player) & g_game->visibilityMask[cy * (g_game->width >> 1) + cx]))
        return 2;
    return map->Get(x, y);
}
