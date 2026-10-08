// Decompiled by Opus, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by GPT-6, re-verified by GPT-6, re-examined by Claude Opus 5.5, finished by Claude Opus 5.5. Names are provisional.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.

// Needed: sets the operand order of the row product (row first).
#include <windows.h>

struct Span_004c0a90 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
};

struct Surface_004c0a90 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

// Not gathered into draw.cpp: its register allocation follows symbol ids.
// FUNCTION: 0x4c0a90
void __stdcall PlotSpanEnds(int row, Span_004c0a90* span, Surface_004c0a90* surf, unsigned char color)
{
    unsigned char* d = surf->depth;
    unsigned char* p = surf->bits;
    int w = span->x2 - span->x1;
    if (w > 0) {
        int start = span->x1;
        // Read here, before the depth test.
        int z = span->z1;
        p += row * surf->pitch + start;
        if (d != 0) {
            d += row * surf->pitch + span->x1;
            unsigned char z1 = z >> 16;
            if (*d <= z1) {
                *p = color;
                *d = z1;
            }
            d += w;
            p += w;
            unsigned char z2 = span->z2 >> 16;
            if (*d <= z2) {
                *p = color;
                *d = z2;
            }
        } else {
            *p = color;
            p[w] = color;
        }
    }
}
