// Decompiled by space-bunny-free. Names are provisional.
// Not matched yet, 75.3%. Still differs from the original in the prologue only:
// the original loads b.y and b.x and subtracts both differences into eax and
// ecx before the register pushes, and the second difference stays in ecx
// across the absolute-value block, while here b.x is loaded later, the
// difference lands in esi and ecx is reused for the abs temporaries. Every
// instruction from the loop head on (the cell lookup, the unit interpolation,
// the loop tail) matches. The two Vec3 arguments are passed by value, which
// is what keeps the otherwise dead `a.y += dh` store alive in the argument
// slot; six plain int parameters let MSVC drop it. Ruled out: all six orders
// of the three difference declarations, static inline helpers per difference,
// spelling each subtraction as two statements, every header set (headers.py)
// and the compiler-state probe (0 to 82 unused extern declarations).
#include <stdlib.h>

#pragma pack(push, 1)
struct Cell_004851c0 {
    unsigned short unit;               // +0x0
    char unknown_2[0x4 - 0x2];
    unsigned char height;              // +0x4
    char unknown_5[0x8 - 0x5];
    unsigned short field_8;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Type_004851c0 {
    char unknown_0[0x16e];
    int field_16e;                     // +0x16e
};

struct Unit_004851c0 {
    char unknown_0[0x6e];
    int field_6e;                      // +0x6e
    char unknown_72[0x92 - 0x72];
    Type_004851c0* type;               // +0x92
    char unknown_96[0x118 - 0x96];
};

struct Game_004851c0 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    unsigned char* mapping;            // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_004851c0* cells;              // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Unit_004851c0* units;              // +0x14357
};
#pragma pack(pop)

extern Game_004851c0* g_game;

struct Vec3_004851c0 {
    int x;
    int y;
    int z;
};

static inline Cell_004851c0* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x4851c0
int __stdcall FUN_004851c0(Vec3_004851c0 a, Vec3_004851c0 b)
{
    int dh = b.y - a.y;
    int dx = b.x - a.x;
    int dz = b.z - a.z;
    int n = (abs(dx) < abs(dz) ? abs(dz) : abs(dx)) / 0x100000 + 1;
    dx /= n;
    dz /= n;
    short best = 0;
    for (int i = 0; i <= n; i++) {
        Cell_004851c0* c = GetCell(a.x / 0x100000, a.z / 0x100000);
        if (c) {
            short v = g_game->mapping[c->field_8 * 256 + 0xfa] + c->height;
            if (best < v) best = v;
            if (c->unit) {
                short w = (g_game->units[c->unit].type->field_16e + g_game->units[c->unit].field_6e) >> 16;
                if (best < w) best = w;
            }
        }
        a.x += dx; a.y += dh; a.z += dz;
    }
    return best;
}
