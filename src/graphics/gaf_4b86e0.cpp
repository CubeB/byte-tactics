// Decompiled by mimo-v2.6-pro. Names are provisional.
// Recursively draws a bitmap tree (`param_2`) at x, y into `param_1`, or into
// the locked screen when `param_1` is null. Runs only when the display flag
// word at +0xf0 has bit 0x100 set (which is byte +0xf1 bit 0, and that spelling
// is what fuses the read into `test byte [eax + 0xf1], 1`) and the record's
// byte at +9 is clear (note the record is tested for null only after that byte
// is read). A record with a child count
// draws every child through this same function; a leaf builds the source rect
// (0, 0, w - 1, h - 1) and the dest rect (x - dx, y - dy, ...), clips them with
// FUN_004b7e60, and when nothing is clipped off builds a 0x30-byte surface
// description from the record (width, height, pitch = width, bits, 10000, -1,
// dx, dy, flag bits), resets its clip rect with FUN_004c69c0 and blits with
// FUN_004cbfc4 using the palette at display +0xcc.
#include <windows.h>

struct Rect_004b86e0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

class Class_004c6ae0 {
public:
    char unknown_0[8];
    int field_8;
    int field_c;
    char unknown_10[0x1c - 0x10];
    Rect_004b86e0 field_1c;             // +0x1c

    Rect_004b86e0* FUN_004c6ae0(Rect_004b86e0* out);
};

struct Bitmap_004b86e0 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
    short dx;                           // +0x4
    short dy;                           // +0x6
    unsigned char colour;               // +0x8
    unsigned char flag9;                // +0x9
    unsigned char count;                // +0xa
    unsigned char kind;                 // +0xb
    int unknown_c;                      // +0xc
    Bitmap_004b86e0** items;            // +0x10
};

struct Surface_004b86e0 {
    int width;                          // +0x0
    int height;                         // +0x4
    int pitch;                          // +0x8
    int bits;                           // +0xc
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    unsigned short x;                   // +0x18
    unsigned short y;                   // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;             // +0x2c bit 0
    unsigned int flag1 : 1;             // +0x2c bit 1
};

struct Display_004b86e0 {
    char unknown_0[0xcc];
    unsigned char* field_cc;            // +0xcc, palette base
    char unknown_d0[0xf0 - 0xd0];
    unsigned short flags;               // +0xf0, bit 0x100 = "may draw"
};

Display_004b86e0* FUN_004b6220(void);
int __stdcall FUN_004c5e70(Surface_004b86e0* out);
int __stdcall FUN_004c5fa0(Surface_004b86e0* s);
void __stdcall FUN_004b7e60(Rect_004b86e0* other, Rect_004b86e0* rect, Rect_004b86e0* bounds);
void __stdcall FUN_004c69c0(int* param_1);
void __cdecl FUN_004cbfc4(Class_004c6ae0* p, Surface_004b86e0* s, Rect_004b86e0* srect,
                          Rect_004b86e0* drect, int colour, unsigned char* palette);
void __stdcall FUN_004b86e0(Class_004c6ae0* param_1, Bitmap_004b86e0* param_2, int x, int y);

// FUNCTION: 0x4b86e0
void __stdcall FUN_004b86e0(Class_004c6ae0* param_1, Bitmap_004b86e0* param_2, int x, int y)
{
    Display_004b86e0* d = FUN_004b6220();
    if ((d->flags & 0x100) != 0 && param_2->flag9 == 0) {
        Surface_004b86e0 screen;
        if (param_1 == 0) {
            int locked = FUN_004c5e70(&screen);
            if (locked != 0)
                param_1 = (Class_004c6ae0*)&screen;
        }
        if (param_2 != 0) {
            if (param_2->count > 0) {
                for (int i = 0; i < param_2->count; i++)
                    FUN_004b86e0(param_1, param_2->items[i], x, y);
            } else {
                Rect_004b86e0 other;
                Rect_004b86e0 rect;
                other.left = 0;
                other.right = param_2->width - 1;
                other.top = 0;
                other.bottom = param_2->height - 1;
                rect.left = x - param_2->dx;
                rect.top = y - param_2->dy;
                rect.right = rect.left + param_2->width - 1;
                rect.bottom = rect.top + param_2->height - 1;
                Rect_004b86e0 bounds;
                param_1->FUN_004c6ae0(&bounds);
                FUN_004b7e60(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    Surface_004b86e0 s;
                    s.width = param_2->width;
                    s.height = param_2->height;
                    s.pitch = param_2->width;
                    s.bits = (int)param_2->items;
                    s.field_10 = 10000;
                    s.field_14 = -1;
                    s.x = param_2->dx;
                    s.y = param_2->dy;
                    s.flag0 = 1;
                    s.flag1 = 0;
                    FUN_004c69c0((int*)&s);
                    FUN_004cbfc4(param_1, &s, &other, &rect, param_2->colour, d->field_cc);
                }
            }
        }
        if (param_1 == (Class_004c6ae0*)&screen)
            FUN_004c5fa0(&screen);
    }
}
