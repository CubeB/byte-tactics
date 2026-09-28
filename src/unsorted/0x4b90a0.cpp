// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial, best 82.8%. Two scheduling differences remain that source rewrites
// did not move (all header sets from tools/headers.py were also tried):
//  1. The destination row pointers: the original adds xoff to the plane first
//     and the row stride last ((xoff + plane) + stride); MSVC 5 here always
//     reassociates to ((plane + stride) + xoff) and also moves the yoff spill
//     before the bounds test instead of into the loop head.
//  2. The threshold compare accumulates *sp1 + level into the other register
//     (ebx instead of edx) and loads *dp1 into edx instead of ebx.

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
    int xoff = dst->field_4 - src->field_4 + x;
    int yoff = dst->field_6 - src->field_6 + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->plane0;
    unsigned char* sp1 = src->plane1;
    for (int row = 0; row < src->height; row++, yoff++) {
        int stride = dst->width * yoff;
        unsigned char* dp0 = xoff + dst->plane0 + stride;
        unsigned char* dp1 = xoff + dst->plane1 + stride;
        int n = src->width;
        while (n--) {
            unsigned char c = *sp0;
            if (c != src->colorKey && *sp1 + level >= *dp1) {
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
