// Decompiled by mimo-v2.6-pro. Names are provisional.
// Draws one bitmap (`param_2`, a BITMAPINFO: width/height shorts, x/y origin
// shorts, two flag bytes, a texture count and a texture array) at x, y, either
// into `param_1` or, when that is null, into the screen locked with
// FUN_004c5e70 and unlocked with FUN_004c5fa0 at the end (which is why the
// unlock is guarded by a comparison of `param_1` with the address of the local
// surface). A record with a child count draws each child through this same
// function (recursion). A leaf builds the source rect (0, 0, w - 1, h - 1) and
// the destination rect (x - dx, y - dy, ...), clips the destination to the
// destination's own clip rect with FUN_004b7e60 and blits through
// FUN_004cbf2c, or, when the mode byte at +9 is set, through FUN_004cc057.

struct Rect_004b8500 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    char unknown_10[0x1c - 0x10];
    Rect_004b8500 field_1c;               // +0x1c

    Rect_004b8500* FUN_004c6ae0(Rect_004b8500* out);
};

struct Sprite_004b8500 {
    unsigned short width;                // +0x0
    unsigned short height;               // +0x2
    short dx;                            // +0x4
    short dy;                            // +0x6
    unsigned char colour;                // +0x8
    unsigned char flag_9;                // +0x9
    unsigned char count;                 // +0xa
    char unknown_b[0x10 - 0xb];
    Sprite_004b8500** items;             // +0x10
};

struct Src_004b8500 {
    int field_0;
    int field_1;
    int field_2;
    Sprite_004b8500** field_3;
};

struct Bounds_src_004b8500 {
    Rect_004b8500 bounds;
    Src_004b8500 src;
};

struct Surface_004b8500 {
    int data[12];
};

struct Screen_004b8500 {
    char unknown_0[0x20];
    Surface_004b8500 surf;                // +0x20
};

struct Display_004b8500 {
    char unknown_0[0xc0];
    unsigned char* field_c0;              // +0xc0, palette base
    char unknown_c4[0xf0 - 0xc4];
    unsigned char flags;                  // +0xf0
};

Display_004b8500* FUN_004b6220(void);
int __stdcall FUN_004c5e70(Surface_004b8500* out);
int __stdcall FUN_004c5fa0(Surface_004b8500* s);
void __stdcall FUN_004b7e60(Rect_004b8500* other, Rect_004b8500* rect, Rect_004b8500* bounds);
void __stdcall FUN_004b8500(Class_004c6ae0* p, Sprite_004b8500* s, int x, int y);
void __cdecl FUN_004cbf2c(Class_004c6ae0* p, Src_004b8500* src, Rect_004b8500* srect, Rect_004b8500* drect, int colour, unsigned char* palette);
void __cdecl FUN_004cc057(int linkid, int sprite, Rect_004b8500* drect, Sprite_004b8500** src, Rect_004b8500* srect, unsigned char* palette);

// FUNCTION: 0x4b8500
void __stdcall FUN_004b8500(Class_004c6ae0* param_1, Sprite_004b8500* param_2, int x, int y)
{
    Display_004b8500* d = FUN_004b6220();
    if ((d->flags & 0x20) != 0) {
        Screen_004b8500 screen;
        if (param_1 == 0) {
            int locked = FUN_004c5e70(&screen.surf);
            if (locked != 0)
                param_1 = (Class_004c6ae0*)&screen.surf;
        }

        if (param_2 != 0) {
            if (param_2->count > 0) {
                for (int i = 0; i < param_2->count; i++)
                    FUN_004b8500(param_1, param_2->items[i], x, y);
            } else {
                Rect_004b8500 screen_rect;
                Rect_004b8500 sprite_rect = { 0, 0, param_2->width - 1, param_2->height - 1 };
                int w = param_2->width;
                int h = param_2->height;
                screen_rect.left = x - param_2->dx;
                screen_rect.top = y - param_2->dy;
                screen_rect.right = w + screen_rect.left - 1;
                screen_rect.bottom = h + screen_rect.top - 1;
                Bounds_src_004b8500 bs;
                param_1->FUN_004c6ae0(&bs.bounds);
                FUN_004b7e60(&sprite_rect, &screen_rect, &bs.bounds);
                if (screen_rect.right >= screen_rect.left && screen_rect.bottom >= screen_rect.top
                    && sprite_rect.right >= sprite_rect.left && sprite_rect.bottom >= sprite_rect.top) {
                    if (param_2->flag_9 == 0) {
                        Src_004b8500& src = bs.src;
                        src.field_0 = param_2->width;
                        src.field_1 = param_2->height;
                        src.field_2 = param_2->width;
                        src.field_3 = param_2->items;
                        FUN_004cbf2c(param_1, &src, &sprite_rect, &screen_rect,
                            param_2->colour, d->field_c0);
                    } else {
                        FUN_004cc057(param_1->field_c, param_1->field_8, &screen_rect,
                            param_2->items, &sprite_rect, d->field_c0);
                    }
                }
            }
        }

        if (param_1 == (Class_004c6ae0*)&screen.surf)
            FUN_004c5fa0(&screen.surf);
    }
}
