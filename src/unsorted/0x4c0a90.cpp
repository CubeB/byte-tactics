// Decompiled by Opus, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by claude-sonnet-5-5. Names are provisional.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.
//
// Status: 96.2% (118 of 118 bytes). The old 86.8% probe (an `unsigned short`
// truncating row) was replaced by correct-semantics source.
// What made the difference: declare the depth pointer `z` WITHOUT an initialiser
// and assign `z = surf->depth` only inside `if (w > 0)`, and compute
// `int off = row * surf->pitch;` once. The inline `row * surf->pitch` head
// (`xor ebx,ebx / mov bx,[ecx] / mov ecx,[esp+0x14] / imul ecx,ebx`), the
// register set (surf=ecx, span=ebp, w=edx, p=esi, x1=edi, z=eax), the `lea ebx`
// / `add ecx,edi` pair and all branch targets then match.
// Only difference left: the original loads surf->depth (mov eax,[ecx+0x14])
// at the top, before the x2 load and before the `jle`; ours loads it after the
// `jle`, so `mov esi,[ecx+0x10]` moves up one slot. Making z live at the top
// (`unsigned char* z = surf->depth;`, with or without `off`) restores that
// load but flips the allocator to surf=edx / w=ecx (56.6%). A separate top-level
// `d = surf->depth` used for the test keeps the right registers (69.2%) but
// spills span out of ebp (two live webs for the depth pointer).
// Tried without effect: decl-order permutations, x1/w spelled via span->x1,
// w spelled as a repeated expression, extra surf refs, `register`, p/z built
// from surf->bits/surf->depth again, off as pitch*row / x1+off sums.

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
    unsigned char* z;
    unsigned char* p = surf->bits;
    int x1 = span->x1;
    int w = span->x2 - x1;
    if (w > 0) {
        z = surf->depth;
        int off = row * surf->pitch;
        p += off + x1;
        if (z != 0) {
            z += off + span->x1;
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