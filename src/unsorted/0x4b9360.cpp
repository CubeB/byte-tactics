// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (38.5%), best found. The structure, every field offset and every
// callee argument match, but the original keeps the rect zero in ebx and `x`
// in ebp (so it pushes ebp and every later stack offset is 4 higher), while
// this source folds the zeros to immediates, drops ebp, and assigns the remap
// loop's cmap/idx/out to edi/ebx/ecx instead of ecx/edi/edx; the first surface
// store is also hoisted differently.
//
// Draws a sprite with a colour-remap effect: the destination surface is
// blitted into the sprite's scratch buffer, then the sprite's index table is
// remapped through that buffer (32000 uses the sprite's flat colour) and the
// result is drawn with FUN_004b7f90.

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
    unsigned char* field_10;     // +0x10
    unsigned char* field_14;     // +0x14
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
    unsigned char flag0 : 1;     // +0x2c
    unsigned char flag1 : 1;
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

// Match notes: structure and every field offset agree, but MSVC 5 allocates
// registers differently here. The original keeps the zero for srcRect.left/top
// in ebx (xor ebx,ebx) and x in ebp, so it pushes ebp and all its stack
// offsets are 4 higher than ours; our build folds the zero to immediate stores
// and puts the x - sprite->x temp in ebx instead, so ebp is never needed. The
// two field swaps and the remap loop then pick different registers
// (we get cmap/idx/out = edi/ebx/ecx, the original ecx/edi/edx) and the first
// surface store (field_10 = 10000) is hoisted above the swap in our build.
// FUNCTION: 0x4b9360
void __stdcall FUN_004b9360(void* dst, Sprite_4b9360* sprite, int x, int y)
{
    unsigned char* t = sprite->field_10;
    sprite->field_10 = sprite->field_14;
    sprite->field_14 = t;

    Surface_4b9360 surface;
    surface.width = sprite->width;
    surface.height = sprite->height;
    surface.pitch = sprite->width;
    surface.bits = (int)sprite->field_10;
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

    t = sprite->field_10;
    sprite->field_10 = sprite->field_14;
    sprite->field_14 = t;

    int n = sprite->width * sprite->height;
    unsigned char* cmap = sprite->field_14;
    unsigned short* idx = (unsigned short*)sprite->field_10;
    unsigned char* out = cmap + n;
    while (n != 0) {
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
        n--;
    }

    int m = sprite->width * sprite->height;
    int saved = (int)sprite->field_10;
    sprite->field_10 = sprite->field_14 + m;
    FUN_004b7f90(dst, sprite, x, y);
    sprite->field_10 = (unsigned char*)saved;
}
