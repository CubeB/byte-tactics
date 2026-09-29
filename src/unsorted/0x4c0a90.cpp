// Decompiled by Opus, finished by deepseek-v4.1-flash. Names are provisional.
// Codex / GPT-6 retest in #13:
// split row and x offsets, unsigned offsets and wider colour
// parameters did not reproduce the original repeated sum and registers.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.
//
// deepseek-v4.1-flash pass in #1337: 86.5 percent, 118 of 118 bytes (up from
// 85.7 percent, 116 bytes). The score moved by reading the pitch into an
// `unsigned short` local before the row multiply. That single change is what
// keeps `surf` in ecx through the whole function; without it every spelling of
// the offset (`int off = row * pitch`, the pitch read inline, or the offset
// expression written twice) flips surf to edx and swaps ecx/edx from the first
// instruction on, which is a hard 49.5 percent. Confirmed 86.5 with the offset
// as `row * pitch + x1` (this file), `x1 + row * pitch`, `pitch * row + x1`
// and `(row * pitch) + x1`; all are byte-identical.
//
// What still differs (two hunks):
//  * the pitch load. Original: `xor ebx,ebx / mov bx,[ecx] / mov ecx,[esp+0x14]
//    / imul ecx,ebx` (zero-extend into scratch ebx, row into ecx, register
//    multiply). Ours: `mov cx,[ecx] / and ecx,0xffff / imul ecx,[esp+0x14]`
//    (pitch into surf's ecx, row as the memory operand of imul). The original
//    head is exactly what the inline `row * surf->pitch` gave (the previous
//    file's head), but that spelling loses surf to edx. Every local pitch type
//    (`int`, `unsigned short`) keeps the registers right and takes the memory
//    operand.
//  * the offset is still computed once. Original materialises `off + x1` twice:
//    `lea ebx,[ecx+edi] / add esi,ebx` for the colour pointer (off stays in
//    ecx), then `add ecx,edi / add eax,ecx` for the depth pointer, and reads
//    `mov bl,[eax]`. Ours folds it into `add ecx,edi / add esi,ecx` then
//    `mov bl,[eax+ecx] / add eax,ecx`. Writing the offset twice is what produces
//    the lea, but only when the pitch is read inline, and that spelling flips
//    the allocation (49.5). So the lea and the correct register set have not yet
//    been reached together; the head difference and the offset difference look
//    coupled through which register holds the multiply result.
//
// Scratch evidence in build/scratch/0x4c0a90/ (v2 to v33): the two separate
// offset expressions (v1, v10, v14, v17, v18, v25, v32) all land at 49.5 with
// the swap; `int off = row*pitch` with two adds (v2, v16, v30) at 56.6; an
// `int pitch` local (v15) at 67.3; the `unsigned short pitch` local family
// (v20, v26, v28, v31) all at 86.5.

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
        unsigned short pitch = surf->pitch;
        p += row * pitch + x1;
        if (z != 0) {
            z += row * pitch + x1;
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
