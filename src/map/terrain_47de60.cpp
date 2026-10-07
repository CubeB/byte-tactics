// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
//
// Decides whether a unit with the given footprint can stand on one map cell:
// the cell's feature must not block it, a unit already standing there must not
// have moved more recently than the mover, and the cell's ground height and
// slope must be inside the unit's limits. Returns 0 when the cell is unusable,
// 1 when it is usable but the slope is a near miss, 3 when it is fully usable.
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

struct Game {
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

extern Game* g_game;

// FUNCTION: 0x47de60
int __stdcall GetPassMapCellValue(Pathfinder_0047de60* obj, Cell_0047de60* cell)
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
        // The width stays an lvalue: a pointer to the field, not a copy of it.
        // A copy has two uses, so it must live in a register, and the one it
        // wins is ecx, which the original uses for spotX; that costs the
        // `mov ecx,[edx+0x14233]` and the register-to-register imul. As a
        // pointer the width folds into `imul eax,[edx+0x14233]` exactly as the
        // original has it, and the two spelled index uses still share one
        // block, which is the other half the original wants.
        int* width = &g_game->width;
        unsigned short f2 =
            (cell - (cell->spotY * *width + cell->spotX))->feature;
        if (0xfffb <= (cell - (cell->spotY * *width + cell->spotX))->feature) {
            blocked = 0;
        } else {
            blocked = (g_game->features[f2].flags >> 6) & 1;
        }
    }
    if (blocked)
        return 0;
    if (cell->spot != 0) {
        // A reference, not a pointer: gives the original's two-step unit load.
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