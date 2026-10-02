// Decompiled by space-bunny-alpha. Names are provisional.
// PARTIAL 97.1%, 791 of 791 bytes (same size, every instruction the compiler
// chose is the original's, in the original's order).
// Scanline filler, the sibling of 0x4c0c70 (which also fills a shade channel):
// it finds the extreme y vertices, then walks the vertex ring from the top one
// backwards to the bottom one filling the left end of each row, walks it
// forwards for the right end, and hands every row to FUN_004c06e0. The 2048
// entry span array is what puts the frame at 0x14028.
//
// Two shape choices carry most of the score:
// - Testing the height inline in the minY guard and then assigning maxRow from
//   the same expression, `if (minY > (int)sf->height - 1) return 0;` followed
//   by `int maxRow = (int)sf->height - 1;`, lets MSVC CSE the two loads. With
//   maxRow assigned first it moves surf into ecx and every guard register with
//   it (74.5% to 87.7%).
// - Indexing pts[i]/pts[j] straight into the reads, with no `Point *p, *q`
//   locals for the two walks. Naming them at function scope homes `p`, which
//   costs two frame slots and the two spill slots of both walk heads (97.1% to
//   92.8%); naming them inside each walk changes the walk head's addressing
//   into the pointer-walking form 0x4c0c70 uses instead (87.7%).
//
// What still differs: 16 instructions, all stack-slot numbers, and only in the
// two walk heads. Both walks spill the two vertex addresses after the y0 < y1
// test; the original gives the reused minX slot (frame + 4) to the second
// address (&pts[j]) and a fresh temporary to the first, ours hands that reused
// slot to &pts[i] instead and the temporary to &pts[j]. Everything else in the
// frame agrees: maxY +0, minX +4, minY +8, the dx spill +0xc, sp +0x10, the j
// temporary +0x14, minYi +0x1c, maxYi +0x20, maxRow +0x24.
//
// Tried and all inert at 97.1% (identical bytes): every permutation of the
// bound declarations and of `int y0, y1, x, dx, dz;` (MSVC assigns these slots
// by something other than declaration order here), maxRow declared with the
// bounds or last, sp and j at function scope, the fill temporaries dy/z/count
// at function scope, y0/y1 or x or dx block-local (each worse), dummy and
// self-assigning locals, initialisers on i/scanIndex/x/maxRow, comma
// expressions and fresh temporaries around the y and x reads, `pts + i`
// instead of `&pts[i]`, a const view of pts, inline accessors returning
// pts + i, an extra block around the fill, the clamps block-local, and
// swapping which pointer is read first. The 0x4c0820 lever (a function-scope
// pointer for the second edge point, which took that file from 94.1 to 98.0)
// drops this one to 92.8%, and naming the first point instead is inert.
// tools/permute.py then tried 8360 rewrites (move_decl, decl_scope, split_init,
// temp_intro, loop_form, flip_compare, include, ...) without finding one, and
// tools/headers.py's 128 sets are all 97.1%. The residue is an allocator
// tie-break over which spilled value gets the freed minX slot.
#include <string.h>

struct Span_004c1000 {
    int x1;                          // +0x0
    int x2;                          // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                          // +0x18, 16.16
    int z2;                          // +0x1c, 16.16
    int s1;                          // +0x20, 16.16
    int s2;                          // +0x24, 16.16
};

struct Point_004c1000 {
    int x;
    int y;
    int z;
};

struct Surface_004c1000 {
    unsigned short pitch;            // +0x0
    unsigned short height;           // +0x2
    char unknown_4[0x10 - 0x4];
    unsigned char* bits;             // +0x10
    unsigned char* depth;            // +0x14
};

void __stdcall FUN_004c06e0(int row, Span_004c1000* span, Surface_004c1000* surf, unsigned char color);

// FUNCTION: 0x4c1000
int __stdcall FUN_004c1000(Surface_004c1000* surf, Point_004c1000* pts, int n, int color)
{
    int y0, y1, x, dx, dz;
    Span_004c1000 spans[2048];
    int maxX = -999999;
    int minY = 999999;
    int maxY = -999999;
    int minX = 999999;
    int minYi;
    int maxYi;
    int i;
    Surface_004c1000* sf = surf;
    int scanIndex = 0;
    if (n > 0) {
        Point_004c1000* p = pts;
        do {
            if (p->y < minY) {
                minY = p->y;
                minYi = scanIndex;
            }
            if (p->y > maxY) {
                maxY = p->y;
                maxYi = scanIndex;
            }
            if (p->x > maxX)
                maxX = p->x;
            if (p->x < minX)
                minX = p->x;
            scanIndex++;
            p++;
        } while (scanIndex < n);
    }
    if (minX > (int)sf->pitch - 1)
        return 0;
    if (maxY < 0)
        return 0;
    if (minY > (int)sf->height - 1)
        return 0;
    int maxRow = (int)sf->height - 1;
    if (minY < 0)
        minY = 0;
    if (maxY > maxRow)
        maxY = maxRow;
    if (maxY == minY)
        return 0;
    {
        Span_004c1000* sp = spans;
        i = minYi;
        for (;;) {
            int j = i - 1;
            if (j < 0)
                j = n - 1;
            y0 = pts[i].y;
            y1 = pts[j].y;
            if (y0 < y1) {
                int dy = y1 - y0;
                x = pts[i].x;
                dx = ((pts[j].x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = pts[i].z << 16;
                dz = ((pts[j].z << 16) - z) / dy;
                if (y0 < 0) {
                    x -= dx * y0;
                    z -= dz * y0;
                    y0 = 0;
                }
                if (y1 > maxRow)
                    y1 = maxRow;
                if (y0 < y1) {
                    int count = y1 - y0;
                    do {
                        sp->x1 = x >> 16;
                        sp->z1 = z;
                        x += dx;
                        z += dz;
                        sp++;
                    } while (--count);
                }
            }
            i = i - 1;
            if (i < 0)
                i = n - 1;
            if (i == maxYi)
                break;
        }
        sp = spans;
        i = minYi;
        for (;;) {
            int j = i + 1;
            if (j >= n)
                j = 0;
            y0 = pts[i].y;
            y1 = pts[j].y;
            if (y0 < y1) {
                int dy = y1 - y0;
                x = pts[i].x;
                dx = ((pts[j].x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = pts[i].z << 16;
                dz = ((pts[j].z << 16) - z) / dy;
                if (y0 < 0) {
                    x -= dx * y0;
                    z -= dz * y0;
                    y0 = 0;
                }
                if (y1 > maxRow)
                    y1 = maxRow;
                if (y0 < y1) {
                    int count = y1 - y0;
                    do {
                        sp->x2 = x >> 16;
                        sp->z2 = z;
                        x += dx;
                        z += dz;
                        sp++;
                    } while (--count);
                }
            }
            i = i + 1;
            if (i >= n)
                i = 0;
            if (i == maxYi)
                break;
        }
    }
    {
        Span_004c1000* sp = spans;
        for (i = minY; i < maxY; i++) {
            if (sp->x2 - sp->x1 > 0)
                FUN_004c06e0(i, sp, surf, color);
            sp++;
        }
    }
    return 1;
}