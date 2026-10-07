// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by DeepSeek V4.1 Flash, checked by GPT-6., retried by Claude Opus 5.5, finished by GPT-6, matched by claude-opus-5-5. Names are provisional.
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

void __stdcall PlotSpanEnds(int row, Span_004c0a90* span, Surface_004c0a90* surf,
                            unsigned char color);

// FUNCTION: 0x4c0820
int __stdcall DrawPolygonEdges(Surface_004c0a90* surf, Point_004c0820* pts, int count,
                           int color) {
    // color stays int, not unsigned char: the final call's register pairing.
    Span_004c0a90 spans[2048];
    Span_004c0a90* out;
    Point_004c0820* b;
    int x, dx;
    int ymin = 999999;
    int xmax = -999999;
    int ymax = -999999;
    int xmin = 999999;
    int iymin, iymax;
    int y;
    Span_004c0a90* s;
    int i;
    Point_004c0820* p;
    i = 0;
    if (count > 0) {
        p = pts;
        do {
            // Only the first test of each pair uses py/px; the second re-reads the field.
            int py = p->y;
            if (py < ymin) {
                ymin = py;
                iymin = i;
            }
            if (p->y > ymax) {
                ymax = p->y;
                iymax = i;
            }
            int px = p->x;
            if (px > xmax)
                xmax = px;
            if (p->x < xmin)
                xmin = p->x;
            i++;
            p++;
        } while (i < count);
    }
    if (ymax == ymin)
        return 0;
    {
        out = spans;
        int i = iymin;
        do {
            int j = i - 1;
            if (j < 0) {
                j = count;
                j--;
            }
            int y0 = pts[i].y;
            b = &pts[j];
            int y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
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
            if (i < 0) {
                i = count;
                i--;
            }
        } while (i != iymax);
    }
    {
        int i = iymin;
        out = spans;
        do {
            int j = i + 1;
            if (j >= count)
                j = 0;
            int y0 = pts[i].y;
            b = &pts[j];
            int y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
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
        s = spans;
        y = ymin;
        while (y < ymax) {
            if (s->x2 - s->x1 > 0)
                PlotSpanEnds(y, s, surf, color);
            s++;
            y++;
        }
    }
    return 1;
}
