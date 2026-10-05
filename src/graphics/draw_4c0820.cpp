// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by DeepSeek V4.1 Flash, checked by GPT-6., retried by Claude Opus 5.5, finished by GPT-6, matched by claude-opus-5-5. Names are provisional.
// MATCH (#5387), from 99.5%. The last difference was the order of the two
// reloads after walk1's fill loop (original: the `i - 1` temporary, then pts).
// They come from C2 splitting both values when walk1's ebp temporary is
// coloured; the one split later is reloaded first, and a step's splits go in
// candidate id order. The `i - 1` temporary is #49, so pts's piece has to get
// a lower id. Pieces take ids from C2's LIFO of freed ids, and the candidates
// dropped before colouring go onto it in reverse code order, so the earliest
// drops sit on top: here the scan's `p++` constant, then walk1's two address
// temporaries (ids 58 and 57). pts's piece is the third piece made at walk2's
// split (after the zero constant's and count's), so it took 57.
// Reading `p->y` and `p->x` into the block locals `py` and `px` for the first
// test of each pair (and its store), while the second test re-reads the field,
// leaves two named copies of the scan's CSE temporaries that C2 drops before
// colouring. They are dropped early in code order, so pts's piece gets a
// named local's id and the reloads come out in the original's order.
// Using py and px in both tests of a pair makes them the CSE values
// themselves (no drop, 99.5%); naming only one of them is not enough.
// The `int color` parameter (not `unsigned char`) is still needed for the
// final call's ebx/ebp pair (98.0% with a byte).
// Diagnosed with tools/c2prio.py --json plus a breakpoint on C2's candidate
// free (0x40ed2b in FUN_0040ecd2) to see the freed-id order.
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
