// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6. Names are
// provisional. PARTIAL 59.8%, 608 of 613 bytes. Cache edge start/end heights before testing
// descent, keep the row countdown separate from the divisor, and share the height locals across
// both edge walks. The scan uses a guarded do-while with explicit index and point increments. This
// improves the previous 41.0% and reproduces the original 0x14024-byte stack allocation. Remaining
// differences include register allocation, extremum-index slots, and the count argument kept in a
// register rather than its original slot. 768 header sets, count representations and paired
// extremum indices did not improve the saved version. Count <= 0 still reaches uninitialized
// extremum indices, as does the original (0x4c08b7 and 0x4c0962).

struct Point_004c0820 {
    int x;
    int y;
    int z;
};

struct Span_004c0a90 {
    int x1; // +0x0
    int x2; // +0x4
    char unknown_8[0x18 - 0x8];
    int z1; // +0x18 (16.16)
    int z2; // +0x1c (16.16)
    char unknown_20[0x28 - 0x20];
};

struct Surface_004c0a90 {
    unsigned short pitch; // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;  // +0x10
    unsigned char* depth; // +0x14
};

void __stdcall FUN_004c0a90(int row, Span_004c0a90* span, Surface_004c0a90* surf,
                            unsigned char color);

// FUNCTION: 0x4c0820
int __stdcall FUN_004c0820(Surface_004c0a90* surf, Point_004c0820* pts, int count,
                           unsigned char color) {
    Span_004c0a90 spans[2048];
    Span_004c0a90* out;
    Point_004c0820* a;
    Point_004c0820* b;
    int ay;
    int y0, y1;
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    int iymin, iymax;
    if (count > 0) {
        Point_004c0820* p = pts;
        int i = 0;
        do {
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
            i++;
            p++;
        } while (i < count);
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
            a = &pts[i];
            b = &pts[j];
            y0 = a->y;
            y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                int x = a->x;
                int dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = a->z << 16;
                int dy = ((b->z << 16) - y) / h;
                int rows = b->y - y0;
                do {
                    out->x1 = x >> 16;
                    out->z1 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--rows);
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
            a = &pts[i];
            b = &pts[j];
            y0 = a->y;
            y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                int x = a->x;
                int dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = a->z << 16;
                int dy = ((b->z << 16) - y) / h;
                int rows = b->y - y0;
                do {
                    out->x2 = x >> 16;
                    out->z2 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--rows);
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
