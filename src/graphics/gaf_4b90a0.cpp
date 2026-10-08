// Decompiled by deepseek-v4.1-flash, finished by Claude Sonnet 5.5, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by space-bunny-free, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Stays in its own file: the merged gaf.cpp cannot place it at the symbol
// count its registers need.
// Both headers are needed: their declaration count sets the row head registers.
#include <windows.h>
#include <math.h>

struct GafFrame {
    unsigned short width;      // +0x0
    unsigned short height;     // +0x2
    short xOffset;             // +0x4
    short yOffset;             // +0x6
    unsigned char colorKey;    // +0x8
    char unknown_9[7];         // +0x9
    unsigned char* plane0;     // +0x10
    unsigned char* plane1;     // +0x14
};

// FUNCTION: 0x4b90a0
void __stdcall DrawFrameDepth(GafFrame* src, GafFrame* dst,
                            int x, int y, int level)
{
    int yoff;
    int xoff;
    xoff = dst->xOffset - src->xOffset + x;
    yoff = dst->yOffset - src->yOffset + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->plane0;
    unsigned char* sp1 = src->plane1;
    for (int row = 0; row < src->height; row++) {
        // Row offset stays inline in both pointers, not a stride local with yoff++.
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
