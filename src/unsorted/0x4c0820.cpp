// Decompiled by space-bunny-free. Names are provisional.
// Fills a closed polygon: scans the vertices for the row range, rasterises
// every edge that runs downwards from the topmost vertex into per row spans
// (x1/z1 for the backward walk, x2/z2 for the forward walk), then plots the
// two end points of each span with FUN_004c0a90.
//
// Not matched: 35.9%, 556 bytes against 613. The body is instruction for
// instruction the original everywhere except the allocation of the four
// register variables, and that one difference is the whole gap:
//   * the original's frame is 0x14024 with the span array at [esp+0x34], ours
//     is 0x1401c with it at [esp+0x2c]. The original has two more scalar
//     slots: xmin in memory (0x10) and a spill of the second vertex pointer
//     (0x20). Every displacement in the two rasteriser loops is therefore
//     8 bytes out, which is most of the lost bytes.
//   * cause, in the scan loop: the original holds ymin in esi, xmax in edi
//     and pts in ebx, and keeps count and xmin in memory (count is re-read
//     from its argument slot on every use, including in the rasterisers);
//     ours holds pts in esi, xmax in edi, xmin in ebx and count in ebp, and
//     ymin only in memory. Same six values, different order of preference.
//     In the rasteriser loops everything is then one register lower than the
//     original (a/&b, the divisor and the running x are demoted), which is
//     what forces the extra &b spill.
// Tried and rejected: <windows.h> (no change at all, 35.9%); declaring and
// testing xmin before xmax in the scan loop (34.9%, worse).
// Untested leads: the number of references ymin and count have in the
// original is the same as here, so the difference is probably in how the
// original spells the count - 1 wrap in the rasterisers (maybe a modulo, or
// a separate index local), which would stop count being live across the
// whole function.
// Suspected original bug: the scan loop only writes iymin and iymax when it
// runs, and 0x4c089a reads iymax and 0x4c08af reads iymin without any
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
    if (ymax != ymin) {
        int i = iymin;
        out = spans;
        do {
            int j = i - 1;
            if (j < 0)
                j = count - 1;
            if (pts[i].y < pts[j].y) {
                Point_004c0820* a = &pts[i];
                Point_004c0820* b = &pts[j];
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
        i = iymin;
        out = spans;
        do {
            int j = i + 1;
            if (j >= count)
                j = 0;
            if (pts[i].y < pts[j].y) {
                Point_004c0820* a = &pts[i];
                Point_004c0820* b = &pts[j];
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
        {
            int y = ymin;
            Span_004c0a90* s = spans;
            while (y < ymax) {
                if (s->x2 > s->x1)
                    FUN_004c0a90(y, s, surf, color);
                s++;
                y++;
            }
        }
        return 1;
    }
    return 0;
}
