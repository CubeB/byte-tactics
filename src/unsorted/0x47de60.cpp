// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// deepseek-v4.1-flash pass (#1182): tested the header lever. The matched sibling
// 0x47dfc0, which contains the same feature dispatch and the same unit-reference
// block, only reached MATCH after a header set was added (its file note credits
// <stdlib.h>). `uv run tools/headers.py 0x47de60` tries all 128 header sets and
// every one stays at 86.8 percent, so this residual is not reachable by headers
// alone. The 4-byte difference is still exactly the six-instruction index block
// at 0x47dea5 to 0x47deb6.
//
// deepseek-v4.1-flash second pass (#1609), all 86.8 percent and 346 bytes:
//  - both multiply orders, both add orders, `int idx` assigned then added,
//    spotX/spotY hoisted into `unsigned int` or `unsigned short` locals in
//    either order, a `Game* g = g_game` second pointer, and a `Cell* p = cell`
//    second pointer: the index block is still byte-identical to ours, so its
//    scheduling is not reachable from the expression or its locals;
//  - headers.py again: 0 of 128 header sets match, all 86.8.
// The isolated 0x421e60 with the same source emits the fold with the second
// byte in edx (both registers free); here edx is pinned to g_game, and the
// pinned case always picks the `mov eax,[width]; imul eax,ecx` form. A plain
// unit pointer brings the fold back but breaks the two-step unit load, so the
// two are coupled by the register allocator, not by the source shape.
// Decides whether a unit with the given footprint can stand on one map cell:
// the cell's feature must not block it, a unit already standing there must not
// have moved more recently than the mover, and the cell's ground height and
// slope must be inside the unit's limits. Returns 0 when the cell is unusable,
// 1 when it is usable but the slope is a near miss, 3 when it is fully usable.
//
// PARTIAL: 86.8%, 4 bytes out. Everything matches except the six instructions
// that compute the map index in the feature == 0xfffe branch. The original
// keeps spotY in the accumulator and folds the width load into the multiply,
// exactly as the matched 0x421e60.cpp does with the same expression:
//
//   original                     ours
//   xor eax, eax                 xor eax, eax
//   xor ecx, ecx                 mov al, byte ptr [esi + 0xa]
//   mov al, byte ptr [esi + 0xa] mov ecx, eax
//   mov cl, byte ptr [esi + 0xb] mov eax, dword ptr [edx + 0x14233]
//   imul eax, [edx + 0x14233]    imul eax, ecx
//   add eax, ecx                 xor ecx, ecx
//                                mov cl, byte ptr [esi + 0xb]
//                                add eax, ecx
//
// What is known about it:
//  - the declarations are not the cause. Starting from the matched 0x421e60.cpp
//    and adding this file's Game struct and this file's Cell head (spot, high,
//    low before feature) still gives MATCH, so the same declarations still
//    produce the folded imul in a small function;
//  - the unit lookup is the cause. Deleting the `cell->spot` block from this
//    function brings the folded imul back, and every spelling of the index
//    expression (a local, the operands reversed, a cast, a static inline
//    helper, a local g_game pointer, a pointer to the slot) changes nothing
//    while it is present;
//  - the block needs a reference to the slot's member,
//    `Unit_0047de60*& unit = g_game->units[cell->spot].unit;`, to get the
//    original's two-step unit load and to put g_game in edx. With a plain
//    pointer the fold comes back but the whole function drops to 68.6%, so the
//    two effects cannot be had at once from the spellings tried.
//
// Claude Sonnet 5.5 pass (#571), all still 86.8 percent and 346 bytes:
//  - N unused `extern int dummyK;` lines in front of the first pragma, K = 0 to
//    200 step 4: flat 86.8, so it is the source shape, not compiler state (unlike
//    0x47d820 in the same issue, which matches for K = 16 to 80);
//  - the feature test moved into a `static inline int FeatureBlocked(Cell*)`, the
//    unit check into a `static inline` helper, both, and a slot pointer
//    (`UnitSlot* slot = &g_game->units[cell->spot]`) instead of the reference;
//  - spotY and spotX copied into `unsigned char`, `int` or `unsigned int` locals
//    in either order, and the index as a separate `int idx` local.
// From the disassembly (0x47dea5 to 0x47deb6): the original zero-extends both
// byte fields first (`xor eax,eax; xor ecx,ecx`), then loads them, then does
// `imul eax,[edx+0x14233]; add eax,ecx`, so the width is the memory operand of the
// multiply. Ours loads the width into eax and copies spotY to ecx. g_game is loaded
// into edx before `push esi` (0x47de62) and stays there, as in ours.
//
// space-bunny-free pass, 100+ free-scored variants, all 86.8 percent and 346
// bytes except where noted, and in EVERY one the eight instructions of the
// index block came out byte-identical, so the fold is not a source-shape effect:
//  - index expression: both add orders, both multiply orders, a separate
//    `int idx`, hoisted `unsigned int` leaves, `other = cell; other -= ...`,
//    `&cell[-k]`, two successive subtractions, `long`/`unsigned`/`short` casts
//    on the width or the leaves, `1 *` in front, and the two leaves combined
//    into a nested anonymous struct: all 86.8, all byte-identical in the block;
//  - the index hoisted into a `static inline int CellIndex(Cell*)`, and a
//    second one taking the game pointer, and the whole feature test moved into
//    a `static inline int FeatureBlocked(Cell*)` (the matched 0x47db70's
//    SteepCell shape), and the 0xfffe arm alone into a helper, and the unit
//    check into a helper, and all of those together: all 86.8, block unchanged;
//  - eleven spellings of the unit block (the reference, a plain pointer, a slot
//    pointer, slot plus reference, slot plus pointer, a local `unsigned short
//    spot`, a local units pointer, a pre-read `unit->lastTick`, a local
//    `now`, two separate `if`s, a negated condition): 86.8 for every form that
//    keeps the original's two-step unit load, 68.6 or 85.3 for the two that do
//    not, and the fold never appears in any of the eleven;
//  - `width` redeclared `short`, `unsigned short`, `unsigned int` or `char`:
//    83.4 to 86.8, all worse or flat;
//  - declaration order: the two byte fields swapped, either one padded out to
//    its own member, `feature` declared last, `height` before `width`, the
//    `blocked` and `feature` declarations swapped, forward-declared
//    `minHeight`/`maxHeight`/`diff` with the initialisers dropped, a
//    forward-declared `int idx`, and `unsigned int py, px` leaves: 86.8 flat,
//    or worse (72.1 to 86.0);
//  - `#include <string.h>` and `#include <stddef.h>` added ahead of the first
//    pragma: 86.8 flat (this band needs no include, as the two siblings do);
//  - N unused `extern int` declarations in front of the first pragma, N = 0, 4,
//    8, 12, 16, 20, 24, 32, 40, 48, 64, 80: flat 86.8 at every N, so unlike
//    0x47d820 in the same issue there is no declaration state that folds the
//    imul here.
//
// One apparent improvement is a trap and was not taken: writing the arm as
// `cell + (cell->spotX - cell->spotY * g_game->width)` scores 94.1 percent at
// exactly 342 bytes, but the differ shows it is a different instruction
// sequence, not a closer one: MSVC hoists the width into ecx, computes
// `imul ecx, eax` with the width as the register operand, adds the spotX
// difference into eax and then folds the whole 13-byte cell stride into
// `mov ax, word ptr [edi + ecx*4 + 8]`, so the original's
// `lea ecx,[eax+eax*2]; lea eax,[eax+ecx*4]; mov ecx,esi; sub ecx,eax` pointer
// walk is gone. The three instructions that follow the block in the original
// do not exist in that variant, and reaching 94.1 percent is a coincidence of
// the size matching, not progress. Every other variant that scored above 86.8
// did so only by deleting code (45.9 to 86.0), which the size drop shows.
//
// Conclusion: the residual survives every source spelling, declaration order
// and declaration state, and the index block is byte-identical across all of
// them, so as on 0x47d820 and 0x47d970 this is front-end state in this
// translation unit rather than a missing source shape. There is no padding
// lever here to decline either: unlike 0x47d820, no declaration state matched.
// space-bunny-free second pass, 2 check runs, 86.8 percent and 346 bytes:
//  - the whole residual is still the one eight-instruction index block; every
//    other difference the differ prints is a jump target 4 bytes further on,
//    so the rest of the function is instruction-for-instruction identical;
//  - the add order is provably canonicalised: `cell - (cell->spotX +
//    cell->spotY * g_game->width)` compiles byte-for-byte like the opposite
//    order (both operands of the `+` keep the same shape, 86.8, 346 bytes).
//    So the original's spotX-loaded-before-the-multiply schedule, and with it
//    the `imul eax, [edx+0x14233]` fold, are not reachable by writing the `+`
//    the other way round, and the guide's "for `imul reg, [mem]` the register
//    operand is the left side of `*`" cannot be applied here: c1 has already
//    swapped the multiply's own operands by the time it chooses the form
//    (it copies the byte into ecx and loads the width into eax, so the width
//    is the left side of `*` in the generated code no matter how it is
//    written).
// deepseek-v4.1 pass (#2438), 28 check runs, all 86.8 percent and 346 bytes
// with the eight-instruction block byte-identical, except the unsigned short
// cast at 85.2:
//  - the index expression: both multiply orders, both add orders (again),
//    `iy * g_game->width + ix` through `unsigned int` and `short` locals,
//    a separate `unsigned int idx` and `int idx`, casts on the whole sum,
//    `&cell[-k]` and a char-pointer scale-by-0xd spelling: flat, so the
//    expression is not the lever, as the earlier passes found;
//  - unused `extern int dummyN;` padding in front of the first pragma at the
//    odd counts N = 1, 3, 5, 7, 9, 11, 13, 15 (the earlier sweeps used step 4
//    only), and `N = 0..15` combined with the two multiply orders: flat, so
//    unlike 0x47d820 there is no declaration state that folds the imul here.
// The matched sibling 0x47dfc0 has this exact expression and folds the imul as
// `xor ecx,ecx; xor edx,edx; mov cl,[esi+0xa]; mov dl,[esi+0xb];
// imul ecx,[edi+0x14233]; add ecx,edx`, i.e. its pair is (ecx, edx) because in
// its loop eax holds the pending `blocked` value while g_game sits in edi. Our
// function needs the pair (eax, ecx) with g_game in edx, and that pinned pair
// is what makes c1 copy the byte out of eax and load the width into it.
#pragma pack(push, 1)
struct Feature_0047de60 {
    char unknown_0[0xfe];
    unsigned char flags;               // +0xfe, bit 6 blocks the move
    char unknown_ff[0x100 - 0xff];
};

