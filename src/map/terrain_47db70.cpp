// Decompiled by space-bunny-free, finished by mimo-v2.6-flash. Names are provisional.
//
// Suspected original bug, 0x47dd05: the cell's owner word is compared against
// the raw low 16 bits of the second argument, and that argument is never
// dereferenced anywhere in the function. So either the parameter really is an
// owner id dressed up as a pointer, or the original meant other->field_0.
#pragma pack(push, 1)

struct Point_0047db70 {
    short x;
    short y;
};

struct Cell_0047db70 {
    unsigned short field_0;             // +0x0
    char unknown_2[3];
    unsigned char field_5;              // +0x5
    unsigned char field_6;              // +0x6
    char unknown_7;
    unsigned short feature;             // +0x8
    unsigned char offsetY;              // +0xa
    unsigned char offsetX;              // +0xb
    char unknown_c;
};

struct Feature_0047db70 {
    char name[0xfe];
    unsigned char unknown_6 : 6;
    unsigned char steep : 1;            // +0xfe, bit 6
    char unknown_ff[0x100 - 0xff];
};

struct Unit_0047db70 {
    char unknown_0[0x14a];
    Point_0047db70 origin;              // +0x14a, footprint in map cells
    char unknown_14e[0x1be - 0x14e];
    short field_1be;                    // +0x1be
    short field_1c0;                    // +0x1c0
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char field_228;            // +0x228
    unsigned char field_229;            // +0x229
    char unknown_22a[0x22f - 0x22a];
    unsigned char field_22f;            // +0x22f
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int nameCount;                      // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_0047db70* names;            // +0x1426f
    // names ends at 0x14273, so the gap to seaLevel is 12 bytes; sizing this
    // padding off the member's own name puts seaLevel 4 bytes low.
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell_0047db70* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall CanBuildAt(Unit_0047db70* unit, Point_0047db70 cell, int unused1, int unused2);

// Returns non-zero when the cell's ground does not take a unit: water, a cliff
// edge, or a feature type whose name entry has the "steep" bit set.
static inline int SteepCell(Cell_0047db70* c)
{
    unsigned short f = c->feature;
    if (f == 0xffff)
        return 0;
    if (f < 0xfffb) {
        if (f >= g_game->nameCount)
            return 1;
        return g_game->names[f].steep;
    }
    if (f != 0xfffe)
        return 1;
    c = c - (c->offsetY * g_game->width + c->offsetX);
    f = c->feature;
    if (f >= 0xfffb)
        return 0;
    return g_game->names[f].steep;
}

// FUNCTION: 0x47db70
int __stdcall FUN_0047db70(Unit_0047db70* unit, Unit_0047db70* other, Point_0047db70 cell, int flags)
{
    Point_0047db70 fp = unit->origin;
    if (cell.x < 0 || cell.y < 0)
        return flags == 2;
    int fx = fp.x;
    if (fx + cell.x >= g_game->width)
        return flags == 2;
    if (fp.y + cell.y >= g_game->height)
        return flags == 2;
    if (!unit->field_22f)
        return CanBuildAt(unit, cell, 0, 0);
    Cell_0047db70* c = &g_game->cells[cell.y * g_game->width + cell.x];
    // Declared ahead of the assignments: places the tolerance load.
    int minHeight, maxHeight, stride;
    stride = g_game->width - fx;
    unsigned char tolerance = unit->field_228;
    minHeight = g_game->seaLevel - unit->field_1be;
    maxHeight = g_game->seaLevel - unit->field_1c0;
    if (flags != 1)
        return 1;
    // `c += stride` stays in the increment expression: places `row++`.
    for (int row = 0; row < fp.y; row++, c += stride) {
        for (int col = 0; col < fx; col++, c++) {
            if (SteepCell(c))
                return 0;
            if (c->field_0 != 0 && c->field_0 != (unsigned short)other)
                return 0;
            if (c->field_6 < minHeight)
                return 0;
            if (c->field_5 > maxHeight)
                return 0;
            if (c->field_5 - c->field_6 > tolerance) {
                if (c->field_6 >= g_game->seaLevel)
                    return 0;
                if (c->field_5 - c->field_6 > unit->field_229)
                    return 0;
            }
        }
    }
    return 1;
}
