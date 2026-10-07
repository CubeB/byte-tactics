// Decompiled by Opus, finished by deepseek-v4.1-flash, GPT-6.1-sol and space-bunny-free, finished by claude-opus-5-5. Names are provisional.
// Returns the average of the two height bytes (+5, +6) of the map cell under a
// 16.16 fixed-point position, or -1 off the map.
// The header choice settles the order of the two height-byte loads.
#include <memory.h>

#pragma pack(push, 1)
struct Cell_00485140 {
    char unknown_0[0x5];
    unsigned char field_5;              // +0x5
    unsigned char field_6;              // +0x6
    char unknown_7[0xd - 0x7];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00485140* cells;               // +0x14287
};
#pragma pack(pop)

struct Fixed_00485140 {
    unsigned short frac;
    short whole;
};

struct Pos_00485140 {
    Fixed_00485140 x;                   // +0x0
    Fixed_00485140 y;                   // +0x4
    Fixed_00485140 z;                   // +0x8
};

extern Game* g_game;

static inline Cell_00485140* GetCell(int x, int y)
{
    if (x >= 0) {
        // Width read into its own local after the x >= 0 test: decides the register tie.
        int w = g_game->width;
        if (x < w && y >= 0 && y < g_game->height)
            return &g_game->cells[w * y + x];
    }
    return 0;
}

// FUNCTION: 0x485140
int __stdcall GetCellMeanHeight(Pos_00485140* p)
{
    // One statement for x and y: settles the order of the two height-byte loads.
    int x = p->x.whole / 16, y = p->z.whole / 16;
    Cell_00485140* cell = GetCell(x, y);
    if (cell)
        return (cell->field_6 + cell->field_5) >> 1;
    return -1;
}