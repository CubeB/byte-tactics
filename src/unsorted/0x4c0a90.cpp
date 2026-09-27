// Decompiled by Opus. Names are provisional.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.
//
// Not matched: the original computes row * pitch + x1 twice (lea for the
// colour pointer, then again for the depth pointer). Writing the depth
// pointer as `z += off; z += x1;` with off = row * pitch reproduces that
// shape and every instruction, but with ecx and edx swapped throughout; the
// single offset below gives the right registers with the sum computed once.

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
    unsigned char* z = surf->depth;
    unsigned char* p = surf->bits;
    int x1 = span->x1;
    int w = span->x2 - x1;
    if (w > 0) {
        int off = row * surf->pitch + x1;
        p += off;
        if (z != 0) {
            z += off;
            unsigned char z1 = span->z1 >> 16;
            if (*z <= z1) {
                *p = color;
                *z = z1;
            }
            z += w;
            p += w;
            unsigned char z2 = span->z2 >> 16;
            if (*z <= z2) {
                *p = color;
                *z = z2;
            }
        } else {
            *p = color;
            p[w] = color;
        }
    }
}
