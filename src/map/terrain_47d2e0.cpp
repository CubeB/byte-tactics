// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, retried by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, retried by claude-opus-5-5, finished by GPT-6, retried by claude-opus-5-5, retried by claude-opus-5-5, retried by claude-opus-5-5, finished by claude-opus-5-5. Names are provisional.
// Can a unit's footprint stand on the map cell `cell`? The guards are the map
// bounds, then the line-of-sight tests (the player's bit in the shared
// visibility mask, then either the explored byte map or the mask again,
// depending on flag 2 of g_game+0x14281), then a walk of the footprint cells
// that accumulates the build cost into DAT_0051e688 and the height envelope
// into the returned DAT_0051e684.
//
// <stdlib.h>, <string.h> and <math.h> stay: their symbol ids decide operand
// orders in the cell multiply, the bounds guard and the explored arm.
#include <stdlib.h>
#include <string.h>
#include <math.h>

#pragma pack(push, 1)

union Fix_0047d2e0 {
    int v;
    struct { short lo; short hi; } p;
};

struct Point {

    short x;
    short y;
};

struct Cell_0047d2e0 {
    short field_0;
    char unknown_2[0x5 - 0x2];
    unsigned char field_5;
    unsigned char field_6;
    unsigned char field_7;
    short field_8;
    unsigned char field_a;
    unsigned char field_b;
    unsigned char field_c;
};

struct Unit_0047d2e0 {
    char unknown_0[0x14a];
    Point origin;
    unsigned char* mask;
    char unknown_152[0x1be - 0x152];
    short field_1be;
    short field_1c0;
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char field_228;
    char unknown_229[0x22c - 0x229];
    unsigned char field_22c;
};

struct MapSize_0047d2e0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};

struct ByteMap_0047d2e0 {
    unsigned char* data;
    MapSize_0047d2e0 size;
    int Contains(int x, int y) { return x < size.width && y < size.height; }
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Los_0047d2e0 {
    char unknown_0[0x7c];
    ByteMap_0047d2e0 explored;
};

struct Game {
    char unknown_0[0x2a43];
    unsigned char player;
    char unknown_2a44[0x14233 - 0x2a44];
    int width;
    int height;
    char unknown_1423b[0x14253 - 0x1423b];
    int field_14253;
    char unknown_14257[0x1426f - 0x14257];
    unsigned char* field_1426f;
    unsigned short* field_14273;
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;
    char unknown_14280[0x14281 - 0x14280];
    unsigned char losFlags;
    char unknown_14282[0x14287 - 0x14282];
    Cell_0047d2e0* cells;
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e684;
extern int DAT_0051e688;

int __stdcall GetCellHeight(Point* p);

struct Pos_0047d2e0 {
    Fix_0047d2e0 x, y, z;
};

struct Position_0047d2e0 {              // 16.16 fixed point, only high words read
    short xFrac;
    short x;
    short yFrac;
    short y;
    short zFrac;
    short z;
};

// The helpers read the position through the six-short Position cast.
static inline int IsExplored_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    if (los->explored.size.Contains(tx, ty) && los->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt, unsigned int bit)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    // Declared after tx/ty: makes the multiply width-first.
    unsigned int w = los->explored.size.width;
    int r;
    if (!los->explored.size.Contains(tx, ty))
        r = 0;
    else
        r = (g_game->field_14273[w * ty + tx] & bit) != 0;
    return r;
}

// Takes the caller's x but derives ty and the width itself: no spelling in
// the caller gives the original's width-first multiply.
static inline unsigned short VisWord_0047d2e0(Los_0047d2e0* los, int tx,
    Position_0047d2e0* pos, Fix_0047d2e0* hgt)
{
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    unsigned int w = los->explored.size.width;
    return g_game->field_14273[w * ty + tx];
}

// Blocked_ and Terrain_ stay early-return helpers.
static int Blocked_0047d2e0(Cell_0047d2e0* c)
{
    unsigned short v = c->field_8;
    if (v == 0xffff)
        return 0;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 1;
        return (g_game->field_1426f[v * 0x100 + 0xfe] >> 6) & 1;
    }
    if (v != 0xfffe)
        return 1;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return (g_game->field_1426f[v2 * 0x100 + 0xfe] >> 6) & 1;
}

