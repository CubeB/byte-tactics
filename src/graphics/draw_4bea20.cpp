// Decompiled by space-bunny-free. Names are provisional.
// Clips the segment (x0, y0) - (x1, y1) to the rect the surface hands back
// from Surface::GetClipRect (the surface's own copy at +0x1c, fetched
// into a local), moving each end point in turn against the four edges, and
// returning 0 when an end point is on the wrong side of an edge or when the
// delta of the axis being moved is zero.
//
// The surface is never dereferenced except as `this`, so nothing in the clip
// steps comes out of it.
//
// Each end point is tested against the four edges in the order left, top,
// right, bottom, and the axis tested alternates x, y, x, y.
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
    // dx is declared before dy.
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
        // Last delta test is positive and falls out to `goto fail`: keeps the
        // block's return 1 from merging with the trailing one.
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
