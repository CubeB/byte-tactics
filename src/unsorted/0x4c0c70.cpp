// Decompiled by space-bunny-free. Names are provisional.
//
// Polygon scanline filler: 79.4 percent, exact size (897 bytes).
//
// It finds the bounding row of the polygon, walks the two chains that join
// its topmost point to its bottommost point (forwards and backwards through
// the 16-byte x/y/z/shade points), and for every edge that descends writes
// one 40-byte Span per scanline into a 2048-entry local array, holding the
// running 16.16 x, depth and shade. The first chain writes the span's left
// end (x1/z1/s1), the second the right end (x2/z2/s2), and the final loop
// hands every non-empty span to 0x4c0b10. The local array is what makes the
// prologue a 0x1402c alloca probe: 0x2c bytes of fixed locals plus
// 2048 * 0x28.
//
// PARTIAL, 79.4 percent. Both edge loops and their inner emit loops are now
// instruction for instruction identical to the original; what is left is the
// frame slot assignment, which the guide's declaration-order rule has to be
// coaxed into, plus the two things noted below.
//
// What moved it: the first point loop must be a `do { } while (++i < n)` under
// an `if (n > 0)`, not a `for`. The `for` form tests the bound twice (once for
// the guard, once for the header) and that alone cost 10 points.
//
// Known remaining differences:
//  1. Slot numbers. The original's frame is end 0x10, sp/minX 0x14, maxY 0x18,
//     minY 0x1c, dx 0x20, x 0x24, dz 0x28, j 0x2c, minYi 0x30, maxRow 0x34,
//     maxYi 0x38, and the first point loop's index also lands in 0x10. Every
//     diff outside the prologue and the three early returns is a mismatch of
//     these numbers alone, not of code. Note minX and the span pointer share
//     0x14: they are never live at the same time.
//  2. The `mov edx, <pts>` load sits before the pushes instead of between
//     `push ebx` and `push ebp`. A scheduler tie-break, per the guide's entry
//     on loads drifting around pushes.

struct Span_004c0c70 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
    int s1;                            // +0x20 (16.16)
    int s2;                            // +0x24 (16.16)
};

struct Point_004c0c70 {
    int x;
    int y;
    int z;
    int s;
};

struct Surface_004c0c70 {
    unsigned short pitch;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[0x10 - 0x4];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

void __stdcall FUN_004c0b10(int row, Span_004c0c70* span, Surface_004c0c70* surf, unsigned char color);

// FUNCTION: 0x4c0c70
int __stdcall FUN_004c0c70(Surface_004c0c70* surf, Point_004c0c70* pts, int n, unsigned char color)
{
    Span_004c0c70 spans[2048];
    int minX = 999999;
    int maxX = -999999;
    int minY = 999999;
    int maxY = -999999;
    int minYi;
    int maxYi;
    int i;
    Surface_004c0c70* sf = surf;
    if (n > 0) {
        Point_004c0c70* p = pts;
        i = 0;
        do {
            if (p->y < minY) {
                minY = p->y;
                minYi = i;
            }
            if (p->y > maxY) {
                maxY = p->y;
                maxYi = i;
            }
            if (p->x > maxX)
                maxX = p->x;
            if (p->x < minX)
                minX = p->x;
            p++;
        } while (++i < n);
    }
    if (minX > (int)sf->pitch - 1)
        return 0;
    if (maxY < 0)
        return 0;
    int maxRow = (int)sf->height - 1;
    if (minY > maxRow)
        return 0;
    if (minY < 0)
        minY = 0;
    if (maxY > maxRow)
        maxY = maxRow;
    if (maxY == minY)
        return 0;
    {
        Span_004c0c70* sp = spans;
        i = minYi;
        for (;;) {
            int j = i - 1;
            if (j < 0)
                j = n - 1;
            Point_004c0c70* p = &pts[i];
            Point_004c0c70* q = &pts[j];
            int y0 = p->y;
            int y1 = q->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                int x = p->x;
                int dx = ((q->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = p->z << 16;
                int s = p->s << 16;
                int dz = ((q->z << 16) - z) / dy;
                int ds = ((q->s << 16) - s) / dy;
                if (y0 < 0) {
                    x -= dx * y0;
                    z -= dz * y0;
                    s -= ds * y0;
                    y0 = 0;
                }
                if (y1 > maxRow)
                    y1 = maxRow;
                if (y0 < y1) {
                    int count = y1 - y0;
                    do {
                        sp->x1 = x >> 16;
                        x += dx;
                        sp->z1 = z;
                        sp->s1 = s;
                        z += dz;
                        s += ds;
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
            Point_004c0c70* p = &pts[i];
            Point_004c0c70* q = &pts[j];
            int y0 = p->y;
            int y1 = q->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                int x = p->x;
                int dx = ((q->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = p->z << 16;
                int s = p->s << 16;
                int dz = ((q->z << 16) - z) / dy;
                int ds = ((q->s << 16) - s) / dy;
                if (y0 < 0) {
                    x -= dx * y0;
                    z -= dz * y0;
                    s -= ds * y0;
                    y0 = 0;
                }
                if (y1 > maxRow)
                    y1 = maxRow;
                if (y0 < y1) {
                    int count = y1 - y0;
                    do {
                        sp->x2 = x >> 16;
                        x += dx;
                        sp->z2 = z;
                        sp->s2 = s;
                        z += dz;
                        s += ds;
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
        Span_004c0c70* sp = spans;
        for (i = minY; i < maxY; i++) {
            if (sp->x2 - sp->x1 > 0)
                FUN_004c0b10(i, sp, surf, color);
            sp++;
        }
    }
    return 1;
}
