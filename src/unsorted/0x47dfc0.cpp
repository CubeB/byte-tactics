// Decompiled by space-bunny-free, finished by mimo-v2.6-flash, then by Claude Sonnet 5.5. Names are provisional.
//
// PARTIAL: 76.4%. The feature dispatch now matches the original instruction for
// instruction: `else if (feature != 0xfffe) { blocked = 1; } else { ... }` writes
// the outer `blocked = 1` as its own block, and the reversed inner arms
// (`if (feature >= count) blocked = 1; else bit`, `if (f2 >= 0xfffb) blocked = 0;
// else bit`) give each trivial arm the original's inline fall-through. With the
// two `mov ecx, 1; jmp` blocks shared (a plain `else if (feature == 0xfffe)`
// chain) the compiler hoists one copy to the end, costs 5 bytes and shifts the
// whole tail.
//
// What is left is ONLY the prologue (+2 bytes): the original keeps `y` in ecx and
// computes `imul ecx, ebx` directly, ours keeps `y` in esi and needs an extra
// `mov ecx, ebx` first. The original's bound-check temporaries are
// h->eax / height->esi, ours are h->ecx / height->eax. Everything from the
// unit-record test onward is identical.
//
// Ruled out for the prologue: index operand order (`x + y*width`, `width*y + x`,
// split into two statements), `h + y` operand order, all four bound checks in one
// `||` chain or two, declaration order of index/result/row/locals, an unsigned
// cast on y, and repeating all of those on top of the fixed dispatch. None of
// them moves `y` off esi.
//
// Already correct and worth keeping: `Cell` needs `unknown_2[2]` so its size is
// exactly 0xd, `Feature` needs a tail pad to be exactly 0x100; comparing
// `(int)cell->low` inline instead of naming an `unsigned char low` local removes
// three spill/reload pairs; the loop increments belong in the `for` header
// (`row++, cell += rowStep` and `col++, cell++`); and the unit pointer must be a
// reference, `Unit_0047dfc0*& unit = g_game->units[cell->spot].unit;`, which
// reproduces the original's two-step `lea ecx, [edx+ecx*8]; mov ecx, [ecx]`.
//
// PARTIAL: 74.9%. This is the pathfinder's "can this rectangle be crossed"
// scan. The unit-record access, the loop tail and the result downgrade match;
// what is left is the prologue and the feature dispatch.
//
// Prologue (about 8 instructions): the original keeps `y` in ecx and computes
// the cell index with `imul ecx, ebx; add ecx, edx`, ours keeps `y` in esi and
// needs a `mov ecx, ebx` first. Its `y + h` bound is `mov eax, [h]; mov esi,
// height; add eax, ecx`, ours is `mov ecx, [h]; mov eax, height; add ecx, esi`.
// Feature dispatch (about 6 instructions): the original has two separate
// `mov ecx, 1; jmp` exits reached by `jl` and by `je`, ours merges them into
// shared `jge`/`jne` targets and has an extra `mov ecx, 1` tail, which suggests
// the two "blocked" assignments live in genuinely separate inlined scopes rather
// than one merged variable.
//
// Ruled out: about thirty permutations of the bound checks and the local
// declaration order; the index through a `static inline CellIndex(y, x)`; each
// bound test through its own `static inline` helper; both together; and every
// header set headers.py tries, none of which changes anything here (unlike
// 0x4399f0 and 0x490080, where an unused include was the whole fix).
//
// Already correct and worth keeping: `Cell` needs `unknown_2[2]` so its size is
// exactly 0xd, `Feature` needs a tail pad to be exactly 0x100; comparing
// `(int)cell->low` inline instead of naming an `unsigned char low` local removes
// three spill/reload pairs (that alone was 33.5% to 71.5%); the loop increments
// belong in the `for` header (`row++, cell += rowStep` and `col++, cell++`)
// rather than as body statements, which is what matches the pointer-advance
// scheduling; and the unit pointer must be declared as a reference,
// `Unit_0047dfc0*& unit = g_game->units[cell->spot].unit;`, which reproduces the
// original's two-step `lea ecx, [edx+ecx*8]; mov ecx, [ecx]` load that a plain
// pointer folds into a single instruction.

