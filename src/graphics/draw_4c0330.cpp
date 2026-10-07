// Decompiled by Sonnet 5.5, finished by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// Fills a convex polygon with one colour, into `surface` or into the locked
// screen when `surface` is null. It finds the top and bottom vertices and the
// horizontal extent, rejects the polygon when it lies wholly outside the
// surface's clip rect, then walks the left edges (backwards from the top
// vertex to the bottom one) and the right edges (forwards) into a table of
// per-scanline x positions with 16.16 steps, and finally fills each scanline
// span clipped to the rect. Returns 1 when something was drawn.
#include <string.h>

struct Rect_004c0330 {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor

    Rect_004c0330* GetClipRect(Rect_004c0330* out);
};

struct Point_004c0330 {
    int x;
    int y;
};

struct Span_004c0330 {
    int left;                          // +0x0
    int right;                         // +0x4
    int unknown_8[8];
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);

// FUNCTION: 0x4c0330
int __stdcall ScanFillPolygon(Surface* surface, Point_004c0330* points, int n, unsigned char color)
{
    Surface screen;
    Rect_004c0330 clip;
    Span_004c0330 span[0x800];
    int locked;
    int minY;
    int maxY;
    int minX;
    int maxX;
    int minIdx;
    int maxIdx;
    int i;
    // j stays at function scope.
    int j;
    int dy;

    if (surface == 0) {
        if (LockScreen(&screen) == 0)
            return 0;
        locked = 1;
        surface = &screen;
    } else {
        locked = 0;
    }

    minY = 999999;
    maxY = -999999;
    minX = 999999;
    maxX = -999999;
    for (i = 0; i < n; i++) {
        // Test order y min, y max, x max, x min fixes the register homes.
        int y = points[i].y;
        if (y < minY) {
            minY = y;
            minIdx = i;
        }
        if (y > maxY) {
            maxY = y;
            maxIdx = i;
        }
        int x = points[i].x;
        if (x > maxX)
            maxX = x;
        if (x < minX)
            minX = x;
    }
    surface->GetClipRect(&clip);
    if (maxX < clip.left) {
        if (locked)
            UnlockScreen(&screen);
        return 0;
    }
    if (minX > clip.right) {
        if (locked)
            UnlockScreen(&screen);
        return 0;
    }
    if (maxY < clip.top) {
        if (locked)
            UnlockScreen(&screen);
        return 0;
    }
    if (minY > clip.bottom) {
        if (locked)
            UnlockScreen(&screen);
        return 0;
    }
    if (minY < clip.top)
        minY = clip.top;
    if (maxY > clip.bottom)
        maxY = clip.bottom;
    if (maxY == minY) {
        if (locked)
            UnlockScreen(&screen);
        return 0;
    }

    Span_004c0330* sp = span;
    i = minIdx;
    do {
        j = i - 1;
        if (j < 0)
            j = n - 1;
        int y1 = points[j].y;
        int y0 = points[i].y;
        if (y1 > clip.top && y0 < y1) {
            int x0 = points[i].x;
            int x1 = points[j].x;
            dy = y1 - y0;
            int slope = ((x1 - x0) << 16) / dy;
            int fx = (x0 << 16) + 0xffff;
            if (y0 < clip.top) {
                fx += (clip.top - y0) * slope;
                y0 = clip.top;
            }
            if (y1 > clip.bottom)
                y1 = clip.bottom;
            for (int y = y0; y < y1; y++) {
                sp->left = fx >> 16;
                fx += slope;
                sp++;
            }
        }
        // Wrap recomputed from the raw i - 1 here, not copied from j.
        i = i - 1;
        if (i < 0)
            i = n - 1;
    } while (i != maxIdx);

    sp = span;
    i = minIdx;
    do {
        j = i + 1;
        if (j >= n)
            j = 0;
        int y1 = points[j].y;
        int y0 = points[i].y;
        if (y1 > clip.top && y0 < y1) {
            int x0 = points[i].x;
            int x1 = points[j].x;
            dy = y1 - y0;
            int slope = ((x1 - x0) << 16) / dy;
            int fx = (x0 << 16) + 0xffff;
            if (y0 < clip.top) {
                fx += (clip.top - y0) * slope;
                y0 = clip.top;
            }
            if (y1 > clip.bottom)
                y1 = clip.bottom;
            for (int y = y0; y < y1; y++) {
                sp->right = fx >> 16;
                fx += slope;
                sp++;
            }
        }
        i = i + 1;
        if (i >= n)
            i = 0;
    } while (i != maxIdx);

    Span_004c0330* s = span;
    for (i = minY; i < maxY; i++) {
        if (s->right > clip.right)
            s->right = clip.right;
        if (s->left < clip.left)
            s->left = clip.left;
        int w = s->right - s->left;
        // Fill value read through an int lvalue; the parameter stays unsigned char
        // (the mangled name depends on it).
        if (w > 0)
            memset(surface->pixels + surface->pitch * i + s->left, *(int *)&color, w);
        s++;
    }
    if (locked)
        UnlockScreen(&screen);
    return 1;
}
