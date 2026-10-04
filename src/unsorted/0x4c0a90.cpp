// Decompiled by Opus, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by GPT-6, re-verified by GPT-6, re-examined by Claude Opus 5.5, finished by Claude Opus 5.5. Names are provisional.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.
//
// MATCH (#5596). The earlier 96.2% file could not combine the original's
// prologue (depth loaded above the `w > 0` test) with its colouring (surf in
// ecx, w in edx). With the depth pointer read above the test, C2 ranked w
// (66) above the row-offset product (60), giving the ecx/edx mirror. What
// closes the gap is `int z = span->z1;` read inside the `w > 0` block, before
// the depth test, the way the matched sibling 0x4c06e0 writes it. Its one use
// is in the next block, so the optimizer cannot fold it into the shift; C2
// counts it (and span) in that block, K 6 to 8, so the product rises to 76
// against w's 64 (c2prio), and z itself ends in memory, read at its use, so
// no code moves. `start` mirrors 0x4c06e0 too. <windows.h> sets
// the product's operand order (row first); without it the product is
// pitch-first (41.9%).

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

// FUNCTION: 0x4c0a90
void __stdcall FUN_004c0a90(int row, Span_004c0a90* span, Surface_004c0a90* surf, unsigned char color)
{
    unsigned char* d = surf->depth;
    unsigned char* p = surf->bits;
    int w = span->x2 - span->x1;
    if (w > 0) {
        int start = span->x1;
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
