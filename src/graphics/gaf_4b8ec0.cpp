// Decompiled by mimo-v2.6-pro. Names are provisional.
// Draws a shadow bitmap (or a list of nested shadow bitmaps) into `dst`, or
// into the screen when `dst` is null: the screen is then locked with
// LockScreen and unlocked with UnlockScreen at the end (which is why the
// unlock is guarded by a comparison of `dst` with the address of the local
// surface). The whole draw only happens when bit 7 of the display's byte at
// +0xf0 is set (the cached object at +0xc8 exists). A record with a child
// count draws each child by recursion. A leaf with a data pointer builds the
// source rect (0, 0, w - 1, h - 1) and the destination rect (x - dx, y - dy,
// ...), clips the destination to the destination's own clip rect with
// ClipRects and blits through FUN_004cc332 (a byte run shadow blitter), or,
// when the mode byte at +9 is set, through FUN_004cc1bf (a word run shadow
// blitter). Both blitters are hand-written assembly and take the display's
// cached object at +0xc8 as their last argument.
#include <windows.h>

struct Rect_004b8ec0 {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    char unknown_0[8];
    int field_8;
    int field_c;
    char unknown_10[0x1c - 0x10];
    Rect_004b8ec0 field_1c;             // +0x1c

    Rect_004b8ec0* GetClipRect(Rect_004b8ec0* out);
};

struct Bitmap_004b8ec0 {
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

struct Surface_004b8ec0 {
    int data[12];
};

struct Desc_004b8ec0 {
    int field_0;
    int field_4;
    int field_8;
    void* field_c;
    char unknown_10[0x30 - 0x10];
};

struct Display_004b8ec0 {
    char unknown_0[0xc8];
    int field_c8;                       // +0xc8
    char unknown_cc[0xf0 - 0xcc];
    unsigned short unknown_bits : 5;    // +0xf0, bits 0 to 4
    unsigned short has_obj_c0 : 1;      // +0xf0, bit 5
    unsigned short has_obj_c4 : 1;      // +0xf0, bit 6
    unsigned short has_obj_c8 : 1;      // +0xf0, bit 7
};

Display_004b8ec0* GetDisplay(void);
int __stdcall LockScreen(Surface_004b8ec0* out);
int __stdcall UnlockScreen(Surface_004b8ec0* s);
void __stdcall ClipRects(Rect_004b8ec0* other, Rect_004b8ec0* rect, Rect_004b8ec0* bounds);
void __cdecl FUN_004cc332(Surface* dst, Desc_004b8ec0* src, Rect_004b8ec0* srect, Rect_004b8ec0* drect, int colour, int param_6);
void __cdecl FUN_004cc1bf(int param_1, int param_2, Rect_004b8ec0* rect, void* plane, Rect_004b8ec0* other, int param_6);

// FUNCTION: 0x4b8ec0
void __stdcall DrawFrameShadow(Surface* dst, Bitmap_004b8ec0* bmp, int x, int y)
{
    Display_004b8ec0* d = GetDisplay();
    if (d->has_obj_c8 == 1) {
        Surface_004b8ec0 screen;
        if (dst == 0) {
            int ok = LockScreen(&screen);
            if (ok != 0)
                dst = (Surface*)&screen;
        }
        if (bmp != 0) {
            if (bmp->count > 0) {
                for (int i = 0; i < (int)bmp->count; i++) {
                    Bitmap_004b8ec0* e = ((Bitmap_004b8ec0**)bmp->field_10)[i];
                    DrawFrameShadow(dst, e, x, y);
                }
            } else if (bmp->field_10 != 0) {
                Rect_004b8ec0 other;
                Rect_004b8ec0 rect;
                Rect_004b8ec0 bounds;
                Desc_004b8ec0 desc;
                other.left = 0;
                other.right = bmp->width - 1;
                other.top = 0;
                other.bottom = bmp->height - 1;
                rect.left = x - bmp->dx;
                rect.top = y - bmp->dy;
                rect.right = rect.left + bmp->width - 1;
                rect.bottom = rect.top + bmp->height - 1;
                dst->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    if (bmp->flag9 == 0) {
                        desc.field_0 = bmp->width;
                        desc.field_8 = bmp->width;
                        desc.field_4 = bmp->height;
                        desc.field_c = bmp->field_10;
                        FUN_004cc332(dst, &desc, &other, &rect, bmp->colour, d->field_c8);
                    } else {
                        FUN_004cc1bf(dst->field_c, dst->field_8, &rect, bmp->field_10, &other, d->field_c8);
                    }
                }
            }
        }
        if (dst == (Surface*)&screen)
            UnlockScreen(&screen);
    }
}
