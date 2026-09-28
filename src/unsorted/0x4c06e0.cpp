// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Draws a full horizontal span: every pixel from x1 to x2 gets the colour
// when it passes the depth test (the depth buffer keeps the integer part of
// the 16.16 depth), or the row is filled unconditionally when the surface has
// no depth buffer. Sibling of 0x4c0a90, which plots only the two end points.
//
// Still differs (61.5%): MSVC 5 gives the span pointer a callee-saved
// register (esi) and puts depth in ebx, while the original keeps span in the
// volatile ecx and depth in esi. That choice cascades through the rest of the
// function: z1 and the loop accumulator z swap registers with span/count, and
// the memset counter swaps with the depth pointer. The body below is written
// with the per-pixel step as an inlined helper, which reproduces the original
// loop's shape; every variant tried for the head (locals for x1/x2/z1, the
// order of the surf and span loads, do/while vs while vs for, a reference or
// copy of the span pointer, __fastcall with unused register args) still made
// span a callee-saved register. What differs is the register allocation, not
// the control flow, offsets or stack slots.

#include <string.h>

struct Span_004c06e0 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
};

struct Surface_004c06e0 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

static inline void Plot_004c06e0(unsigned char* p, unsigned char* d, int z, char color)
{
    unsigned char zi = (unsigned char)(z >> 16);
    if (*d <= zi) { *p = color; *d = zi; }
}

// FUNCTION: 0x4c06e0
void __stdcall FUN_004c06e0(int row, Span_004c06e0* span, Surface_004c06e0* surf, char color)
{
    unsigned char* d = surf->depth;
    unsigned char* p = surf->bits;
    int w = span->x2 - span->x1;
    int slope = (span->z2 - span->z1) / w;
    if (span->x1 < 0) {
        span->z1 = span->z1 - slope * span->x1;
        span->x1 = 0;
    }
    if (span->x2 > (int)surf->pitch - 1)
        span->x2 = surf->pitch - 1;
    int start = span->x1;
    int count = span->x2 - start;
    if (count > 0) {
        int z = span->z1;
        p += row * surf->pitch + start;
        if (d != 0) {
            d += row * surf->pitch + start;
            int n = count;
            do {
                Plot_004c06e0(p, d, z, color);
                p++; z += slope; d++;
            } while (--n);
        } else {
            memset(p, color, count);
        }
    }
}
