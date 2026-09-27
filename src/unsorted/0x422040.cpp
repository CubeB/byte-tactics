// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// For every map cell whose feature index is below 0xfffb and whose feature
// definition has a non-zero value at +0xf0 and bit 1 set in its flags byte,
// stamp the feature's value into the byte at +7 of every cell covered by the
// feature footprint.
//
// Partial (73.6%): the whole function matches instruction for instruction
// (298 bytes, same stack frame, same locals, same call sequence) except that
// the outer cell index `i` and the footprint's row counter `y` trade the ebx
// and ebp registers. The original keeps `i` in ebx (so the FUN_00481550 result
// is parked in ebx, spilling `i`) and `y` in ebp; this version keeps `i` in
// ebp and `y` in ebx. Everything else, including the reload of g_game around
// the nested loops and the pointer walk over `cells` from cells+8 by 0xd,
// is identical.
//
// Not changed by: declaring `i` before/outside the loop, declaring x/y/x0/y0
// at function scope, `while`/`do-while`/label+goto instead of `for`, swapping
// the increment order, a Cell* variable vs indexing a cached base, caching or
// not caching the feature index, a per-cell/per-row `static inline` helper,
// the reference-loop form, `if (i == i)` (folded), or any header set
// (tools/headers.py tries all 128 and every one gives 73.6%). Register
// priority seems to need one more reference to `i` inside the innermost loop:
// adding one raises `i` to ecx/edi/ebx but the only constructs that do so
// (recomputing `i % width` in the x-loop) add extra `idiv`s. `<windows.h>` is
// required: without it the cells walk loses its absolute pointer (49.2%).

#include <windows.h>

#pragma pack(push, 1)
struct Feature {
    char unknown_0[0x94];
    short footprintX;                  // +0x94
    short footprintY;                  // +0x96
    char unknown_98[0xf0 - 0x98];
    float value;                       // +0xf0
    char unknown_f4[0xff - 0xf4];
    unsigned char flags;               // +0xff
};

struct Cell {
    char unknown_0[7];
    char field7;                       // +0x7
    unsigned short feature;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell* cells;                       // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

Cell* __stdcall FUN_00481550(int x, int y);

// FUNCTION: 0x422040
void FUN_00422040(void)
{
    Cell* c = g_game->cells;
    for (int i = 0; i < g_game->width * g_game->height; i++, c++) {
        if (c->feature < 0xfffb) {
            Feature* f = &g_game->features[c->feature];
            if (f->value != 0.0f && (f->flags & 2)) {
                int x0 = i % g_game->width;
                int y0 = i / g_game->width;
                for (int y = y0; y < y0 + f->footprintY; y++) {
                    for (int x = x0; x < x0 + f->footprintX; x++) {
                        Cell* cc = FUN_00481550(x, y);
                        if (cc != 0)
                            cc->field7 = (char)f->value;
                    }
                }
            }
        }
    }
}
