// Decompiled by deepseek-v4.1-flash. Names are provisional.

#include "../util/vec3.h"

struct UnitDef;

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x7e];
    Point16 footprint;                 // +0x7e
    char unknown_82[0x92 - 0x82];
    UnitDef* def;                      // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short id;                 // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char player;              // +0xff
};

#include "../units/unit_def.h"

struct Feature {
    char unknown_0[0xfe];
    unsigned char flags;               // +0xfe
    char unknown_ff;
};

struct Cell {
    unsigned short unit;               // +0
    unsigned short unit2;              // +2
    unsigned char height;              // +4
    unsigned char high;                // +5
    unsigned char low;                 // +6
    unsigned char metal;               // +7
    unsigned short feature;            // +8
    unsigned char spotY;               // +0xa
    unsigned char spotX;               // +0xb
    unsigned char flags;               // +0xc
};

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int maxFeature;                    // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature* features;                 // +0x1426f
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell* cells;                       // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

// Can a unit be placed at pos: the whole footprint must be on the map, clear
// of features and of terrain steeper than the unit's tolerance, and the
// footprint centre must be visible to the unit's player (a hidden centre
// returns true, so unexplored ground is buildable).
// Suspected original bug: the visibility index's row term shifts footprint.x
// by 2 where the footprint is a Point16 and the row extent is footprint.y (the
// loop below counts rows with footprint.y).
// Stays in a file of its own: in the merged file the symbol ids move the def
// load and the feature lookup's operand order.
// FUNCTION: 0x47e2d0
int __stdcall CanPlaceFootprintAt(Unit* unit, Vec3* pos)
{
    Point16 footprint = unit->footprint;
    Vec3 v = *pos;
    UnitDef* def = unit->def;
    short a = (v.x - (footprint.x << 19) + 0x80000) >> 20;
    short c = (v.z - (footprint.y << 19) + 0x80000) >> 20;
    if (a < 0 || c < 0)
        return 0;
    Game* g = g_game;
    if (a + footprint.x >= g->width)
        return 0;
    if (c + footprint.y >= g->height)
        return 0;
    int w = g->width;
    int x = footprint.x;
    int idx = ((c >> 1) + (footprint.x >> 2)) * (w >> 1)
            + (a >> 1) + (footprint.x >> 2);
    if (!(g->visibilityMask[idx] & (1 << unit->player)))
        return 1;
    unsigned char field228 = def->maxslope;
    short bd0 = def->maxwaterdepth;
    int sl = g->seaLevel;
    int lowBound = sl - bd0;
    int upperBound = sl - def->minwaterdepth;
    if (lowBound < sl && (def->flags1 & 0x800) && !(def->flags1 & 0x200000))
        lowBound = sl;
    Cell* cell = &g->cells[c * w + a];
    int rowStep = w - x;
    for (int i = 0; i < footprint.y; i++, cell += rowStep) {
        for (int j = 0; j < x; j++, cell++) {
            unsigned short f = cell->feature;
            unsigned int occ;
            if (f == 0xffff) {
                occ = 0;
            } else if (f < 0xfffb) {
                if (f < g->maxFeature)
                    occ = (g->features[f].flags >> 6) & 1;
                else
                    occ = 1;
            } else {
                if (f != 0xfffe) {
                    occ = 1;
                } else {
                    unsigned short f2 = cell[-(cell->spotY * w + cell->spotX)].feature;
                    if (f2 >= 0xfffb)
                        occ = 0;
                    else
                        occ = (g->features[f2].flags >> 6) & 1;
                }
            }
            if (occ != 0)
                return 0;
            if (cell->flags & 2)
                return 0;
            if (cell->unit != 0 && cell->unit != unit->id)
                return 0;
            if (cell->unit2 != 0 && cell->unit2 != unit->id)
                return 0;
            int lo = cell->low;
            if (lo < lowBound)
                return 0;
            int hi = cell->high;
            if (hi > upperBound)
                return 0;
            if (hi - lo > field228)
                return 0;
        }
    }
    return 1;
}
