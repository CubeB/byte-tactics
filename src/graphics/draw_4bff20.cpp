// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.

// Clears a dither pattern inside the clipped rectangle `rect` of `surface`, or
// of the screen (locked with LockScreen and unlocked with UnlockScreen) when
// `surface` is null. The rect is copied to a local first because the clip
// helper ClipRectangle clips it in place. Rows alternate between the two byte
// masks (which half of each dword is kept) according to the phase parity
// `(i + phase) & 1`, the aligned middle is done a dword at a time, and the
// unaligned byte ends are filled by stepping 2 bytes at a time. Returns 1 once
// the pattern is laid down, 0 when the screen cannot be locked.
//
// MATCH. The whole match hinged on giving the trailing byte fill ONE shared
// loop after the if/else instead of spelling the loop out in both arms: with
// two copies, `x2` and the loop counter `i` swap their callee-saved registers
// (edi/ebp instead of ebp/edi) and the function comes out 14 bytes too long.
// Sharing the loop between the two `pe`/`pb` assignments fixes the register
// priority as well as the block layout.

#include <windows.h>

struct Rect_004bff20 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipRectangle(Surface* s, Rect_004bff20* r);

// FUNCTION: 0x4bff20
int __stdcall DitherRectangle(Surface* surface, Rect_004bff20* rect, int phase)
{
    Surface screen;
    if (surface == 0) {
        if (LockScreen(&screen))
            surface = &screen;
        else
            return 0;
    }
    Rect_004bff20 r = *rect;
    if (ClipRectangle(surface, &r)) {
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
            unsigned char* pe;         // last byte of the trailing fill
            unsigned char* pb;         // where that fill starts
            if ((i + phase) & 1) {
                unsigned int* q = (unsigned int*)base;
                unsigned int* qe = (unsigned int*)end;
                while (q < qe) {
                    *q &= 0xff00ff;
                    q++;
                }
                pe = (unsigned char*)q + r.right - x2;
                pb = (unsigned char*)q + 1;
            } else {
                unsigned int* q = (unsigned int*)base;
                unsigned int* qe = (unsigned int*)end;
                while (q < qe) {
                    *q &= 0xff00ff00;
                    q++;
                }
                pe = (unsigned char*)q + r.right - x2;
                pb = (unsigned char*)q;
            }
            while (pb <= pe) {
                *pb = 0;
                pb += 2;
            }
        }
    }
    if (surface == &screen)
        UnlockScreen(&screen);
    return 1;
}
