// Decompiled by deepseek-v4.1-flash, space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Draws a sprite with a colour-remap effect: the destination surface is blitted
// into the sprite's scratch buffer, the sprite's index table is remapped through
// that buffer (32000 uses the sprite's flat colour), and the result is drawn
// with DrawFrame.
//
// The two scratch buffers are swapped twice around the blit. That swap must go
// through a reference-taking helper: written inline, MSVC 5 forwards the
// stored value into the later `sprite->buffers[0]` read and sinks a store,
// which cascades through the whole first block and the remap loop. The
// reference parameters give MSVC two aliasing pointers, so every read after a
// swap is a fresh load, as in the original.
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

struct Surface {
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

void __stdcall ResetClipRect(Surface* surface);
void __stdcall CopySurfaceRect(void* dst, void* src, Rect_4b9360* rect, Rect_4b9360* pos);
void __stdcall DrawFrame(void* dst, Sprite_4b9360* sprite, int x, int y);

static void SwapPtr(unsigned char*& a, unsigned char*& b)
{
    unsigned char* t = a;
    a = b;
    b = t;
}

// FUNCTION: 0x4b9360
void __stdcall DrawLens(void* dst, Sprite_4b9360* sprite, int x, int y)
{
    SwapPtr(sprite->buffers[0], sprite->buffers[1]);

    Surface surface;
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
    ResetClipRect(&surface);

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

    CopySurfaceRect(&surface, dst, &dstRect, &srcRect);

    SwapPtr(sprite->buffers[0], sprite->buffers[1]);

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
    DrawFrame(dst, sprite, x, y);
    sprite->buffers[0] = (unsigned char*)saved;
}
