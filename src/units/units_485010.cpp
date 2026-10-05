// Decompiled by Opus. Names are provisional.
// Returns the byte at +4 of the map cell under a point, or 0 off the map.

#pragma pack(push, 1)
struct Cell_00485010 {
    char unknown_0[0x4];
    unsigned char field_4;              // +0x4
    char unknown_5[0xd - 0x5];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00485010* cells;               // +0x14287
};
#pragma pack(pop)

struct Point16_00485010 {
    short x;
    short y;
};

extern Game* g_game;

static inline Cell_00485010* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x485010
int __stdcall GetCellHeight(Point16_00485010* p)
{
    Cell_00485010* cell = GetCell(p->x, p->y);
    if (cell)
        return cell->field_4;
    return 0;
}
