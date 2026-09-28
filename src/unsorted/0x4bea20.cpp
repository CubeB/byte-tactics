// Decompiled by space-bunny-free. Names are provisional.
// Clips the segment (x0, y0) - (x1, y1) to the destination surface's rect
// (fetched through Class_004c6ae0::FUN_004c6ae0 into a local copy), moving
// each end point in turn against the four edges and returning 0 when an end
// point is on the wrong side of an edge, or when the delta of the axis being
// moved is zero. The x delta is taken from the surface's first dword, not from
// *x1, and the four clip steps for the second end point reuse that same
// delta, so the interpolated x of the second end point moves on the y steps
// and the y of the first end point moves on the x steps of the second half:
// the arithmetic is what the exe does (see the note in the report).
//
// NOT MATCHING: 24.7 percent, 634 bytes against 589. The previous pass reached
// its step limit with its best variant still only in build/scratch, so nothing
// above 24.7 percent was recovered; the four `w_T_*` scratch files that were
// meant to hold the promising "four loaded-value temporaries" version declare
// xa, xv, yv and y1v and then never use them, so they compile to exactly this
// file. Treat any claim of a higher score for those as unverified.
//
// What IS established, and is the reason to keep going rather than restart:
//  - the signature is confirmed by nine already-matched callers (0x4be950,
//    0x4bed70, 0x4bec70, 0x4bf060, 0x4c01a0), all of which pass
//    (surface, &x0, &y0, &x1, &y1).
//  - eight clip steps, four per end point, each of the shape
//    `if (cond) { if (flag) return 0; if (delta == 0) goto fail; interpolate;
//    store; }`.
//  - the eight flag failures carry their own inline `return 0` epilogues, and
//    the six zero-delta failures share one epilogue at the end, which is why
//    the source needs a `goto fail` (MSVC 5 never merges identical returns).
//  - steps 7 and 8 test with `>` (the original branches on `jle`) while steps
//    1 to 6 test with `<` (`jge`). That asymmetry is read straight off the
//    disassembly and is written into the file below. It does not show up in
//    the score yet, because the 45-byte size difference dominates the diff;
//    it is kept because the disassembly evidence is direct.
//  - the last block returns 1 inline, with its own epilogue carrying a hoisted
//    `mov eax, 1`, and a second separate `return 1` tail follows the shared
//    `fail` block. Never reproduced: a source with the last step returning 1
//    inside the block plus `goto ok; fail: return 0; ok: return 1;` always had
//    MSVC merge the two, so the original's shape is something else, perhaps an
//    `else` arm or the `return 1` written before the `goto ok`.
//
// The one blocker, and everything tried against it: the prologue. The original
// spills the two comparison flags into the DEAD argument slots ([esp+0x28] is
// argument 2's slot, [esp+0x2c] is argument 3's) and keeps the frame at
// `sub esp, 0x10`, so the only locals are the four dwords of the rect. This
// file gives the first-declared flag a NEW frame slot (`sub esp, 0x14`) and
// hoists the `*y1` load above the `to_right` comparison, so the flag slots are
// wrong and dx/dy land in swapped registers (here dx->ebx and dy->edi, the
// original has dx->edi and dy->ebx). Rejected without success: all 24
// declaration orders of (to_right, downwards, dy, dx); bool and unsigned
// flags; ternaries; `!(*x0 > A)`; uninitialised-then-assigned; loading the four
// values into temps; declaring the rect before or after the flags; and moving
// the call. Untried, and the most promising: have the delta statements and the
// flag statements share a NAMED value instead of being separate locals (compute
// `int d = dst->field_0 - *x0;` and then `to_right = (*x0 <= dst->field_0);` with
// `dx` never existing as its own local, or the reverse), or introduce an
// `int* px = x0;` style local pointer so argument 2's slot dies early enough
// for the allocator to notice the merge.
#include <windows.h>

struct Rect_004bea20 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    int field_0;
    char unknown_4[0x1c - 4];
    Rect_004bea20 field_1c;                // +0x1c

    Rect_004bea20* FUN_004c6ae0(Rect_004bea20* out);
};

// FUNCTION: 0x4bea20
int __stdcall FUN_004bea20(Class_004c6ae0* dst, int* x0, int* y0, int* x1, int* y1)
{
    Rect_004bea20 r;
    int to_right = (*x0 <= dst->field_0);
    int downwards = (*y0 <= *y1);
    int dy = *y1 - *y0;
    int dx = dst->field_0 - *x0;
    dst->FUN_004c6ae0(&r);
    if (*x0 < r.left) {
        if (to_right == 0)
            return 0;
        if (dx == 0)
            return 0;
        *y0 += (r.left - *x0) * dy / dx;
        *x0 = r.left;
    }
    if (*y0 < r.top) {
        if (downwards == 0)
            return 0;
        if (dy == 0)
            return 0;
        *x0 += (r.top - *y0) * dx / dy;
        *y0 = r.top;
    }
    if (*y0 < r.right) {
        if (downwards != 0)
            return 0;
        if (dy == 0)
            return 0;
        *x0 += (r.right - *y0) * dx / dy;
        *y0 = r.right;
    }
    if (*y0 < r.bottom) {
        if (downwards != 0)
            return 0;
        if (dy == 0)
            return 0;
        *x0 += (r.bottom - *y0) * dx / dy;
        *y0 = r.bottom;
    }
    if (*x1 < r.left) {
        if (to_right != 0)
            return 0;
        if (dx == 0)
            return 0;
        *y1 += (r.left - *x1) * dy / dx;
        *x1 = r.left;
    }
    if (*y1 < r.top) {
        if (downwards != 0)
            return 0;
        if (dy == 0)
            return 0;
        *x1 += (r.top - *y1) * dx / dy;
        *y1 = r.top;
    }
    if (*x1 > r.right) {
        if (to_right == 0)
            return 0;
        if (dx == 0)
            return 0;
        *y1 += (r.right - *x1) * dy / dx;
        *x1 = r.right;
    }
    if (*y1 > r.bottom) {
        if (downwards == 0)
            return 0;
        if (dy == 0)
            return 0;
        *x1 += (r.bottom - *y1) * dx / dy;
        *y1 = r.bottom;
        return 1;
    }
    return 1;
}
