// Decompiled by deepseek-v4.1-flash, finished by Claude Sonnet 5.5, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by space-bunny-free, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Claude Opus 5.5 (#5673, 2026-10-04): MATCH. The row offset is written
// inline in both row pointers, `dst->width * (yoff + row)`, not kept in a
// `stride` local with `yoff++` in the loop increment. MSVC CSEs the two copies
// and strength-reduces `yoff + row` into a new induction variable, initialised
// from yoff in the loop preheader: that is the original's yoff store after the
// loop's `jbe` (no other function under 1500 bytes in the exe stores a
// register there that the loop test does not read). The <windows.h> and
// <math.h> declaration count
// then gives the original's (xoff + plane) + stride row head and registers:
// with no headers the same body is 69.5%, and <math.h> alone or the whole set
// of <windows.h>, <string.h>, <math.h> and <stdlib.h> also match.
#include <windows.h>
#include <math.h>

struct Bitmap_004b90a0 {
    unsigned short width;      // +0x0
    unsigned short height;     // +0x2
    short field_4;             // +0x4
    short field_6;             // +0x6
    unsigned char colorKey;    // +0x8
    char unknown_9[7];         // +0x9
    unsigned char* plane0;     // +0x10
    unsigned char* plane1;     // +0x14
};

// FUNCTION: 0x4b90a0
void __stdcall FUN_004b90a0(Bitmap_004b90a0* src, Bitmap_004b90a0* dst,
                            int x, int y, int level)
{
    int yoff;
    int xoff;
    xoff = dst->field_4 - src->field_4 + x;
    yoff = dst->field_6 - src->field_6 + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->plane0;
    unsigned char* sp1 = src->plane1;
    for (int row = 0; row < src->height; row++) {
        unsigned char* dp0 = xoff + dst->plane0 + dst->width * (yoff + row);
        unsigned char* dp1 = xoff + dst->plane1 + dst->width * (yoff + row);
        int n = src->width;
        while (n--) {
            unsigned char c = *sp0;
            if (c != src->colorKey && *dp1 <= *sp1 + level) {
                *dp0 = c;
                *dp1 = *sp1 + level;
            }
            dp0++;
            sp0++;
            dp1++;
            sp1++;
        }
    }
}
