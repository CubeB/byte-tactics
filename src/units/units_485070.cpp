// Decompiled by space-bunny-free, finished by Sonnet 5.5. Names are provisional.
// Bilinear terrain height at a 12.4 fixed point position (x in the high word at
// +2, z at +10), or -1 outside the map.
#pragma pack(push, 1)
#include "../map/cell.h"

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell* cells;                       // +0x14287
};
#pragma pack(pop)

struct Fixed_00485070 {
    unsigned short frac;
    short whole;
};

struct Pos_00485070 {
    Fixed_00485070 x;                  // +0x0
    Fixed_00485070 y;                  // +0x4
    Fixed_00485070 z;                  // +0x8
};

extern Game* g_game;

// FUNCTION: 0x485070
int __stdcall GetGroundHeight(Pos_00485070* p)
{
    int x = p->x.whole;
    int z = p->z.whole;
    int xc = x >> 4;
    int zc = z >> 4;
    int xf = x & 15;
    int zf = z & 15;
    if (xc < 0 || xc + 1 >= g_game->width || zc < 0 || zc + 1 >= g_game->height)
        return -1;
    Cell* c = &g_game->cells[zc * g_game->width + xc];
    int a = c->height;
    int b = c[1].height;
    // Advance the pointer instead of indexing c[width]: gives the original's address chain.
    c += g_game->width;
    int e = c->height;
    int f = c[1].height;
    // Named local used inside the second expression: computed once, stays in ecx.
    int r1 = a + (b - a) * xf / 16;
    return r1 + ((e + (f - e) * xf / 16) - r1) * zf / 16;
}