// Claude Sonnet 5.5 (#599): MATCH. The two bytes and the register differences in
// the prologue were compiler state, not source: the source below was already
// right. Scoring it with N unused `extern int dummyK;` declarations in front gives
// 76.4 for N = 84 and below (which is why the old 0 to 82 sweep looked flat) and
// MATCH at 559 bytes for every N from 92 to 404. tools/headers.py then finds that
// 30 of the 128 header sets reach the same state, among them <stdio.h>,
// <stdlib.h>, <string.h> and <math.h> on their own. The earlier note that no
// header set helped was wrong (or from a version of headers.py that only said
// "identical bytes"). <stdlib.h> is used here because the neighbouring pathfinder
// function 0x4851c0 includes it for abs().
#include <stdlib.h>

#pragma pack(push, 2)
struct Unit_0047dfc0 {
    char unknown_0[0x26];
    unsigned int lastTick;             // +0x26, when the unit last moved
};
#pragma pack(pop)

struct UnitSlot_0047dfc0 {
    Unit_0047dfc0* unit;               // +0x0
    char unknown_4[0x118 - 0x4];
};

#pragma pack(push, 1)
struct Feature_0047dfc0 {
    char unknown_0[0xfe];
    unsigned char flags;               // +0xfe, bit 6 blocks the search
    char unknown_ff[0x100 - 0xff];
};

struct Cell_0047dfc0 {
    unsigned short spot;               // +0x0, index of the unit standing here
    char unknown_2[2];
    unsigned char field_4;
    unsigned char high;                // +0x5
    unsigned char low;                 // +0x6
    unsigned char unknown_7;
    unsigned short feature;            // +0x8
    unsigned char spotY;               // +0xa
    unsigned char spotX;               // +0xb
    unsigned char flags;               // +0xc
};

struct Game_0047dfc0 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_0047dfc0* features;        // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell_0047dfc0* cells;              // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    UnitSlot_0047dfc0* units;          // +0x14357
};
#pragma pack(pop)

struct Pathfinder_0047dfc0 {
    void* field_0;
    short footprintX;                  // +0x4
    short footprintY;                  // +0x6
    short minHeight;                   // +0x8
    short maxHeight;                   // +0xa
    unsigned char wetSlope;            // +0xc
    unsigned char wetSlope2;           // +0xd
    unsigned char drySlope;            // +0xe
    unsigned char drySlope2;           // +0xf
    char unknown_10[0x1c - 0x10];
    unsigned int lastTick;             // +0x1c
};

extern Game_0047dfc0* g_game;

// FUNCTION: 0x47dfc0
int __stdcall FUN_0047dfc0(Pathfinder_0047dfc0* obj, int x, int y, int w, int h)
{
    if (x < 0 || y < 0)
        return 0;
    if (x + w >= g_game->width)
        return 0;
    if (y + h >= g_game->height)
        return 0;
    int index = y * g_game->width + x;
    unsigned int result = 3;
    int row;
    Cell_0047dfc0* cell = &g_game->cells[index];
    int rowStep = g_game->width - w;
    int minHeight = g_game->seaLevel - obj->minHeight;
    int maxHeight = g_game->seaLevel - obj->maxHeight;
    for (row = 0; row < h; row++, cell += rowStep) {
        for (int col = 0; col < w; col++, cell++) {
            unsigned short feature = cell->feature;
            int blocked;
            if (feature == 0xffff) {
                blocked = 0;
            } else if (feature < 0xfffb) {
                if (feature >= g_game->featureCount)
                    blocked = 1;
                else
                    blocked = (g_game->features[feature].flags >> 6) & 1;
            } else if (feature != 0xfffe) {
                blocked = 1;
            } else {
                Cell_0047dfc0* other =
                    cell - (cell->spotY * g_game->width + cell->spotX);
                unsigned short f2 = other->feature;
                if (f2 >= 0xfffb)
                    blocked = 0;
                else
                    blocked = (g_game->features[f2].flags >> 6) & 1;
            }

            if (blocked)
                return 0;
            if (cell->spot != 0) {
                Unit_0047dfc0*& unit = g_game->units[cell->spot].unit;
                if (!unit || unit->lastTick < obj->lastTick)
                    return 0;
            }
            if ((int)cell->low < minHeight)
                return 0;
            if ((int)cell->high > maxHeight)
                return 0;
            unsigned char diff = cell->high - cell->low;
            if (cell->low < g_game->seaLevel) {
                if (diff > obj->drySlope2) {
                    if (diff > obj->drySlope)
                        return 0;
                    if (result > 1)
                        result = 1;
                }
            } else {
                if (diff > obj->wetSlope2) {
                    if (diff > obj->wetSlope)
                        return 0;
                    if (result > 1)
                        result = 1;
                }
            }
        }
    }
    return result;
}
