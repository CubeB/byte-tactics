// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Point {
    short x, y;
};

struct Vec3 {
    int x, y, z;
};

struct UnitDef;

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x7e];
    Point footprint;                   // +0x7e
    char unknown_82[0x92 - 0x82];
    UnitDef* def;                      // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short id;                 // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char player;              // +0xff
};

struct UnitDef {
    char unknown_0[0x1be];
    short field_1be;                   // +0x1be
    short field_1c0;                   // +0x1c0
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char field_228;           // +0x228
    char unknown_229[0x241 - 0x229];
    unsigned int flags;                // +0x241
};

struct Feature {
    char unknown_0[0xfe];
    unsigned char flags;               // +0xfe
    char unknown_ff;
};

struct Cell {
    unsigned short field_0;            // +0
    unsigned short field_2;            // +2
    unsigned char field_4;             // +4
    unsigned char field_5;             // +5
    unsigned char field_6;             // +6
    unsigned char field_7;             // +7
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
// by 2 where the footprint is a Point and the row extent is footprint.y (the
// loop below counts rows with footprint.y).
// FUNCTION: 0x47e2d0
int __stdcall FUN_0047e2d0(Unit* unit, Vec3* pos)
{
    Point footprint = unit->footprint;
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
    unsigned char field228 = def->field_228;
    short bd0 = def->field_1be;
    int sl = g->seaLevel;
    int lowBound = sl - bd0;
    int upperBound = sl - def->field_1c0;
    if (lowBound < sl && (def->flags & 0x800) && !(def->flags & 0x200000))
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
            if (cell->field_0 != 0 && cell->field_0 != unit->id)
                return 0;
            if (cell->field_2 != 0 && cell->field_2 != unit->id)
                return 0;
            int lo = cell->field_6;
            if (lo < lowBound)
                return 0;
            int hi = cell->field_5;
            if (hi > upperBound)
                return 0;
            if (hi - lo > field228)
                return 0;
        }
    }
    return 1;
}
