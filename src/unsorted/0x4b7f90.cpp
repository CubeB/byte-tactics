// Decompiled by space-bunny-free. Names are provisional.
// Draws a bitmap (or a list of nested bitmaps) into `dst`, or into the screen
// when `dst` is null: the screen is then locked with FUN_004c5e70 and unlocked
// with FUN_004c5fa0 at the end (which is why the unlock is guarded by a
// comparison of `dst` with the address of the local surface).
// A record with a child count draws each child, through FUN_004b8500 when the
// child's kind byte is set and through this function (recursion) otherwise.
// A leaf builds the source rect (0, 0, w - 1, h - 1) and the destination rect
// (x - dx, y - dy, ...), clips the destination to the destination's own clip
// rect with FUN_004b7e60 and blits through FUN_004cbe70, or, when the mode
// byte at +9 is set, through FUN_004cc51d.
// <windows.h> is needed although nothing of it is used: without a header
// MSVC 5 picks the other base/index order for the second lea.
#include <windows.h>

struct Rect_004b7f90 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    char unknown_0[8];
    int field_8;
    int field_c;
    char unknown_10[0x1c - 0x10];
    Rect_004b7f90 field_1c;             // +0x1c

    Rect_004b7f90* FUN_004c6ae0(Rect_004b7f90* out);
};

struct Bitmap_004b7f90 {
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

struct Surface_004b7f90 {
    int data[12];
};

struct Desc_004b7f90 {
    int field_0;
    int field_4;
    int field_8;
    void* field_c;
    char unknown_10[0x30 - 0x10];
};

int __stdcall FUN_004c5e70(Surface_004b7f90* out);
int __stdcall FUN_004c5fa0(Surface_004b7f90* s);
void __stdcall FUN_004b7e60(Rect_004b7f90* other, Rect_004b7f90* rect, Rect_004b7f90* bounds);
void __cdecl FUN_004cbe70(Class_004c6ae0* dst, Desc_004b7f90* src, Rect_004b7f90* srect, Rect_004b7f90* drect, int colour);
void __cdecl FUN_004cc51d(int param_1, int param_2, Rect_004b7f90* rect, void* plane, Rect_004b7f90* other);
void __stdcall FUN_004b8500(Class_004c6ae0* dst, Bitmap_004b7f90* bmp, int x, int y);

// FUNCTION: 0x4b7f90
void __stdcall FUN_004b7f90(Class_004c6ae0* dst, Bitmap_004b7f90* bmp, int x, int y)
{
    Surface_004b7f90 screen;
    if (dst == 0) {
        int ok = FUN_004c5e70(&screen);
        if (ok != 0)
            dst = (Class_004c6ae0*)&screen;
    }
    if (bmp != 0) {
        if (bmp->count > 0) {
            for (int i = 0; i < (int)bmp->count; i++) {
                Bitmap_004b7f90* e = ((Bitmap_004b7f90**)bmp->field_10)[i];
                if (e->kind > 0)
                    FUN_004b8500(dst, e, x, y);
                else
                    FUN_004b7f90(dst, e, x, y);
            }
        } else {
            Rect_004b7f90 other;
            Rect_004b7f90 rect;
            Rect_004b7f90 bounds;
            Desc_004b7f90 desc;
            other.left = 0;
            other.right = bmp->width - 1;
            other.top = 0;
            other.bottom = bmp->height - 1;
            rect.left = x - bmp->dx;
            rect.top = y - bmp->dy;
            rect.right = rect.left + bmp->width - 1;
            rect.bottom = rect.top + bmp->height - 1;
            dst->FUN_004c6ae0(&bounds);
            FUN_004b7e60(&other, &rect, &bounds);
            if (rect.right >= rect.left && rect.bottom >= rect.top &&
                other.right >= other.left && other.bottom >= other.top) {
                if (bmp->flag9 == 0) {
                    desc.field_0 = bmp->width;
                    desc.field_8 = bmp->width;
                    desc.field_4 = bmp->height;
                    desc.field_c = bmp->field_10;
                    FUN_004cbe70(dst, &desc, &other, &rect, bmp->colour);
                } else {
                    FUN_004cc51d(dst->field_c, dst->field_8, &rect, bmp->field_10, &other);
                }
            }
        }
    }
    if (dst == (Class_004c6ae0*)&screen)
        FUN_004c5fa0(&screen);
}
