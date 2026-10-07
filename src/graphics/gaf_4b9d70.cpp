// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
//
// Suspected bugs:
//  - The horizontal copy count `n` is clamped to dst->width but dstCol is not
//    subtracted from it, so when dstCol > 0 the inner loop writes up to dstCol
//    bytes past the end of the destination row. The vertical clip does account
//    for dstRow. Kept as the original does.
//  - The inner loop guards on `n == 0` and not on `n <= 0`, so a negative n
//    (possible when srcCol > src->width, i.e. when x is large) counts down to
//    zero and wraps round, overwriting the row about 2^32 times.

// Must include <string.h>: dropping it changes the generated code.
#include <string.h>

struct Image_004b9d70 {
    unsigned short width;   // +0x0
    unsigned short height;  // +0x2
    short x;                // +0x4
    short y;                // +0x6
    unsigned char colorKey; // +0x8
    char unknown_9[7];      // +0x9
    unsigned char* data;    // +0x10
};

// Clipped 8-bit sprite blit: copies the part of `src` that overlaps `dst`,
// skipping source pixels equal to the source colour key and writing the
// destination colour key.
// FUNCTION: 0x4b9d70
void __stdcall CutOutFrame(Image_004b9d70* src, Image_004b9d70* dst, int x, int y)
{
    int srcCol, dstCol, srcRow, dstRow;
    x += src->x - dst->x;
    y = (dst->y - src->y) - y;
    // Plain if/else, not an initialisation plus override: fixes the clip block.
    if (y < 0) {
        srcRow = -y;
        dstRow = 0;
    } else {
        srcRow = 0;
        dstRow = y;
    }

    dstCol = -x;
    if (dstCol < 0) { srcCol = -dstCol; dstCol = 0; } else { srcCol = 0; }

    int n;
    // Clamp stays an if/else with these arms: fixes the register allocation.
    if (src->width - srcCol > dst->width)
        n = dst->width;
    else
        n = src->width - srcCol;

    for (; srcRow < src->height; srcRow++, dstRow++) {
        if (dstRow >= dst->height)
            break;
        unsigned char* s = src->data + srcRow * src->width + srcCol;
        unsigned char* d = dst->data + dstRow * dst->width + dstCol;
        int i = n;
        while (i != 0) {
            if (*s != src->colorKey)
                *d = dst->colorKey;
            s++;
            d++;
            i--;
        }
    }
}