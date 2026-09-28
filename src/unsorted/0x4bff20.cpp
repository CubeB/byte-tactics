// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Clears a dither pattern inside the clipped rectangle `rect` of `surface`, or
// of the screen (locked with FUN_004c5e70 and unlocked with FUN_004c5fa0) when
// `surface` is null. Rows alternate between the two byte masks (which half of
// each dword is kept) according to the phase parity `(i + phase) & 1`, and the
// unaligned byte ends are filled by stepping 2 bytes at a time. Returns 1 once
// the pattern is laid down, 0 when the screen cannot be locked.
//
// 76.4 percent. The branch layout, the operand order and every instruction are
// right, and only one thing differs: the compiler hands ebp to `x2` and edi to
// `i` here (`lea ebp,[eax+3]` / `mov edi,eax` / `inc edi`), while ours hands
// edi to `x2` and ebp to `i` and everything that mentions those two registers
// follows from that swap. Runtime byte behaviour is identical.
//
// Tried and none of it moved the swap: every local declaration order for i/x1/
// x2, function-scope vs for-scope i, `for`/`while`/`do`-with-guard loop forms,
// signed vs unsigned x1/x2, a `static inline` RoundUp helper, an inlined
// row-fill helper, inlining x1 or x2 at their uses, `(base + x2) - x1` vs
// `base + (x2 - x1)` vs `end = pixels + row + x2`, moving the trailing bound
// to the loop condition, and all 768 header sets tools/headers.py tries (it
// found the <windows.h> shape below, which is what fixed the operand order).
// Note 0x4bf4d0, the same-style sibling, is also stuck at a callee-saved
// register swap for the same reason.

#include <windows.h>

struct Rect_004bff20 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface_004bff20 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall FUN_004c5e70(Surface_004bff20* out);
int __stdcall FUN_004c5fa0(Surface_004bff20* s);
int __stdcall FUN_004bf620(Surface_004bff20* s, Rect_004bff20* r);

// FUNCTION: 0x4bff20
int __stdcall FUN_004bff20(Surface_004bff20* surface, Rect_004bff20* rect, int phase)
{
    Surface_004bff20 screen;
    if (surface == 0) {
        if (FUN_004c5e70(&screen))
            surface = &screen;
        else
            return 0;
    }
    Rect_004bff20 r = *rect;
    if (FUN_004bf620(surface, &r)) {
        int x1 = (r.left + 3) & ~3;
        int x2 = (r.right + 3) & ~3;
        for (int i = r.top; i <= r.bottom; i++) {
            unsigned char* base = surface->pixels + i * surface->pitch + x1;
            unsigned char* end = base + x2 - x1;
            unsigned char* p = base + ((r.left + i + phase) & 1) + r.left - x1;
            while (p < base) {
                *p = 0;
                p += 2;
            }
            if ((i + phase) & 1) {
                unsigned int* q = (unsigned int*)base;
                unsigned int* qe = (unsigned int*)end;
                while (q < qe) {
                    *q &= 0xff00ff;
                    q++;
                }
                p = (unsigned char*)q + 1;
                unsigned char* pe = end + r.right - x2;
                while (p <= pe) {
                    *p = 0;
                    p += 2;
                }
            } else {
                unsigned int* q = (unsigned int*)base;
                unsigned int* qe = (unsigned int*)end;
                while (q < qe) {
                    *q &= 0xff00ff00;
                    q++;
                }
                p = (unsigned char*)q;
                unsigned char* pe = end + r.right - x2;
                while (p <= pe) {
                    *p = 0;
                    p += 2;
                }
            }
        }
    }
    if (surface == &screen)
        FUN_004c5fa0(&screen);
    return 1;
}
