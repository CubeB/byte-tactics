// Decompiled by space-bunny-free. Names are provisional.
// Draws one bitmap (`param_2`, a BITMAPINFO: width/height shorts, x/y origin
// shorts, two flag bytes, a texture count and a texture array) at x, y, either
// into `param_1` or, when that is null, into the locked screen. When the
// bitmap has more than one texture every texture is drawn with DrawFrameBlended,
// which is this function again for the next level down.
#include <ddraw.h>

struct Rect_004b8310 {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    char unknown_10[0x1c - 0x10];
    Rect_004b8310 field_1c;               // +0x1c

    Rect_004b8310* GetClipRect(Rect_004b8310* out);
};

struct Sprite_004b8310 {
    unsigned short width;                // +0x0
    unsigned short height;               // +0x2
    short dx;                            // +0x4
    short dy;                            // +0x6
    unsigned char flag_8;                // +0x8
    unsigned char flag_9;                // +0x9
    unsigned char count;                 // +0xa
    char unknown_b[0x10 - 0xb];
    Sprite_004b8310** items;             // +0x10
};

struct Src_004b8310 {
    int field_0;
    int field_1;
    int field_2;
    Sprite_004b8310** field_3;
};

struct Bounds_src_004b8310 {
    Rect_004b8310 bounds;
    Src_004b8310 src;
};

struct Surface_004b8310 {
    int data[12];
};

struct Screen_004b8310 {
    char unknown_0[0x20];
    Surface_004b8310 surf;                // +0x20
};

struct Display_004b8310 {
    char unknown_0[0xc8];
    unsigned char* field_c8;             // +0xc8, palette base
    char unknown_cc[0xf0 - 0xcc];
    unsigned char flags;                 // +0xf0
};

Display_004b8310* GetDisplay(void);
int __stdcall LockScreen(Surface_004b8310* out);
int __stdcall UnlockScreen(Surface_004b8310* s);
void __stdcall ClipRects(Rect_004b8310* other, Rect_004b8310* rect, Rect_004b8310* bounds);
void __stdcall DrawFrameBlended(Surface* p, Sprite_004b8310* s, int x, int y);
void __cdecl FUN_004cbf2c(Surface* p, Src_004b8310* src, Rect_004b8310* srect, Rect_004b8310* drect, int colour, unsigned char* palette);
void __cdecl BlitCompressedLit(int linkid, int sprite, Rect_004b8310* drect, Sprite_004b8310** src, Rect_004b8310* srect, unsigned char* palette);

// FUNCTION: 0x4b8310
void __stdcall DrawFrameLit(Surface* param_1, Sprite_004b8310* param_2, int x, int y, int param_5)
{
    Display_004b8310* d = GetDisplay();
    if ((d->flags & 0x80) == 0x80) {
        // Surface stays a member at +0x20 of a bigger local: sets the frame layout.
        Screen_004b8310 screen;
        if (param_1 == 0) {
            int locked = LockScreen(&screen.surf);
            if (locked != 0)
                param_1 = (Surface*)&screen.surf;
        }

        if (param_2 != 0) {
            if (param_2->count > 0) {
                for (int i = 0; i < param_2->count; i++)
                    DrawFrameBlended(param_1, param_2->items[i], x, y);
            } else {
                Rect_004b8310 screen_rect;
                // Aggregate initialiser: orders the stores and the rect stack slots.
                Rect_004b8310 sprite_rect = { 0, 0, param_2->width - 1, param_2->height - 1 };
                int w = param_2->width;
                int h = param_2->height;
                screen_rect.left = x - param_2->dx;
                screen_rect.top = y - param_2->dy;
                screen_rect.right = w + screen_rect.left - 1;
                screen_rect.bottom = h + screen_rect.top - 1;
                Bounds_src_004b8310 bs;
                param_1->GetClipRect(&bs.bounds);
                ClipRects(&sprite_rect, &screen_rect, &bs.bounds);
                if (screen_rect.right >= screen_rect.left && screen_rect.bottom >= screen_rect.top
                    && sprite_rect.right >= sprite_rect.left && sprite_rect.bottom >= sprite_rect.top) {
                    if (param_2->flag_9 == 0) {
                        Src_004b8310& src = bs.src;
                        src.field_0 = param_2->width;
                        src.field_1 = param_2->height;
                        src.field_2 = param_2->width;
                        src.field_3 = param_2->items;
                        FUN_004cbf2c(param_1, &src, &sprite_rect, &screen_rect, param_5, d->field_c8);
                    } else {
                        BlitCompressedLit(param_1->field_c, param_1->field_8, &screen_rect,
                            param_2->items, &sprite_rect, d->field_c8 + (param_5 << 8));
                    }
                }
            }
        }

        if (param_1 == (Surface*)&screen.surf)
            UnlockScreen(&screen.surf);
    }
}
