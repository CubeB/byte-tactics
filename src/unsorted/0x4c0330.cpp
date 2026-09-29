// Decompiled by Sonnet 5.5, finished by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
// Fills a convex polygon with one colour, into `surface` or into the locked
// screen when `surface` is null. It finds the top and bottom vertices and the
// horizontal extent, rejects the polygon when it lies wholly outside the
// surface's clip rect, then walks the left edges (backwards from the top
// vertex to the bottom one) and the right edges (forwards) into a table of
// per-scanline x positions with 16.16 steps, and finally fills each scanline
// span clipped to the rect. Returns 1 when something was drawn.
//
// NOT MATCHED: 75.9%, 945 of 932 bytes. Frame matches exactly (0x14060:
// twelve dword locals at [esp+0x10..0x3c], the 0x30-byte locked-screen struct
// at 0x40 and the 0x800 x 0x28 span table at 0x70). Init sequence, bounds
// loop, clip rejects/clamps and both edge-walk bodies follow the original.
// The edge walks use clip.top/clip.bottom as their y bounds (kept in ebx /
// reloaded from the clip rect), while the span fill iterates minY..maxY.
//
// Still differs: (1) home-slot permutation: mine assigns minY=0x10,
// minX=0x14, locked=0x18, maxY=0x1c, minIdx=0x24?, maxIdx=0x28? against the
// original minX=0x10, locked=0x14, maxY=0x18, minY=0x1c, maxIdx=0x24,
// minIdx=0x28, so most [esp+X] operands below the lock struct are permuted;
// declaration order and literal-vs-copy initialisers had no effect on this.
// (2) Both walks store j to its home on the wrap path too (mine) where the
// original stores only the unwrapped value and re-derives at the latch
// (`mov eax,[j]; test; jge; mov eax,[n]; dec eax`), costing ~4 bytes per
// walk; the latch then compares directly instead. A defensive bottom adjust
// (`if (i<0) i=n-1` after `i=j`) made it worse (56.0%). (3) Inner y-loop:
// original does copy-fx, fx+=slope, shift, store, sp++ (`mov [edx],esi; add
// edx,0x28`); an explicit temp copy collapsed my code instead (65.3%).
// (4) Fill counter lives in memory in mine, in ebp in the original; colour
// byte handling differs accordingly.
#include <string.h>

struct Rect_004c0330 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor

    Rect_004c0330* FUN_004c6ae0(Rect_004c0330* out);
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

int __stdcall FUN_004c5e70(Class_004c6ae0* out);
int __stdcall FUN_004c5fa0(Class_004c6ae0* s);

// FUNCTION: 0x4c0330
int __stdcall FUN_004c0330(Class_004c6ae0* surface, Point_004c0330* points, int n, unsigned char color)
{
    Class_004c6ae0 screen;
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
    int j;
    int dy;

    if (surface == 0) {
        if (FUN_004c5e70(&screen) == 0)
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
    surface->FUN_004c6ae0(&clip);
    if (maxX < clip.left) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (minX > clip.right) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (maxY < clip.top) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (minY > clip.bottom) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (minY < clip.top)
        minY = clip.top;
    if (maxY > clip.bottom)
        maxY = clip.bottom;
    if (maxY == minY) {
        if (locked)
            FUN_004c5fa0(&screen);
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
                sp++;
                fx += slope;
            }
        }
        i = j;
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
                sp++;
                fx += slope;
            }
        }
        i = j;
    } while (i != maxIdx);

    Span_004c0330* s = span;
    for (i = minY; i < maxY; i++) {
        if (s->right > clip.right)
            s->right = clip.right;
        if (s->left < clip.left)
            s->left = clip.left;
        int w = s->right - s->left;
        if (w > 0)
            memset(surface->pixels + surface->pitch * i + s->left, color, w);
        s++;
    }
    if (locked)
        FUN_004c5fa0(&screen);
    return 1;
}
