// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash: function-scope a/b/ay and the sibling 0x4c0c70 do-while
// scan form were tried; they do not move the frame (still 0x1401c against
// 0x14024) or the ymin/xmin register choice, so this is left at 41.0%.
// Fills a closed polygon: scans the vertices for the row range, then walks
// backwards from the topmost vertex rasterising every edge that runs
// downwards into per row spans (x1/z1), then forwards doing the same for the
// other side (x2/z2), and finally plots the two end points of each span with
// FUN_004c0a90.
//
// Not matched: 40.9% (554 bytes against 613). The instruction sequence of
// both rasteriser bodies is right, but the whole function is allocated one
// register out of step, which moves every stack displacement with it:
//   * frame 0x14024 against our 0x1401c: the original has two more scalar
//     slots, the second vertex pointer b (+0x20) and a copy of a->y (+0x28,
//     and +0x24 in the second loop). Ours has neither, so the spans array
//     sits at [esp+0x2c] instead of [esp+0x34] and every argument slot is
//     4 bytes lower (0x14034 for pts against 0x1403c).
//   * the original keeps count in its argument slot and re-reads it on every
//     use (0x4c088b, 0x4c08c8, 0x4c095a, 0x4c0981, 0x4c0a1c); ours copies it
//     into ebp. The original's scan loop then has esi=ymin, edi=xmax,
//     ebx=pts, ebp unused, while ours has esi=pts, edi=xmax, ebx=xmin,
//     ebp=count. Same six values, different preference order, and the
//     rasteriser loops are all one register lower as a knock-on effect
//     (the running x is in ebp, not ebx, so the second vertex pointer takes
//     edi instead of a stack slot).
//   * the original computes the row count a second time, as
//     sub esi, [esp+0x28] (b->y minus a saved a->y), so the divisor
//     (edi) and the countdown (esi) are two distinct values. Any spelling
//     that keeps them as one expression makes MSVC 5 CSE them into a single
//     register.
// Tried and rejected, all worse or unchanged: <windows.h> (35.9%, no change,
// from the earlier attempt on this file); declaring a and b before the
// pts[i].y < pts[j].y test (38.2%); a local int h for the divisor (unchanged,
// still CSEd); a local int ay = a->y used by the countdown (40.9% but the
// body spills badly, 617 bytes); spelling the countdown b->y - pts[i].y so it
// cannot be CSEd with the divisor (28.4%, 615 bytes).
// Suspected original bug: the scan loop only writes iymin and iymax when it
// runs, and 0x4c089a reads iymax and 0x4c08b7 reads iymin without any
// initialisation, so a caller passing count <= 0 rasterises with two
// uninitialised stack words (0x4c0865, 0x4c0875 versus 0x4c0962, 0x4c08b7).

struct Point_004c0820 {
    int x;
    int y;
    int z;
};

struct Span_004c0a90 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
    char unknown_20[0x28 - 0x20];
};

struct Surface_004c0a90 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

void __stdcall FUN_004c0a90(int row, Span_004c0a90* span, Surface_004c0a90* surf, unsigned char color);

// FUNCTION: 0x4c0820
int __stdcall FUN_004c0820(Surface_004c0a90* surf, Point_004c0820* pts, int count, unsigned char color)
{
    Span_004c0a90 spans[2048];
    Span_004c0a90* out;
    Point_004c0820* a;
    Point_004c0820* b;
    int ay;
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    int iymin, iymax;
    if (count > 0) {
        Point_004c0820* p = pts;
        for (int i = 0; i < count; i++, p++) {
            if (p->y < ymin) {
                ymin = p->y;
                iymin = i;
            }
            if (p->y > ymax) {
                ymax = p->y;
                iymax = i;
            }
            if (p->x > xmax)
                xmax = p->x;
            if (p->x < xmin)
                xmin = p->x;
        }
    }
    if (ymax == ymin)
        return 0;
    {
        int i = iymin;
        out = spans;
        do {
            int j = i - 1;
            if (j < 0)
                j = count - 1;
            if (pts[i].y < pts[j].y) {
                a = &pts[i];
                b = &pts[j];
                int dx = ((b->x - a->x) << 16) / (b->y - a->y);
                int x = (a->x << 16) + 0xffff;
                int y = a->z << 16;
                int dy = ((b->z << 16) - y) / (b->y - a->y);
                int k = b->y - a->y;
                do {
                    out->x1 = x >> 16;
                    out->z1 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--k);
            }
            i = i - 1;
            if (i < 0)
                i = count - 1;
        } while (i != iymax);
    }
    {
        int i = iymin;
        out = spans;
        do {
            int j = i + 1;
            if (j >= count)
                j = 0;
            if (pts[i].y < pts[j].y) {
                a = &pts[i];
                b = &pts[j];
                int dx = ((b->x - a->x) << 16) / (b->y - a->y);
                int x = (a->x << 16) + 0xffff;
                int y = a->z << 16;
                int dy = ((b->z << 16) - y) / (b->y - a->y);
                int k = b->y - a->y;
                do {
                    out->x2 = x >> 16;
                    out->z2 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--k);
            }
            i = i + 1;
            if (i >= count)
                i = 0;
        } while (i != iymax);
    }
    {
        int y = ymin;
        Span_004c0a90* s = spans;
        while (y < ymax) {
            if (s->x2 - s->x1 > 0)
                FUN_004c0a90(y, s, surf, color);
            s++;
            y++;
        }
    }
    return 1;
}
