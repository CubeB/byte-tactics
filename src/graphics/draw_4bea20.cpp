// Decompiled by space-bunny-free. Names are provisional.
// Clips the segment (x0, y0) - (x1, y1) to the rect the surface hands back
// from Surface::GetClipRect (the surface's own copy at +0x1c, fetched
// into a local), moving each end point in turn against the four edges, and
// returning 0 when an end point is on the wrong side of an edge or when the
// delta of the axis being moved is zero.
//
// The prologue is easy to misread, because the two `[esp + 0x24]` loads are
// different slots: 0x4bea2a runs with three registers pushed (esp = orig-0x1c)
// and so reads orig+8, the x0 pointer, while 0x4bea60 runs with four pushed
// (esp = orig-0x20) and so reads orig+4, the surface, as the `this` of the
// call. Likewise the entry load at 0x4bea23 ([esp+0x20] with esp = orig-0x10)
// is orig+0x10, the x1 pointer, which is what makes the first comparison
// `*x0 <= *x1` and the delta `*x1 - *x0`. The surface is never dereferenced
// except as `this`, so nothing in the clip steps comes out of it.
//
// Each end point is tested against the four edges in the order left, top,
// right, bottom, and the axis tested alternates x, y, x, y. The flag and the
// delta guard of a step are the two ways out of it: the flag failure returns
// 0 from inside the block (eight separate epilogues, one per step, because
// MSVC 5 never merges two identical returns) while the zero-delta failure
// jumps to the one shared `fail` at the end.
//
// Two spellings here are load bearing. `dx` is declared before `dy` (any other
// order of the four prologue locals costs about 25 percent, mostly through the
// register dx and dy end up in), and the last step tests its delta positively
// and falls out of the block to `goto fail`, rather than testing it negatively
// and returning: written the other way round MSVC contracts that goto into an
// inline `return 0` and tail merges the block's `return 1` with the trailing
// one, which loses 19 bytes and the shared epilogue's position.
#include <windows.h>

struct Rect_004bea20 {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    int field_0;
    char unknown_4[0x1c - 4];
    Rect_004bea20 field_1c;                // +0x1c

    Rect_004bea20* GetClipRect(Rect_004bea20* out);
};

// FUNCTION: 0x4bea20
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1)
{
    Rect_004bea20 r;
    int to_right = (*x0 <= *x1);
    int downwards = (*y0 <= *y1);
    int dx = *x1 - *x0;
    int dy = *y1 - *y0;
    dst->GetClipRect(&r);
    if (*x0 < r.left) {
        if (to_right == 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y0 += (r.left - *x0) * dy / dx;
        *x0 = r.left;
    }
    if (*y0 < r.top) {
        if (downwards == 0)
            return 0;
        if (dy == 0)
            goto fail;
        *x0 += (r.top - *y0) * dx / dy;
        *y0 = r.top;
    }
    if (*x0 > r.right) {
        if (to_right != 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y0 += (r.right - *x0) * dy / dx;
        *x0 = r.right;
    }
    if (*y0 > r.bottom) {
        if (downwards != 0)
            return 0;
        if (dy == 0)
            goto fail;
        *x0 += (r.bottom - *y0) * dx / dy;
        *y0 = r.bottom;
    }
    if (*x1 < r.left) {
        if (to_right != 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y1 += (r.left - *x1) * dy / dx;
        *x1 = r.left;
    }
    if (*y1 < r.top) {
        if (downwards != 0)
            return 0;
        if (dy == 0)
            goto fail;
        *x1 += (r.top - *y1) * dx / dy;
        *y1 = r.top;
    }
    if (*x1 > r.right) {
        if (to_right == 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y1 += (r.right - *x1) * dy / dx;
        *x1 = r.right;
    }
    if (*y1 > r.bottom) {
        if (downwards == 0)
            return 0;
        if (dy != 0) {
            *x1 += (r.bottom - *y1) * dx / dy;
            *y1 = r.bottom;
            return 1;
        }
        goto fail;
    }
    return 1;
fail:
    return 0;
}