struct Cell_0047de60 {
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

struct Unit_0047de60 {
    char unknown_0[0x26];
    unsigned int lastTick;             // +0x26, when the unit last moved
};

struct UnitSlot_0047de60 {
    Unit_0047de60* unit;               // +0x0
    char unknown_4[0x118 - 0x4];
};

struct Game_0047de60 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_0047de60* features;        // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14357 - 0x14280];
    UnitSlot_0047de60* units;          // +0x14357
};

struct Pathfinder_0047de60 {
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
#pragma pack(pop)

extern Game_0047de60* g_game;

// FUNCTION: 0x47de60
int __stdcall FUN_0047de60(Pathfinder_0047de60* obj, Cell_0047de60* cell)
{
    int blocked;
    unsigned short feature = cell->feature;
    if (feature == 0xffff) {
        blocked = 0;
    } else if (feature < 0xfffb) {
        if (feature >= g_game->featureCount) {
            blocked = 1;
        } else {
            blocked = (g_game->features[feature].flags >> 6) & 1;
        }
    } else if (feature != 0xfffe) {
        blocked = 1;
    } else {
        Cell_0047de60* other =
            cell - (cell->spotY * g_game->width + cell->spotX);
        unsigned short f2 = other->feature;
        if (f2 >= 0xfffb) {
            blocked = 0;
        } else {
            blocked = (g_game->features[f2].flags >> 6) & 1;
        }
    }
    if (blocked)
        return 0;
    if (cell->spot != 0) {
        Unit_0047de60*& unit = g_game->units[cell->spot].unit;
        if (!unit || unit->lastTick < obj->lastTick)
            return 0;
    }
    int minHeight = g_game->seaLevel - obj->minHeight;
    if ((int)cell->low < minHeight)
        return 0;
    int maxHeight = g_game->seaLevel - obj->maxHeight;
    if ((int)cell->high > maxHeight)
        return 0;
    unsigned char diff = cell->high - cell->low;
    if (cell->low < g_game->seaLevel) {
        if (diff > obj->drySlope2)
            return !(obj->drySlope < diff);
    } else {
        if (diff > obj->wetSlope2)
            return !(obj->wetSlope < diff);
    }
    return 3;
}
