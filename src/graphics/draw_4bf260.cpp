// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
//
// Draws a grid of connected lines into `surface`, or into the screen (locked
// with FUN_004c5e70, unlocked with FUN_004c5fa0) when `surface` is null:
// `rows` polylines, where counts[r] is the number of vertices of row r and
// the vertices follow one another in `points`. Every segment is clipped by
// FUN_004bea20 and drawn by FUN_004cc7ab. Like 0x4bf060 it returns the lock
// result on the screen path (a failed lock returns 0 without unlocking) and 1
// on the caller-surface path, and the locked loop carries an inlined copy of
// the single segment drawer (0x4be950) with its own null-surface lock, which
// stays because the compiler tests the address of the local `screen`.
//
// check.py prints MATCH (610 bytes). Three source shapes carry it:
// - the four clipped coordinates are declared in the order x0, y0, x1, y1 and
//   assigned in the order y1, x1, y0, x0, the same orders 0x4bf060 uses: that
//   pair picks which dead argument slot each one lands in (x0 at [esp+0x78],
//   x1 at 0x7c, y1 at 0x80, y0 at 0x84), which is the only way to get the
//   original's slot order out of the argument area. All 24 orders of each were
//   swept; nothing else scores above 85 percent.
// - `counts` is copied to a local `c` BEFORE `points` is copied to `p`, in both
//   branches. The original loads the three argument values in the order rows,
//   counts, points, so the counts pointer has to be live first: hoisting it
//   past the `rows > 0` test is what puts `mov ebx, [esp + 0x80]` above the
//   `jle` instead of below it. Worth 9 percent on its own.
// - the inner loop is a `while` with its own counter increment in the body
//   ahead of the pointer bump, not a `for`. The original's latch is
//   `inc edi` and then `add esi, 8`; a `for` header increment is emitted after
//   the body's last statement, so the two come out the wrong way round.

struct Surface_004bf260 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor
};

struct Point_004bf260 {
    int x;                             // +0x0
    int y;                             // +0x4
};

int __stdcall FUN_004c5e70(Surface_004bf260* out);
int __stdcall FUN_004c5fa0(Surface_004bf260* s);
int __stdcall FUN_004bea20(Surface_004bf260* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl FUN_004cc7ab(Surface_004bf260* dst, int x0, int y0, int x1, int y1, int color);

// FUNCTION: 0x4bf260
int __stdcall FUN_004bf260(Surface_004bf260* surface, Point_004bf260* points, int* counts,
                           int rows, int color)
{
    int result;
    if (surface == 0) {
        Surface_004bf260 screen;
        result = FUN_004c5e70(&screen);
        if (result != 0) {
            int r = rows;
            int* c = counts;
            Point_004bf260* p = points;
            while (r > 0) {
                int j = 0;
                while (j < *c - 1) {
                    int x0, y0, x1, y1;
                    y1 = p[1].y;
                    x1 = p[1].x;
                    y0 = p[0].y;
                    x0 = p[0].x;
                    if (&screen == 0) {
                        Surface_004bf260 inner;
                        if (FUN_004c5e70(&inner)) {
                            if (FUN_004bea20(&inner, &x0, &y0, &x1, &y1))
                                FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                            FUN_004c5fa0(&inner);
                        }
                    } else {
                        if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                    }
                    j++;
                    p++;
                }
                p++;
                c++;
                r--;
            }
            FUN_004c5fa0(&screen);
        }
    } else {
        int r = rows;
        int* c = counts;
        Point_004bf260* p = points;
        while (r > 0) {
            int j = 0;
            while (j < *c - 1) {
                int x0, y0, x1, y1;
                y1 = p[1].y;
                x1 = p[1].x;
                y0 = p[0].y;
                x0 = p[0].x;
                if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
                    FUN_004cc7ab(surface, x0, y0, x1, y1, color);
                j++;
                p++;
            }
            p++;
            c++;
            r--;
        }
        result = 1;
    }
    return result;
}
