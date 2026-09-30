// Decompiled by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
// PARTIAL 94.5%, 897 bytes. Share y0, y1, x, dx and dz across the two
// edge walks, but retain block-local z/shade values. Declare bounds in the
// order maxX, minY, maxY, minX. Both together restore all original local
// slots and edge-loop instructions. Give the scan its own index initialized
// before the positive-count guard, and increment it before the point cursor.
// Remaining differences are prologue scheduling and clipping-register choices.
// Sharing every temporary instead is worse. Two 768-header sweeps and
// surface getters/clamping helpers did not resolve the remaining differences.
// GPT-6.1-sol refinement: reference aliases, a height accessor, changing the
// maxRow declaration point, and moving the surface alias after the scan all
// kept 94.5%. The root pass tried the equivalent pitch-bound form; it scored 86.6%, so the original source shape is restored below.
// deepseek-v4.1-flash: all 5 residual hunks are one eax/ecx rotation. The
// original keeps the surface pointer in ecx (live across the pitch and height
// reads) and uses eax for maxY and maxRow; ours swaps the two. No change from:
// dropping/keeping the sf alias, unsigned short* views, thiscall Width()/Height()
// accessors, merged || condition, reference parameter, swapped or negated pitch
// comparisons, &spans[0] and cast forms for the edge-walk cursor lea, reordering
// locals, dummy prefix functions, or headers.py (128 sets, all 94.5%).

struct Span_004c0c70 {
    int x1; // +0x0
    int x2; // +0x4
    char unknown_8[0x18 - 0x8];
    int z1; // +0x18 (16.16)
    int z2; // +0x1c (16.16)
    int s1; // +0x20 (16.16)
    int s2; // +0x24 (16.16)
};

struct Point_004c0c70 {
    int x;
    int y;
    int z;
    int s;
};

struct Surface_004c0c70 {
    unsigned short pitch;  // +0x0
    unsigned short height; // +0x2
    char unknown_4[0x10 - 0x4];
    unsigned char* bits;  // +0x10
    unsigned char* depth; // +0x14
};

void __stdcall FUN_004c0b10(int row, Span_004c0c70* span, Surface_004c0c70* surf,
                            unsigned char color);

// FUNCTION: 0x4c0c70
int __stdcall FUN_004c0c70(Surface_004c0c70* surf, Point_004c0c70* pts, int n,
                           unsigned char color) {
    int y0, y1, x, dx, dz;
    Span_004c0c70 spans[2048];
    int maxX = -999999;
    int minY = 999999;
    int maxY = -999999;
    int minX = 999999;
    int minYi;
    int maxYi;
    int i;
    Surface_004c0c70* sf = surf;
    int scanIndex = 0;
    if (n > 0) {
        Point_004c0c70* p = pts;
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
            y0 = p->y;
            y1 = q->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                x = p->x;
                dx = ((q->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = p->z << 16;
                int s = p->s << 16;
                dz = ((q->z << 16) - z) / dy;
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
            y0 = p->y;
            y1 = q->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                x = p->x;
                dx = ((q->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = p->z << 16;
                int s = p->s << 16;
                dz = ((q->z << 16) - z) / dy;
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
