// Decompiled by Sonnet 5.5. Names are provisional.
// Draws a bitmap tree scaled by (sx, sy) at x, y into `dst`, or into the
// locked screen when `dst` is null. A record with a child count draws every
// child by recursion. A leaf scales its size and origin, clips the
// destination rectangle to the surface's clip rect with FUN_004b7e60, maps
// the clipped source rectangle back to bitmap pixels and then walks the
// destination pixels with 16.16 steps through the bitmap, blending every
// pixel that is not the transparent colour through the display's 256x256
// table at +0xc0.
//
// NOT MATCHED: 93.2%, size exact. What differs: the two rect end points
// `w + left - 1` / `h + top - 1` come out as `[left+w-1]` (operands swapped in
// the lea), the two loop variables (row and the 16.16 source y) take the
// dead x/y argument slots in the other order, and the pixel address adds the column before the row base where the original adds the row base first.
// Tried: operand order of every one of those expressions, declaration order of
// the loop variables, a row pointer, and casting the pixel address.
#include <windows.h>

struct Rect_004b9740 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    char unknown_0[8];
    int pitch;                          // +0x8
    unsigned char* pixels;              // +0xc
    char unknown_10[0x1c - 0x10];
    Rect_004b9740 field_1c;             // +0x1c

    Rect_004b9740* FUN_004c6ae0(Rect_004b9740* out);
};

struct Bitmap_004b9740 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
    short dx;                           // +0x4
    short dy;                           // +0x6
    unsigned char colour;               // +0x8
    unsigned char flag9;                // +0x9
    unsigned char count;                // +0xa
    unsigned char kind;                 // +0xb
    int unknown_c;                      // +0xc
    void* field_10;                     // +0x10
};

struct Surface_004b9740 {
    int data[12];
};

struct Display_004b9740 {
    char unknown_0[0xc0];
    unsigned char* blend;               // +0xc0
};

Display_004b9740* FUN_004b6220(void);
int __stdcall FUN_004c5e70(Surface_004b9740* out);
int __stdcall FUN_004c5fa0(Surface_004b9740* s);
void __stdcall FUN_004b7e60(Rect_004b9740* other, Rect_004b9740* rect, Rect_004b9740* bounds);

// FUNCTION: 0x4b9740
void __stdcall FUN_004b9740(Class_004c6ae0* dst, Bitmap_004b9740* bmp, int x, int y, double sx, double sy)
{
    Display_004b9740* d = FUN_004b6220();
    Surface_004b9740 screen;
    if (dst == 0) {
        int ok = FUN_004c5e70(&screen);
        if (ok != 0)
            dst = (Class_004c6ae0*)&screen;
    }
    if (bmp->count > 0) {
        for (int i = 0; i < (int)bmp->count; i++)
            FUN_004b9740(dst, ((Bitmap_004b9740**)bmp->field_10)[i], x, y, sx, sy);
    } else {
        int w = (int)(bmp->width * sx);
        int h = (int)(bmp->height * sy);
        int dx = (int)(bmp->dx * sx);
        int dy = (int)(bmp->dy * sy);
        if (w > 0 && h > 0) {
            Rect_004b9740 src;
            Rect_004b9740 dest;
            Rect_004b9740 bounds;
            dest.left = x - dx;
            dest.top = y - dy;
            dest.right = w + dest.left - 1;
            dest.bottom = h + dest.top - 1;
            src.left = 0;
            src.top = 0;
            src.right = w - 1;
            src.bottom = h - 1;
            int stepX = (bmp->width << 16) / w;
            int stepY = (bmp->height << 16) / h;
            dst->FUN_004c6ae0(&bounds);
            FUN_004b7e60(&src, &dest, &bounds);
            if (dest.right >= dest.left && dest.bottom >= dest.top && src.right >= src.left &&
                src.bottom >= src.top) {
                src.left = (int)(src.left / sx);
                src.top = (int)(src.top / sy);
                src.right = (int)(src.right / sx);
                src.bottom = (int)(src.bottom / sy);
                int row = dest.top;
                int fy = src.top << 16;
                for (; row <= dest.bottom; row++) {
                    int rowBase = row * dst->pitch;
                    int srcRow = (fy >> 16) * bmp->width;
                    int fx = src.left << 16;
                    for (int col = dest.left; col <= dest.right; col++) {
                        unsigned char c = ((unsigned char*)bmp->field_10)[(fx >> 16) + srcRow];
                        if (c != bmp->colour) {
                            unsigned char* p = dst->pixels + col + rowBase;
                            *p = d->blend[(c << 8) + *p];
                        }
                        fx += stepX;
                    }
                    fy += stepY;
                }
            }
        }
    }
    if (dst == (Class_004c6ae0*)&screen)
        FUN_004c5fa0(&screen);
}
