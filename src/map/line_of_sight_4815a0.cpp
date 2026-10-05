// Decompiled by Opus. Names are provisional.
// Returns the map cell under a 16.16 world position (x at +0, z at +8), or
// null when it lies outside the map.

#pragma pack(push, 1)
struct Cell_004815a0 {
    char unknown_0[0xd];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_004815a0* cells;               // +0x14287
};
#pragma pack(pop)

struct Vec3_004815a0 {
    int x;
    int y;
    int z;
};

extern Game* g_game;

// FUNCTION: 0x4815a0
Cell_004815a0* __stdcall GetMapCellAtPosition(Vec3_004815a0* pos)
{
    int x = pos->x >> 20;
    int y = pos->z >> 20;
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}