static unsigned char* Terrain_0047d2e0(
Cell_0047d2e0* c)
{
    if (c == 0)
        return 0;
    unsigned short v = c->field_8;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 0;
        return g_game->field_1426f + v * 0x100;
    }
    if (v != 0xfffe)
        return 0;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return g_game->field_1426f + v2 * 0x100;
}

// FUNCTION: 0x47d2e0
int __stdcall CanBuildAt(Unit_0047d2e0* unit, Point cell, short type, Los_0047d2e0* los)
{
    int ok;
    DAT_0051e684 = 0;
    DAT_0051e688 = 0;
    Point origin = unit->origin;
    // The guard reads cell.x and cell.y directly.
    if (cell.x < 1 || cell.y < 1 || cell.x + origin.x >= g_game->width ||
        cell.y + origin.y >= g_game->height)
        return 0;
    int cols = origin.x;
    int x;
    int y;
    ok = 1;
    if (los != 0) {
        Pos_0047d2e0 pos;
        // A separate Fix local: it shares the dead los argument slot with the bit spill.
        Fix_0047d2e0 hgt;
        pos.x.v = (origin.x + cell.x * 2) << 19;
        pos.z.v = (origin.y + cell.y * 2) << 19;
        hgt.v = GetCellHeight(&cell) << 16;
        x = pos.x.p.hi >> 5;
        y = (pos.z.p.hi - (hgt.p.hi >> 1)) >> 5;
        if (!los->explored.size.Contains(x, y))
            return 0;
        // Computed after the first Contains test.
        unsigned int bit = 1 << g_game->player;
        if ((VisWord_0047d2e0(los, x, (Position_0047d2e0*)&pos, &hgt) & bit) == 0)
            return 0;
        if ((g_game->losFlags & 2) == 2)
            ok = IsExplored_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt);
        else
            ok = IsSeen_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt, bit);
    }
    // The cell pointer is computed before the min6/max5 initialisers.
    Cell_0047d2e0* c = &g_game->cells[cell.y * g_game->width + cell.x];
    unsigned char min6 = 0xff;
    unsigned char max5 = 0;
    unsigned char max5b = 0;
    int found80 = 0;
    int foundFE20 = 0;
    int index = 0;
    int row;
    int col;
    row = 0;
    if (origin.y > row) {
        // Rotated do/while with a positive bottom test.
        do {
            for (col = 0; col < cols; col++) {
                DAT_0051e688 += c->field_7;
                int m = unit->mask[index++];
                if (m & 8) {
                    if (c->field_6 < min6)
                        min6 = c->field_6;
                    if (c->field_5 > max5)
                        max5 = c->field_5;
                }
                if ((m & 0x10) && c->field_5 > max5b)
                    max5b = c->field_5;
                if ((m & 1) && (c->field_c & 2) && ok)
                    return 0;
                if ((m & 6) && c->field_0 != 0 && c->field_0 != type && ok)
                    return 0;
                if (m & 0x20) {
                    if (Blocked_0047d2e0(c) != 0)
                        return 0;
                }
                if (m & 0x40) {
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xff] & 2))
                        return 0;
                }
                if (m & 0x80) {
                    found80 = 1;
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xfe] & 0x20))
                        foundFE20 = 1;
                }
                ++c;
            }
            row++;
            c = (g_game->width - ((int)cols)) + c;
        } while (row < origin.y);
    }
    if (found80 && !foundFE20)
        return 0;
    unsigned char r;
    if (max5 < min6) {
        r = g_game->seaLevel - unit->field_22c;
    } else {
        if (max5 - min6 > unit->field_228)
            return 0;
        r = min6;
    }
    if (max5b > r)
        return 0;
    if (min6 < g_game->seaLevel - unit->field_1be)
        return 0;
    if ((max5 > max5b ? max5 : max5b) > g_game->seaLevel - unit->field_1c0)
        return 0;
    DAT_0051e684 = r;
    return 1;
}
