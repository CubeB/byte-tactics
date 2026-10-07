// Decompiled by Claude Opus 5.5. Names are provisional.
// A map cell drawn as contour lines: 0x4181d0 splits the cell into four
// triangles around its centre and 0x417f60 (the function just before it in
// the original file) draws the contour lines that cross each triangle.
// Unused headers, kept with 0x417f60 defined first: the operand order of the
// products and sums depends on them.
#include <stdio.h>
#include <string.h>

struct Game {
    char unknown_0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
};

// A screen position; the third coordinate (height) travels separately.
struct Point_00417f60 {
    int x;
    int y;
};

extern Game* g_game;
extern int DAT_00511dd0;               // contour spacing
extern int DAT_00511dd4;               // contour offset
extern unsigned char DAT_00501d18[];   // colour by height band

void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);

// Draws the contour lines that cross one triangle of the height map. Each
// corner is a screen point and a height (8 fraction bits). The corners are
// sorted by height, then every contour level between the lowest and highest
// corner (the multiples of DAT_00511dd0, offset by DAT_00511dd4) is drawn as
// one line across the triangle: first where it crosses the long edge 1-3 and
// the upper edge 2-3, then the long edge and the lower edge 1-2. The colour
// comes from the level's height above sea level through DAT_00501d18.
// FUNCTION: 0x417f60
void __stdcall FUN_00417f60(void* surface, Point_00417f60 p1, int z1,
                            Point_00417f60 p2, int z2,
                            Point_00417f60 p3, int z3)
{
    int tz;
    Point_00417f60 tp;
    if (z2 > z3) {
        tz = z2; z2 = z3; z3 = tz;
        tp = p2; p2 = p3; p3 = tp;
    }
    if (z1 > z2) {
        tz = z1; z1 = z2; z2 = tz;
        tp = p1; p1 = p2; p2 = tp;
    }
    if (z2 > z3) {
        tz = z2; z2 = z3; z3 = tz;
        tp = p2; p2 = p3; p3 = tp;
    }
    int level = z3 / DAT_00511dd0 * DAT_00511dd0 + DAT_00511dd4;
    while (level > z3) {
        level -= DAT_00511dd0;
    }
    if (level > z2) {
        int dz13 = z3 - z1;
        int dz23 = z3 - z2;
        do {
            int h13 = level - z1;
            int h23 = level - z2;
            DrawLine(surface,
                         (p1.x * (dz13 - h13) + h13 * p3.x) / dz13,
                         (p1.y * (dz13 - h13) + h13 * p3.y) / dz13,
                         (p2.x * (dz23 - h23) + h23 * p3.x) / dz23,
                         (p2.y * (dz23 - h23) + h23 * p3.y) / dz23,
                         DAT_00501d18[((level >> 8) - g_game->seaLevel + 0x100) >> 4]);
            level -= DAT_00511dd0;
        } while (level > z2);
    }
    if (level > z1) {
        int dz13 = z3 - z1;
        int dz12 = z2 - z1;
        do {
            int h13 = level - z1;
            // Separate local from h13: a shared one computes both weights through one neg.
            int h12 = level - z1;
            DrawLine(surface,
                         (p1.x * (dz13 - h13) + h13 * p3.x) / dz13,
                         (p1.y * (dz13 - h13) + h13 * p3.y) / dz13,
                         (p1.x * (dz12 - h12) + h12 * p2.x) / dz12,
                         (p1.y * (dz12 - h12) + h12 * p2.y) / dz12,
                         DAT_00501d18[((level >> 8) - g_game->seaLevel + 0x100) >> 4]);
            level -= DAT_00511dd0;
        } while (level > z1);
    }
}

// Draws one map cell: four triangles fanning from the cell's centre, whose
// position and height are the rounded averages of the four corners. The
// corner heights are bytes, scaled to 8 fraction bits.
// FUNCTION: 0x4181d0
void __stdcall FUN_004181d0(void* surface, Point_00417f60* corners, unsigned char* heights)
{
    Point_00417f60 mid;
    mid.x = (corners[0].x + corners[1].x + corners[2].x + corners[3].x + 2) / 4;
    mid.y = (corners[0].y + corners[1].y + corners[2].y + corners[3].y + 2) / 4;
    int midHeight = (heights[0] + heights[1] + heights[2] + heights[3]) * 64;
    // "* 256", not "<< 8": gives mov cl,[mem]; shl ecx,8 instead of mov ch.
    FUN_00417f60(surface, corners[0], heights[0] * 256, corners[1], heights[1] * 256, mid, midHeight);
    FUN_00417f60(surface, corners[1], heights[1] * 256, corners[2], heights[2] * 256, mid, midHeight);
    FUN_00417f60(surface, corners[2], heights[2] * 256, corners[3], heights[3] * 256, mid, midHeight);
    FUN_00417f60(surface, corners[3], heights[3] * 256, corners[0], heights[0] * 256, mid, midHeight);
}
