// Decompiled by Opus. Names are provisional.
// Returns the map cell at grid position (x, y), or null when it lies outside
// the map. Sibling of GetMapCellAtPosition.

#pragma pack(push, 1)
struct Cell_00481550 {
    char unknown_0[0xd];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00481550* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x481550
Cell_00481550* __stdcall GetMapCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}
