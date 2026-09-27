// Decompiled by Opus. Names are provisional.
// Converts map cell (x, y) to a 16.16 fixed-point position (cell * 16 in x
// and z, the cell's height byte in y); leaves `out` unchanged off the map.
// The zeroing is an inlined memset (a zero register stored three times
// through a copy of the pointer).
#include <string.h>

#pragma pack(push, 1)
struct Cell_00485330 {
    char unknown_0[0x4];
    unsigned char height;               // +0x4
    char unknown_5[0xd - 0x5];
};

struct Game_00485330 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00485330* cells;               // +0x14287
};
#pragma pack(pop)

struct Fixed_00485330 {
    unsigned short frac;
    short whole;
};

struct Pos_00485330 {
    Fixed_00485330 x;                   // +0x0
    Fixed_00485330 y;                   // +0x4
    Fixed_00485330 z;                   // +0x8
};

extern Game_00485330* g_game;

static inline Cell_00485330* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x485330
void __stdcall FUN_00485330(int x, int y, Pos_00485330* out)
{
    Cell_00485330* cell = GetCell(x, y);
    if (cell) {
        memset(out, 0, sizeof(*out));
        out->x.whole = x << 4;
        out->y.whole = cell->height;
        out->z.whole = y << 4;
    }
}
