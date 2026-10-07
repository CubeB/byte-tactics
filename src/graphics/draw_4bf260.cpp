// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
//
// Draws a grid of connected lines into `surface`, or into the screen (locked
// with LockScreen, unlocked with UnlockScreen) when `surface` is null:
// `rows` polylines, where counts[r] is the number of vertices of row r and
// the vertices follow one another in `points`. Every segment is clipped by
// ClipLine and drawn by FUN_004cc7ab. Like 0x4bf060 it returns the lock result
// on the screen path (a failed lock returns 0 without unlocking) and 1 on the
// caller-surface path.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor
};

struct Point_004bf260 {
    int x;                             // +0x0
    int y;                             // +0x4
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl FUN_004cc7ab(Surface* dst, int x0, int y0, int x1, int y1, int color);

// FUNCTION: 0x4bf260
int __stdcall DrawPolylines(Surface* surface, Point_004bf260* points, int* counts,
                           int rows, int color)
{
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            // counts is copied before points: the original loads the arguments in that order.
            int r = rows;
            int* c = counts;
            Point_004bf260* p = points;
            while (r > 0) {
                int j = 0;
                // A while loop, not a for: the latch is counter increment, then pointer bump.
                while (j < *c - 1) {
                    // Declared x0, y0, x1, y1 but assigned y1, x1, y0, x0: fixes the stack slots.
                    int x0, y0, x1, y1;
                    y1 = p[1].y;
                    x1 = p[1].x;
                    y0 = p[0].y;
                    x0 = p[0].x;
                    // Kept: the original's inlined segment drawer has its own null-surface lock.
                    if (&screen == 0) {
                        Surface inner;
                        if (LockScreen(&inner)) {
                            if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                                FUN_004cc7ab(&inner, x0, y0, x1, y1, color);
                            UnlockScreen(&inner);
                        }
                    } else {
                        if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                            FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                    }
                    j++;
                    p++;
                }
                p++;
                c++;
                r--;
            }
            UnlockScreen(&screen);
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
                if (ClipLine(surface, &x0, &y0, &x1, &y1))
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
