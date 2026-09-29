// Decompiled by space-bunny-free. Names are provisional.
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
