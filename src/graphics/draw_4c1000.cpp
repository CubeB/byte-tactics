// Decompiled by space-bunny-alpha, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Scanline filler, the sibling of 0x4c0c70 (which also fills a shade channel):
// it finds the extreme y vertices, then walks the vertex ring from the top one
// backwards to the bottom one filling the left end of each row, walks it
// forwards for the right end, and hands every row to FillFlatSpan. The 2048
// entry span array is what puts the frame at 0x14028.
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

void __stdcall FillFlatSpan(int row, Span_004c1000* span, Surface_004c1000* surf, unsigned char color);

// FUNCTION: 0x4c1000
int __stdcall FillFlatPolygon(Surface_004c1000* surf, Point_004c1000* pts, int n, int color)
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
    // One function-scope j shared by both walks; each walk has its own block-scoped i.
    int j;
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
    // Height tested inline here and reused for maxRow: the two loads must CSE.
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
        {
            int i = minYi;
            for (;;) {
                j = i - 1;
                if (j < 0)
                    j = n - 1;
                // pts[i] and pts[j] indexed directly: named pointers would re-pack the frame.
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
        }
        sp = spans;
        {
            int i = minYi;
            for (;;) {
                j = i + 1;
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
    }
    {
        Span_004c1000* sp = spans;
        for (i = minY; i < maxY; i++) {
            if (sp->x2 - sp->x1 > 0)
                FillFlatSpan(i, sp, surf, color);
            sp++;
        }
    }
    return 1;
}
