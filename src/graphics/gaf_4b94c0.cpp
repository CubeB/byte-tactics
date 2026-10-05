// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

struct Sprite_004b94c0 {
    unsigned short width;        // +0x0
    unsigned short height;       // +0x2
    short x;                     // +0x4
    short y;                     // +0x6
    char unknown_8[8];           // +0x8
    unsigned char* bits;         // +0x10
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

struct Rect_004b94c0 {
    int left;                    // +0x0
    int top;                     // +0x4
    int right;                   // +0x8
    int bottom;                  // +0xc
};

void __stdcall ResetClipRect(Surface* surface);
void __stdcall CopySurfaceRect(void* dst, void* src, Rect_004b94c0* rect, Rect_004b94c0* pos);

// FUNCTION: 0x4b94c0
void __stdcall GrabBackground(void* dst, Sprite_004b94c0* sprite, int x, int y)
{
    Surface surface;
    surface.width = sprite->width;
    surface.height = sprite->height;
    surface.pitch = sprite->width;
    surface.bits = (int)sprite->bits;
    surface.field_10 = 10000;
    surface.field_14 = -1;
    surface.x = sprite->x;
    surface.y = sprite->y;
    surface.flag0 = 1;
    surface.flag1 = 0;
    ResetClipRect(&surface);

    Rect_004b94c0 srcRect;
    srcRect.left = 0;
    srcRect.right = surface.width - 1;
    srcRect.top = 0;
    srcRect.bottom = surface.height - 1;

    Rect_004b94c0 dstRect;
    dstRect.left = x - sprite->x;
    dstRect.right = surface.width + x - sprite->x - 1;
    dstRect.top = y - sprite->y;
    dstRect.bottom = surface.height + y - sprite->y - 1;

    CopySurfaceRect(&surface, dst, &dstRect, &srcRect);
}
