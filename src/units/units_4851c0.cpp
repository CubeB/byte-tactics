// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by GPT-6, retried by claude-opus-5-5, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, #5611). The whole residual of the earlier passes
// (84.7%) was one colouring order in the prologue: the x difference had to be
// coloured before the |dx| temporary and the step count n, so that it takes ecx
// and they take esi. Read with tools/c2prio.py, two things were needed:
//  * n computed in each arm of an if/else with one `n++` after it (n 130 -> 96;
//    the two tails merge into the original's bytes);
//  * the x and z differences as named locals that are written back into b
//    before the copy (`int dx = b.x - a.x; b.x = dx;`). A local whose definition
//    is the subtraction is its own candidate and keeps the write-back as a
//    reference, where `b.x -= a.x` or a plain copy of b.x into a local is
//    forwarded into b.x and loses it: dx rises from 68 to 108, above |dx| (106)
//    and n (96). Comparing `abs(dx) < abs(dz)` on the two locals keeps |dx|
//    computed first, in esi, as the original's `cmp esi, eax; jl` needs.
// A batch of 72 prologue orders (each difference in place or as a local, the
// compare written `<` or `>`) gave four matches, all with this shape; the dz
// local must come first and the y difference stays in place on b.
//
// The step loop walks from a to b in n + 1 steps and returns the highest
// ground or unit top it crosses. The y step is the undivided y difference,
// as in the original (only x and z are divided).
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

struct Unit {
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
    Unit* units;                       // +0x14357
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
    int dz = b.z - a.z;
    b.z = dz;
    b.y -= a.y;
    int dx = b.x - a.x;
    b.x = dx;
    Vec3_004851c0 d = b;
    int n;
    if (abs(dx) < abs(dz))
        n = abs(dz) / 0x100000;
    else
        n = abs(dx) / 0x100000;
    n++;
    d.x /= n;
    d.z /= n;
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
        a.x += d.x; a.y += d.y; a.z += d.z;
    }
    return best;
}
