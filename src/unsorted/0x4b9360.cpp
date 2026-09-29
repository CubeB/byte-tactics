// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// PARTIAL (83.5%). The frame, every field offset, every callee argument and
// the whole remap loop now match: the loop counter lives in a register of its
// own (that is what makes MSVC keep the rect zero in ebx and x in ebp, so the
// prologue pushes ebp and every stack offset lines up).
//
// Still different, all of it instruction order inside blocks that hold the
// right instructions:
//  - both field swaps. The original emits the two loads and the two stores as
//    one adjacent group (field_14 first, then field_10 into eax, store
//    [0x10] then [0x14]). Every source spelling of the swap tried here
//    (t = a; a = b; b = t, the reverse, two temps, a static inline helper, a
//    pointer-to-pair local) makes MSVC sink the second store below the
//    zero-extension of the width load. Going through a `unsigned char** p`
//    local over the two buffers at least keeps the loads adjacent.
//  - because that store is sunk, the second swap's value survives in edx, so
//    the remap loop reuses it (`mov ecx, edx`) where the original reloads
//    field_14, and the surface's bits field is filled from ecx where the
//    original reloads field_10.
//  - the surface block: the original loads height before width, loads x0
//    before the first store (which frees ecx), stores height after bits and
//    stores 10000 before the flags. MSVC's own order ignores the order of the
//    assignments, only the position of the `bits` assignment changes it.
//  - `cmap[c]` compiles to `mov bl, [ebx+ecx]` here and `mov bl, [ecx+ebx]` in
//    the original (same registers, swapped base and index).
//  - both `width * height` products put height in the accumulator register
//    here and width in the original. No source spelling of the product
//    (operand order, casts, locals, indexing, unsigned) changes that.
//
// Claude Sonnet 5.5 pass (#589): 83.1 to 87.3 percent. It is compiler state:
// scoring this file with N unused `extern int dummyK;` lines in front (not
// committed) gives 83.1 for N = 8 to 32, 83.9 for 40 to 88 and 87.3 for every N
// from 96 to 208, and at 87.3 two of the differences below disappear (the
// `width * height` accumulator and the `cmap[c]` base and index order both match
// the original). The legitimate way to that state is a header: headers.py finds
// `<string.h>` alone (also `<windows.h>`, `<ddraw.h>`), so the file includes
// <string.h>, as a real game file would. The notes above about the two products
// and `cmap[c]` are therefore obsolete; what is left at 87.3 is the swaps and the
// surface block. Buffer swap spellings re-scored with <string.h> in scope: a
// pointer-to-pair local (this file), the same with the two loads reversed
// (87.3, same), `t = a; a = b; b = t` on the fields directly (67.5), two named
// temporaries (67.5, 67.5).
//
// Draws a sprite with a colour-remap effect: the destination surface is
// blitted into the sprite's scratch buffer, then the sprite's index table is
// remapped through that buffer (32000 uses the sprite's flat colour) and the
// result is drawn with FUN_004b7f90.

#include <string.h>

struct Sprite_4b9360 {
    unsigned short width;        // +0x0
    unsigned short height;       // +0x2
    short x;                     // +0x4
    short y;                     // +0x6
    unsigned char color;         // +0x8
    unsigned char field_9;       // +0x9
    unsigned char frames;        // +0xa
    unsigned char field_b;       // +0xb
    char unknown_c[4];           // +0xc
    unsigned char* buffers[2];   // +0x10, the two interchangeable scratch buffers
};

struct Surface_4b9360 {
    int width;                   // +0x0
    int height;                  // +0x4
    int pitch;                   // +0x8
    int bits;                    // +0xc
    int field_10;                // +0x10
    int field_14;                // +0x14
    unsigned short x;            // +0x18
    unsigned short y;            // +0x1a
    char unknown_1c[0x10];       // +0x1c
    unsigned int flag0 : 1;      // +0x2c
    unsigned int flag1 : 1;
};

struct Rect_4b9360 {
    int left;                    // +0x0
    int top;                     // +0x4
    int right;                   // +0x8
    int bottom;                  // +0xc
};

void __stdcall FUN_004c69c0(Surface_4b9360* surface);
void __stdcall FUN_004c6d20(void* dst, void* src, Rect_4b9360* rect, Rect_4b9360* pos);
void __stdcall FUN_004b7f90(void* dst, Sprite_4b9360* sprite, int x, int y);

// FUNCTION: 0x4b9360
void __stdcall FUN_004b9360(void* dst, Sprite_4b9360* sprite, int x, int y)
{
    unsigned char** p = sprite->buffers;
    unsigned char* t = p[0];
    p[0] = p[1];
    p[1] = t;

    Surface_4b9360 surface;
    surface.width = sprite->width;
    surface.pitch = sprite->width;
    surface.height = sprite->height;
    surface.bits = (int)sprite->buffers[0];
    surface.field_10 = 10000;
    surface.field_14 = -1;
    surface.x = sprite->x;
    surface.y = sprite->y;
    surface.flag0 = 1;
    surface.flag1 = 0;
    FUN_004c69c0(&surface);

    Rect_4b9360 srcRect;
    srcRect.left = 0;
    srcRect.right = surface.width - 1;
    srcRect.top = 0;
    srcRect.bottom = surface.height - 1;

    Rect_4b9360 dstRect;
    dstRect.left = x - sprite->x;
    dstRect.right = x + surface.width - sprite->x - 1;
    dstRect.top = y - sprite->y;
    dstRect.bottom = y + surface.height - sprite->y - 1;

    FUN_004c6d20(&surface, dst, &dstRect, &srcRect);

    t = p[0];
    p[0] = p[1];
    p[1] = t;

    int n = sprite->width * sprite->height;
    unsigned char* cmap = sprite->buffers[1];
    unsigned short* idx = (unsigned short*)sprite->buffers[0];
    unsigned char* out = cmap + n;
    int i = n;
    while (i != 0) {
        short c = *idx;
        unsigned char v;
        if (c != 32000)
            v = cmap[c];
        else
            v = sprite->color;
        *out = v;
        out++;
        cmap++;
        idx++;
        i--;
    }

    int m = sprite->width * sprite->height;
    int saved = (int)sprite->buffers[0];
    sprite->buffers[0] = sprite->buffers[1] + m;
    FUN_004b7f90(dst, sprite, x, y);
    sprite->buffers[0] = (unsigned char*)saved;
}
