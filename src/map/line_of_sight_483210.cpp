// Decompiled by space-bunny-free. Names are provisional.
// Smooths the map cells of a rectangle: each cell keeps the lowest (+6) and
// highest (+5) of its own terrain height (+4) and those of its right, lower
// and lower-right neighbours.
// <stdio.h> is included because headers.py needs it: without it MSVC 5 picks
// the row stride as the base of the two neighbour height loads
// ([edx+eax+4] instead of [eax+edx+4]), which is 2 bytes out of 347. The same
// state is reached with 20 to 58 unused extern declarations in front, so the
// source shape is right and only the compiler's state was wrong.
#include <stdio.h>
#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Cell_00483210 {
    char unknown_0[0x4];
    unsigned char height;               // +0x4
    unsigned char field_5;              // +0x5
    unsigned char field_6;              // +0x6
    char unknown_7[0xd - 0x7];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00483210* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

static inline int SumX(Point a, Point b) { return a.x + b.x; }
static inline int SumY(Point a, Point b) { return a.y + b.y; }

// FUNCTION: 0x483210
void __stdcall UpdateCellHeightRange(Point pos, Point size)
{
    int width = g_game->width;
    int height = g_game->height;
    int xmax = SumX(pos, size);
    int ymax = SumY(pos, size);
    if (pos.x < 0)
        pos.x = 0;
    if (pos.y < 0)
        pos.y = 0;
    if (xmax >= width)
        xmax = width - 1;
    if (ymax >= height)
        ymax = height - 1;
    if (pos.x >= xmax)
        return;
    if (pos.y >= ymax)
        return;
    int col, row;
    for (row = pos.y; row < ymax; row++) {
        Cell_00483210* c = &g_game->cells[row * width + pos.x];
        for (col = pos.x; col < xmax; col++, c++) {
            unsigned char lo = c->height;
            unsigned char hi = c->height;
            if (col < width - 1) {
                unsigned char v = c[1].height;
                if (v < lo)
                    lo = v;
                if (v > hi)
                    hi = v;
            }
            if (row < height - 1) {
                unsigned char v = c[width].height;
                if (v < lo)
                    lo = v;
                if (v > hi)
                    hi = v;
                if (col < width - 1) {
                    unsigned char v2 = c[width + 1].height;
                    if (v2 < lo)
                        lo = v2;
                    if (v2 > hi)
                        hi = v2;
                }
            }
            c->field_6 = lo;
            c->field_5 = hi;
        }
    }
}
