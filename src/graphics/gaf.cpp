// Decompiled by Opus, Sonnet, space-bunny-free, mimo-v2.6-pro, deepseek-v4.1-flash,
// Claude Sonnet 5.5, GPT-6.1-sol, GPT-6, Claude Opus 5.5, Space Bunny Free,
// claude-opus-5-5, Sonnet 5.5 and claude-sonnet-5-5. Names are provisional.

// The gaf module: the GAF frame trees and their drawing (keyed, opaque, lit,
// blended, grey, shadowed, dithered, depth and scaled), the frame allocators,
// the sequence references, the lens and depth frames, the image helpers and
// the row compression. DrawFrameLit (0x4b8310), DrawFrameDepth (0x4b90a0) and
// DownsampleFrame (0x4b95a0) are joined at the end, out of address order.
// <windows.h> is needed even though no function here calls it: its
// declaration count sets the register windows of 0x4b9360 and 0x4b9740.
#include <windows.h>
#include <string.h>
#include <math.h>

// A 16-byte rectangle: clip bounds and blit rectangles.
struct Rect_004b7e60 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// Clips `rect` to `bounds`, moving `other` by the same amounts.
// FUNCTION: 0x4b7e60
void __stdcall ClipRects(Rect_004b7e60* other, Rect_004b7e60* rect, Rect_004b7e60* bounds)
{
    int d;
    d = rect->left - bounds->left;
    if (d < 0) {
        other->left -= d;
        rect->left -= d;
    }
    d = rect->right - bounds->right;
    if (d > 0) {
        other->right -= d;
        rect->right -= d;
    }
    d = rect->top - bounds->top;
    if (d < 0) {
        other->top -= d;
        rect->top -= d;
    }
    d = rect->bottom - bounds->bottom;
    if (d > 0) {
        other->bottom -= d;
        rect->bottom -= d;
    }
}

struct Entry_004b7ee0 {
    int value;                         // +0x0
    int unknown_4;
};

struct Table_004b7ee0 {
    unsigned short count;              // +0x0
    char unknown_2[0x26];
    Entry_004b7ee0 entries[1];         // +0x28
};

struct Handle_004b7ee0 {
    unsigned short index;              // +0x0
    char unknown_2[6];
    Table_004b7ee0* table;             // +0x8
};

// Looks up an entry of the table that GetGafFrame indexes (count at +0,
// 8-byte entries from +0x28) through a handle holding an index and the table.
// FUNCTION: 0x4b7ee0
int __stdcall GetGafSequenceFrame(Handle_004b7ee0* h)
{
    int result = 0;
    if (h->table != 0)
        result = h->table->entries[h->index].value;
    return result;
}

struct Obj_004b7f00 {
    unsigned short index;              // +0x0
    char unknown_2[6];
    unsigned short* table;             // +0x8, starts with the entry count
};
// FUNCTION: 0x4b7f00
int __stdcall SetGafSequenceFrame(Obj_004b7f00* obj, int index)
{
    int result = 0;
    if (obj->table != 0 && *obj->table > index) {
        obj->index = index;
        result = 1;
    }
    return result;
}
// FUNCTION: 0x4b7f30
int __stdcall GetGafFrame(unsigned short* param_1, int param_2)
{
    int result = 0;
    if (param_2 >= 0 && param_2 < (int)*param_1 && param_1 != 0) {
        result = *(int*)((char*)param_1 + param_2 * 8 + 0x28);
    }
    return result;
}
// FUNCTION: 0x4b7f60
unsigned short __stdcall GetGafFrameCount(void* ptr)
{
    unsigned int result = 0;
    result = *(unsigned short*)ptr;
    return (unsigned short)result;
}

struct Entry_004b7f70 {
    unsigned short value;
    char unknown_2[6];
};

struct Obj_004b7f70 {
    unsigned short index;
    char unknown_2[6];
    void* table;
};

static inline Entry_004b7f70* GetEntries(Obj_004b7f70* obj)
{
    return (Entry_004b7f70*)((char*)obj->table + 0x2c);
}
// FUNCTION: 0x4b7f70
unsigned short __stdcall GetGafFrameDuration(Obj_004b7f70* param1)
{
    unsigned short result;
    if (param1->table != 0) {
        result = GetEntries(param1)[param1->index].value;
    } else {
        result = 0xffff;
    }
    return result;
}

// A 0x30-byte surface descriptor: an allocated image's header, the screen
// lock's output, and the blitters' source and destination. Its pixels follow
// at +0x30. GetClipRect is defined in surface.cpp.
struct Surface {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int zPriority;                     // +0x10
    int colorKey;                      // +0x14
    unsigned short x;                  // +0x18
    unsigned short y;                  // +0x1a
    Rect_004b7e60 clip;                // +0x1c
    unsigned int flag0 : 1;            // +0x2c bit 0
    unsigned int flag1 : 1;            // +0x2c bit 1

    Rect_004b7e60* GetClipRect(Rect_004b7e60* out);
};

// A GAF frame header: its size and origin, the colour key, the two flag
// bytes, the child count, and the pointers to its one or two pixel planes.
struct GafFrame {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short x;                           // +0x4
    short y;                           // +0x6
    unsigned char colour;              // +0x8
    unsigned char flag9;               // +0x9
    unsigned char count;               // +0xa
    unsigned char kind;                // +0xb
    int reserved;                      // +0xc
    unsigned char* plane0;             // +0x10
    unsigned char* plane1;             // +0x14
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
void __cdecl BlitRectKeyed(Surface* dst, Surface* src, Rect_004b7e60* rect,
                           Rect_004b7e60* pos, int colour);
void __cdecl BlitRect(Surface* dst, Surface* src, Rect_004b7e60* rect,
                      Rect_004b7e60* pos);
void __cdecl BlitCompressed(unsigned char* pixels, int pitch, Rect_004b7e60* rect,
                            void* plane, Rect_004b7e60* other);
void __stdcall DrawFrameBlended(Surface* dst, GafFrame* bmp, int x, int y);

// Draws a bitmap (or a list of nested bitmaps) into `dst`, or into the screen
// when `dst` is null: the screen is then locked with LockScreen and unlocked
// with UnlockScreen at the end (which is why the unlock is guarded by a
// comparison of `dst` with the address of the local surface).
// A record with a child count draws each child, through DrawFrameBlended when the
// child's kind byte is set and through this function (recursion) otherwise.
// A leaf builds the source rect (0, 0, w - 1, h - 1) and the destination rect
// (x - dx, y - dy, ...), clips the destination to the destination's own clip
// rect with ClipRects and blits through BlitRectKeyed, or, when the mode
// byte at +9 is set, through BlitCompressed.
// FUNCTION: 0x4b7f90
void __stdcall DrawFrame(Surface* dst, GafFrame* bmp, int x, int y)
{
    Surface screen;
    if (dst == 0) {
        int ok = LockScreen(&screen);
        if (ok != 0)
            dst = (Surface*)&screen;
    }
    if (bmp != 0) {
        if (bmp->count > 0) {
            for (int i = 0; i < (int)bmp->count; i++) {
                GafFrame* e = ((GafFrame**)bmp->plane0)[i];
                if (e->kind > 0)
                    DrawFrameBlended(dst, e, x, y);
                else
                    DrawFrame(dst, e, x, y);
            }
        } else {
            Rect_004b7e60 other;
            Rect_004b7e60 rect;
            Rect_004b7e60 bounds;
            Surface desc;
            other.left = 0;
            other.right = bmp->width - 1;
            other.top = 0;
            other.bottom = bmp->height - 1;
            rect.left = x - bmp->x;
            rect.top = y - bmp->y;
            rect.right = rect.left + bmp->width - 1;
            rect.bottom = rect.top + bmp->height - 1;
            dst->GetClipRect(&bounds);
            ClipRects(&other, &rect, &bounds);
            if (rect.right >= rect.left && rect.bottom >= rect.top &&
                other.right >= other.left && other.bottom >= other.top) {
                if (bmp->flag9 == 0) {
                    desc.width = bmp->width;
                    desc.pitch = bmp->width;
                    desc.height = bmp->height;
                    desc.pixels = bmp->plane0;
                    BlitRectKeyed(dst, &desc, &other, &rect, bmp->colour);
                } else {
                    BlitCompressed(dst->pixels, dst->pitch, &rect, bmp->plane0, &other);
                }
            }
        }
    }
    if (dst == (Surface*)&screen)
        UnlockScreen(&screen);
}

// Draws a bitmap (or a list of nested bitmaps) into `dst`, or into the screen
// when `dst` is null: the screen is then locked with LockScreen and unlocked
// with UnlockScreen at the end (which is why the unlock is guarded by a
// comparison of `dst` with the address of the local surface).
// A record with a child count draws each child, through DrawFrameBlended when the
// child's kind byte is set and through DrawFrame otherwise (the same code as
// here, but the one that blits with BlitRectKeyed).
// A leaf builds the source rect (0, 0, w - 1, h - 1) and the destination rect
// (x - dx, y - dy, ...), clips the destination to the destination's own clip
// rect with ClipRects and blits through the hand-written BlitRect, or,
// when the mode byte at +9 is set, through BlitCompressed.
// FUNCTION: 0x4b8150
void __stdcall DrawFrameOpaque(Surface* dst, GafFrame* bmp, int x, int y)
{
    Surface screen;
    if (dst == 0) {
        int locked = LockScreen(&screen);
        if (locked != 0)
            dst = (Surface*)&screen;
    }
    if (bmp != 0) {
        if (bmp->count > 0) {
            for (int i = 0; i < (int)bmp->count; i++) {
                GafFrame* e = ((GafFrame**)bmp->plane0)[i];
                if (e->kind > 0)
                    DrawFrameBlended(dst, e, x, y);
                else
                    DrawFrame(dst, e, x, y);
            }
        } else {
            Rect_004b7e60 other;
            Rect_004b7e60 rect;
            Rect_004b7e60 bounds;
            Surface desc;
            other.left = 0;
            other.right = bmp->width - 1;
            other.top = 0;
            other.bottom = bmp->height - 1;
            rect.left = x - bmp->x;
            rect.top = y - bmp->y;
            rect.right = rect.left + bmp->width - 1;
            rect.bottom = rect.top + bmp->height - 1;
            dst->GetClipRect(&bounds);
            ClipRects(&other, &rect, &bounds);
            if (rect.right >= rect.left && rect.bottom >= rect.top &&
                other.right >= other.left && other.bottom >= other.top) {
                if (bmp->flag9 == 0) {
                    desc.width = bmp->width;
                    desc.pitch = bmp->width;
                    desc.height = bmp->height;
                    desc.pixels = bmp->plane0;
                    BlitRect(dst, &desc, &other, &rect);
                } else {
                    BlitCompressed(dst->pixels, dst->pitch, &rect, bmp->plane0, &other);
                }
            }
        }
    }
    if (dst == (Surface*)&screen)
        UnlockScreen(&screen);
}

// +0xf0: the display object's state flags, read as a byte, a word or a
// bitfield. Bit 7 (has_c8) is the cached object the shadow draw needs.
union Flags_004b8310 {
    unsigned short word;               // +0xf0
    unsigned char byte;                // +0xf0
    struct {
        unsigned short bit0 : 1;
        unsigned short bit1 : 1;
        unsigned short bit2 : 1;
        unsigned short bit3 : 1;
        unsigned short bit4 : 1;
        unsigned short has_obj_c0 : 1; // bit 5
        unsigned short has_obj_c4 : 1; // bit 6
        unsigned short has_obj_c8 : 1; // bit 7
        unsigned short bit8_15 : 8;
    } bits;
};

// The display object GetDisplay returns. Only the fields the gaf drawing
// reads are named: the alpha, shade, light, gray and blue tables at +0xc0 to
// +0xd0, and the flag word at +0xf0.
struct Display_004b8310 {
    char unknown_0[0xc0];
    unsigned char* alphaTable;         // +0xc0
    unsigned char* shadeTable;         // +0xc4
    unsigned char* lightTable;         // +0xc8
    unsigned char* grayTable;          // +0xcc
    unsigned char* blueTable;          // +0xd0
    char unknown_d4[0xf0 - 0xd4];
    Flags_004b8310 flags;              // +0xf0
};

// The locked screen as a member at +0x20 of a bigger local: sets the frame
// layout of the functions that lock the screen.
struct Screen_004b8310 {
    char unknown_0[0x20];
    Surface surf;                      // +0x20
};

// The 0x10-byte source descriptor the blended blitters read: width, height,
// pitch and the bits pointer.
struct Src_004b8310 {
    int width;
    int height;
    int pitch;
    GafFrame** pixels;
};

// The clip rect and the source descriptor DrawFrameBlended builds together.
struct Bounds_src_004b8310 {
    Rect_004b7e60 bounds;
    Src_004b8310 src;
};

Display_004b8310* GetDisplay();
void __cdecl BlitRectBlended(Surface* p, Src_004b8310* src, Rect_004b7e60* rect,
                             Rect_004b7e60* pos, int colour, unsigned char* palette);
void __cdecl BlitCompressedLit(unsigned char* pixels, int pitch, Rect_004b7e60* drect,
                               GafFrame** src, Rect_004b7e60* srect,
                               unsigned char* palette);
void __cdecl BlitCompressedBlended(unsigned char* pixels, int pitch, Rect_004b7e60* drect,
                                   GafFrame** src, Rect_004b7e60* srect,
                                   unsigned char* palette);

// Draws one bitmap (`param_2`, a BITMAPINFO: width/height shorts, x/y origin
// shorts, two flag bytes, a texture count and a texture array) at x, y, either
// into `param_1` or, when that is null, into the screen locked with
// LockScreen and unlocked with UnlockScreen at the end (which is why the
// unlock is guarded by a comparison of `param_1` with the address of the local
// surface). A record with a child count draws each child through this same
// function (recursion). A leaf builds the source rect (0, 0, w - 1, h - 1) and
// the destination rect (x - dx, y - dy, ...), clips the destination to the
// destination's own clip rect with ClipRects and blits through
// BlitRectBlended, or, when the mode byte at +9 is set, through BlitCompressedBlended.
// FUNCTION: 0x4b8500
void __stdcall DrawFrameBlended(Surface* param_1, GafFrame* param_2, int x, int y)
{
    Display_004b8310* d = GetDisplay();
    if ((d->flags.byte & 0x20) != 0) {
        Screen_004b8310 screen;
        if (param_1 == 0) {
            int locked = LockScreen(&screen.surf);
            if (locked != 0)
                param_1 = (Surface*)&screen.surf;
        }

        if (param_2 != 0) {
            if (param_2->count > 0) {
                for (int i = 0; i < param_2->count; i++)
                    DrawFrameBlended(param_1, ((GafFrame**)param_2->plane0)[i], x, y);
            } else {
                Rect_004b7e60 screen_rect;
                Rect_004b7e60 sprite_rect = { 0, 0, param_2->width - 1, param_2->height - 1 };
                int w = param_2->width;
                int h = param_2->height;
                screen_rect.left = x - param_2->x;
                screen_rect.top = y - param_2->y;
                screen_rect.right = w + screen_rect.left - 1;
                screen_rect.bottom = h + screen_rect.top - 1;
                Bounds_src_004b8310 bs;
                param_1->GetClipRect(&bs.bounds);
                ClipRects(&sprite_rect, &screen_rect, &bs.bounds);
                if (screen_rect.right >= screen_rect.left && screen_rect.bottom >= screen_rect.top
                    && sprite_rect.right >= sprite_rect.left && sprite_rect.bottom >= sprite_rect.top) {
                    if (param_2->flag9 == 0) {
                        Src_004b8310& src = bs.src;
                        src.width = param_2->width;
                        src.height = param_2->height;
                        src.pitch = param_2->width;
                        src.pixels = (GafFrame**)param_2->plane0;
                        BlitRectBlended(param_1, &src, &sprite_rect, &screen_rect,
                            param_2->colour, d->alphaTable);
                    } else {
                        BlitCompressedBlended(param_1->pixels, param_1->pitch, &screen_rect,
                            (GafFrame**)param_2->plane0, &sprite_rect, d->alphaTable);
                    }
                }
            }
        }

        if (param_1 == (Surface*)&screen.surf)
            UnlockScreen(&screen.surf);
    }
}

void __cdecl BlitRectRemapDest(Surface* dst, Surface* src, Rect_004b7e60* rect,
                               Rect_004b7e60* pos, int colour, unsigned char* table);
void __stdcall ResetClipRect(int* param_1);

// Recursively draws a bitmap tree (`param_2`) at x, y into `param_1`, or into
// the locked screen when `param_1` is null. Runs only when the display flag
// word at +0xf0 has bit 0x100 set (which is byte +0xf1 bit 0) and the record's
// byte at +9 is clear (note the record is tested for null only after that byte
// is read). A record with a child count
// draws every child through this same function; a leaf builds the source rect
// (0, 0, w - 1, h - 1) and the dest rect (x - dx, y - dy, ...), clips them with
// ClipRects, and when nothing is clipped off builds a 0x30-byte surface
// description from the record (width, height, pitch = width, bits, 10000, -1,
// dx, dy, flag bits), resets its clip rect with ResetClipRect and blits with
// BlitRectRemapDest using the palette at display +0xcc.
// FUNCTION: 0x4b86e0
void __stdcall DrawFrameGray(Surface* param_1, GafFrame* param_2, int x, int y)
{
    Display_004b8310* d = GetDisplay();
    // Flag tested as bit 0x100 of the word: fuses into a byte test at +0xf1.
    if ((d->flags.word & 0x100) != 0 && param_2->flag9 == 0) {
        Surface screen;
        if (param_1 == 0) {
            int locked = LockScreen(&screen);
            if (locked != 0)
                param_1 = (Surface*)&screen;
        }
        if (param_2 != 0) {
            if (param_2->count > 0) {
                for (int i = 0; i < param_2->count; i++)
                    DrawFrameGray(param_1, ((GafFrame**)param_2->plane0)[i], x, y);
            } else {
                Rect_004b7e60 other;
                Rect_004b7e60 rect;
                other.left = 0;
                other.right = param_2->width - 1;
                other.top = 0;
                other.bottom = param_2->height - 1;
                rect.left = x - param_2->x;
                rect.top = y - param_2->y;
                rect.right = rect.left + param_2->width - 1;
                rect.bottom = rect.top + param_2->height - 1;
                Rect_004b7e60 bounds;
                param_1->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    Surface s;
                    s.width = param_2->width;
                    s.height = param_2->height;
                    s.pitch = param_2->width;
                    s.pixels = (unsigned char*)param_2->plane0;
                    s.zPriority = 10000;
                    s.colorKey = -1;
                    s.x = param_2->x;
                    s.y = param_2->y;
                    s.flag0 = 1;
                    s.flag1 = 0;
                    ResetClipRect((int*)&s);
                    BlitRectRemapDest(param_1, &s, &other, &rect, param_2->colour, d->grayTable);
                }
            }
        }
        if (param_1 == (Surface*)&screen)
            UnlockScreen(&screen);
    }
}

// Checkerboard eraser: clears destination pixels to 0 where a bitmap's
// pixels differ from its transparent colour, stepping by two so only one
// parity of the checkerboard is touched (parity comes from the last
// argument). Like 0x4b7f90 it draws a bitmap (or a list of nested
// bitmaps) into `dst`, or into the screen when `dst` is null: the screen
// is then locked with LockScreen and unlocked with UnlockScreen at the
// end (which is why the unlock is guarded by a comparison of `dst` with
// the address of the local surface). The whole body is skipped when the
// byte at +9 is set. Suspected original bug: that test dereferences the
// record (byte at +9) before the later `bmp != 0` check on the same
// pointer, so a null record would crash before the null check. A record
// with a child count draws each child through this function (recursion).
// A leaf builds the source rect (0, 0, w - 1, h - 1) and the destination
// rect (x - dx, y - dy, ...), clips the destination to the destination's
// own clip rect with ClipRects, then walks the clipped rows and
// columns.
// FUNCTION: 0x4b88d0
void __stdcall EraseFrameDithered(Surface* dst, GafFrame* bmp, int x, int y, int parity)
{
    Surface screen;
    if (bmp->flag9 == 0) {
        if (dst == 0) {
            int ok = LockScreen(&screen);
            if (ok != 0)
                dst = (Surface*)&screen;
        }
        if (bmp != 0) {
            if (bmp->count > 0) {
                for (int i = 0; i < (int)bmp->count; i++) {
                    GafFrame* e = ((GafFrame**)bmp->plane0)[i];
                    EraseFrameDithered(dst, e, x, y, parity);
                }
            } else {
                Rect_004b7e60 rect;
                Rect_004b7e60 other;
                Rect_004b7e60 bounds;
                other.left = 0;
                other.top = 0;
                other.right = bmp->width - 1;
                other.bottom = bmp->height - 1;
                rect.left = x - bmp->x;
                rect.top = y - bmp->y;
                rect.right = rect.left + bmp->width - 1;
                rect.bottom = rect.top + bmp->height - 1;
                dst->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    // `sy` must be a plain local here, not other.top++: pins stack slots and loop order.
                    for (int yy = rect.top, sy = other.top; yy <= rect.bottom; yy++, sy++) {
                        unsigned char* s = (unsigned char*)bmp->plane0
                            + sy * bmp->width + other.left;
                        unsigned char* d = dst->pixels
                            + yy * dst->pitch + rect.left;
                        unsigned char* end = d + rect.right - rect.left;
                        if (((yy + rect.left + parity) & 1) != 0) {
                            d++;
                            s++;
                        }
                        while (d <= end) {
                            if (*s != bmp->colour)
                                *d = 0;
                            d += 2;
                            s += 2;
                        }
                    }
                }
            }
        }
        if (dst == (Surface*)&screen)
            UnlockScreen(&screen);
    }
}

// Unused here: real functions declared to keep the file's symbol count.
void __stdcall SetCameraPosition(int x, int y, int z);
void __stdcall RecalculateLineOfSight(int param);
void __stdcall CollectVisibleUnitIds();

// Initialises a drawing surface description from a frame: width, height,
// pitch (= width) and pixels, then resets its clip rectangle through
// ResetClipRect.
// FUNCTION: 0x4b8a80
void __stdcall SurfaceFromFrame(Surface* dst, GafFrame* src)
{
    dst->width = src->width;
    dst->height = src->height;
    dst->pitch = src->width;
    dst->pixels = src->plane0;
    dst->zPriority = 10000;
    dst->colorKey = -1;
    dst->x = src->x;
    dst->y = src->y;
    dst->flag1 = 0;
    dst->flag0 = 1;
    ResetClipRect((int*)dst);
}

// A 0x14-byte surface as 0x4b8ae0 reads it: size, pitch, bits and the two
// origin words.
struct Src_004b8ae0 {
    unsigned short a;               // +0x0
    char unknown_2[2];
    unsigned short b;               // +0x4
    char unknown_6[2];
    unsigned short c;               // +0x8
    char unknown_a[2];
    int d;                          // +0xc
    char unknown_10[8];
    unsigned short e;               // +0x18
    unsigned short f;               // +0x1a
};

// Unused here: real functions declared to keep the file's symbol count.
void __stdcall DrawBattleFrame(int param_1, int param_2);
void __stdcall HAPI_FindClose(int handle);
void __stdcall InstallOutOfMemoryHandler();

// Initialises a frame from a surface description. The width is written
// twice in the original (first the surface's width, then its pitch).
// FUNCTION: 0x4b8ae0
void __stdcall FrameFromSurface(GafFrame* dst, Src_004b8ae0* src)
{
    dst->width = src->a;
    dst->height = src->b;
    dst->width = src->c;
    dst->plane0 = (unsigned char*)src->d;
    dst->x = src->e;
    dst->y = src->f;
    dst->flag9 = 0;
    dst->colour = 0xff;
    dst->count = 0;
    dst->kind = 0;
}

// One 8-byte entry of a GAF sequence: the frame's timer value.
struct Entry_004b8b30 {
    unsigned short value;
    char unknown_2[6];
};

// A GAF sequence: its entry count, the loop flag and the 8-byte entries.
struct Src_004b8b30 {
    unsigned short count;           // +0x0
    unsigned char kind;             // +0x2
    char unknown_3[0x2c - 3];
    Entry_004b8b30 entries[1];      // +0x2c
};

// A reference into a sequence: the current entry, its timer value, the loop
// flag and the sequence itself.
struct Ref_004b8b30 {
    unsigned short index;           // +0x0
    unsigned short value;           // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    Src_004b8b30* src;              // +0x8
};

static inline unsigned short Value_004b8b30(Src_004b8b30* s, unsigned short i)
{
    if (s)
        return s->entries[i].value;
    return 0xffff;
}
// FUNCTION: 0x4b8b30
void __stdcall InitGafSequence(Ref_004b8b30* ref, Src_004b8b30* src, int index)
{
    ref->index = index < src->count ? index : 0;
    ref->src = src;
    ref->value = Value_004b8b30(src, ref->index);
    ref->kind = src->kind;
}

// Same frame-sequence layout as 0x4b8b30: counts the current entry's timer
// down, and when it runs out moves to the next entry (wrapping around, or
// dropping the sequence when it doesn't loop). Returns 1 when the entry
// changed.
// FUNCTION: 0x4b8b90
int __stdcall StepGafSequence(Ref_004b8b30* ref)
{
    Src_004b8b30* src = ref->src;
    if (src) {
        if (ref->value < 2) {
            ref->index++;
            if (ref->index >= src->count) {
                if (ref->kind) {
                    ref->index = 0;
                } else {
                    ref->src = 0;
                    return 1;
                }
            }
            ref->value = src->entries[ref->index].value;
            return 1;
        }
        ref->value--;
    }
    return 0;
}

// The advancing view of a sequence reference: the timer value is signed here,
// so a step can take it below zero.
struct Ref_004b8bf0 {
    unsigned short index;           // +0x0
    short value;                    // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    Src_004b8b30* src;              // +0x8
};

static inline unsigned short Value_004b8bf0(Ref_004b8bf0* r)
{
    if (r->src)
        return r->src->entries[r->index].value;
    return 0xffff;
}

// Same frame-sequence layout as 0x4b8b30: advances the reference by `step`
// time units, moving to the next entry whenever its countdown runs out and
// wrapping around (or dropping the sequence when it doesn't loop).
// FUNCTION: 0x4b8bf0
void __stdcall AdvanceGafSequence(Ref_004b8bf0* ref, short step)
{
    Src_004b8b30* src = ref->src;
    if (src == 0 || src->count <= 1) {
        return;
    }
    ref->value -= step;
    while (ref->value <= 0) {
        ref->index++;
        if (ref->index >= src->count) {
            if (!ref->kind) {
                ref->src = 0;
                return;
            }
            ref->index = 0;
        }
        ref->value += Value_004b8bf0(ref);
    }
}

void* __stdcall HAPI_LoadFile(char* name, int flags);

// A block of the root object: a count, a sub-count, then a run of 8-byte
// entries whose first int is an offset that is rebased in place.
struct Entry_004b8c60 {
    int off;
    int pad;
};
struct Blk {
    unsigned short count;
    unsigned char pad1[8];
    unsigned char subcount;
    unsigned char pad2[29];
    Entry_004b8c60 e[2];
};
// FUNCTION: 0x4b8c60
void* __stdcall LoadGaf(char* name)
{
    int* base = (int*)HAPI_LoadFile(name, 0);
    if (base == 0)
        return 0;

    for (int i = 0; i < (short)base[1]; i++) {
        Blk* p = (Blk*)((char*)base + base[3 + i]);
        base[3 + i] = (int)p;
        int j = 0;
        if (p->count > 0) {
            do {
                int* q = (int*)((char*)base + p->e[j].off);
                p->e[j].off = (int)q;
                q[4] = q[4] + (int)base;
                if (((unsigned char*)q)[0xa] > 0) {
                    for (int k = 0; k < (int)((unsigned char*)q)[0xa]; k++) {
                        ((int*)q[4])[k] = ((int*)q[4])[k] + (int)base;
                        int* s = (int*)((int*)q[4])[k];
                        s[4] = s[4] + (int)base;
                    }
                }
                j++;
            } while (j < (int)p->count);
        }
    }
    return base;
}

// A GAF entry as FindGafEntry reads it: its name follows the 8-byte header.
struct GafEntry_004b8d40 {
    char unknown_0[8];
    char name[1];                      // +0x8
};

// The loaded GAF file: its entry count and the entry pointers.
struct Gaf_004b8d40 {
    char unknown_0[4];
    short count;                       // +0x4
    char unknown_6[6];
    GafEntry_004b8d40* entries[1];     // +0xc
};

// Looks up a GAF entry by name (case-insensitive); returns 0 when the file is
// missing or no entry has that name.
// FUNCTION: 0x4b8d40
GafEntry_004b8d40* __stdcall FindGafEntry(Gaf_004b8d40* gaf, const char* name)
{
    if (gaf) {
        GafEntry_004b8d40** p = gaf->entries;
        for (int i = 0; i < gaf->count; i++, p++) {
            if (_strcmpi((*p)->name, name) == 0)
                return *p;
        }
    }
    return 0;
}

void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
// FUNCTION: 0x4b8da0
GafFrame* __stdcall AllocFrame(const char* name, int width, int height)
{
    GafFrame* p = (GafFrame*)GameAllocIgnoreTag(name, height * width + 0x18);
    if (p == 0) {
        return 0;
    }
    p->width = width;
    p->height = height;
    p->plane0 = (unsigned char*)(p + 1);
    p->plane1 = 0;
    p->x = 0;
    p->y = 0;
    p->flag9 = 0;
    p->count = 0;
    p->kind = 0;
    return p;
}
// FUNCTION: 0x4b8e00 ?AllocDepthFrame@@YGPAUGafFrame@@IHH@Z
GafFrame* __stdcall AllocDepthFrame(unsigned int heap, int width, int height)
{
    int size = height * width;
    GafFrame* b = (GafFrame*)GameAllocIgnoreTag((const char*)heap, size * 2 + sizeof(GafFrame));
    unsigned char* p = (unsigned char*)(b + 1);
    b->plane0 = p;
    b->height = height;
    p += size;
    b->plane1 = p;
    b->width = width;
    b->x = 0;
    b->y = 0;
    b->flag9 = 0;
    b->count = 0;
    b->kind = 0;
    return b;
}
// FUNCTION: 0x4b8e50
void __stdcall ClearFrame(GafFrame* b, int color)
{
    if (b->flag9 == 0) {
        memset(b->plane0, color, b->width * b->height);
        if (b->plane1 != 0) {
            memset(b->plane1, 0, b->width * b->height);
        }
    }
}

void __cdecl BlitRectShadow(Surface* dst, Surface* src, Rect_004b7e60* rect,
                            Rect_004b7e60* pos, int colour, unsigned char* table);
void __cdecl BlitCompressedShadow(unsigned char* pixels, int pitch, Rect_004b7e60* rect,
                                  void* plane, Rect_004b7e60* other, unsigned char* table);

// Draws a shadow bitmap (or a list of nested shadow bitmaps) into `dst`, or
// into the screen when `dst` is null: the screen is then locked with
// LockScreen and unlocked with UnlockScreen at the end (which is why the
// unlock is guarded by a comparison of `dst` with the address of the local
// surface). The whole draw only happens when bit 7 of the display's byte at
// +0xf0 is set (the cached object at +0xc8 exists). A record with a child
// count draws each child by recursion. A leaf with a data pointer builds the
// source rect (0, 0, w - 1, h - 1) and the destination rect (x - dx, y - dy,
// ...), clips the destination to the destination's own clip rect with
// ClipRects and blits through BlitRectShadow (a byte run shadow blitter), or,
// when the mode byte at +9 is set, through BlitCompressedShadow (a word run shadow
// blitter). Both blitters are hand-written assembly and take the display's
// cached object at +0xc8 as their last argument.
// FUNCTION: 0x4b8ec0
void __stdcall DrawFrameShadow(Surface* dst, GafFrame* bmp, int x, int y)
{
    Display_004b8310* d = GetDisplay();
    if (d->flags.bits.has_obj_c8 == 1) {
        Surface screen;
        if (dst == 0) {
            int ok = LockScreen(&screen);
            if (ok != 0)
                dst = (Surface*)&screen;
        }
        if (bmp != 0) {
            if (bmp->count > 0) {
                for (int i = 0; i < (int)bmp->count; i++) {
                    GafFrame* e = ((GafFrame**)bmp->plane0)[i];
                    DrawFrameShadow(dst, e, x, y);
                }
            } else if (bmp->plane0 != 0) {
                Rect_004b7e60 other;
                Rect_004b7e60 rect;
                Rect_004b7e60 bounds;
                Surface desc;
                other.left = 0;
                other.right = bmp->width - 1;
                other.top = 0;
                other.bottom = bmp->height - 1;
                rect.left = x - bmp->x;
                rect.top = y - bmp->y;
                rect.right = rect.left + bmp->width - 1;
                rect.bottom = rect.top + bmp->height - 1;
                dst->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    if (bmp->flag9 == 0) {
                        desc.width = bmp->width;
                        desc.pitch = bmp->width;
                        desc.height = bmp->height;
                        desc.pixels = bmp->plane0;
                        BlitRectShadow(dst, &desc, &other, &rect, bmp->colour, d->lightTable);
                    } else {
                        BlitCompressedShadow(dst->pixels, dst->pitch, &rect, bmp->plane0, &other, d->lightTable);
                    }
                }
            }
        }
        if (dst == (Surface*)&screen)
            UnlockScreen(&screen);
    }
}

// Builds the "lens" displacement frame that 0x420620 asks for with
// (22, 22, 8): a GAF-style frame header with two w*h buffers of 16-bit cells.
// Each cell inside radius w/4 of the centre holds the offset (in cells) to
// the source pixel of a magnifying lens; the others hold 0x7d00 (no
// displacement).
GafFrame* __stdcall AllocDepthFrame(const char* name, int width, int height)
{
    int size = height * width;
    GafFrame* b = (GafFrame*)GameAllocIgnoreTag(name, size * 2 + sizeof(GafFrame));
    unsigned char* p = (unsigned char*)(b + 1);
    b->width = width;
    b->plane0 = p;
    b->height = height;
    p += size;
    b->plane1 = p;
    b->x = 0;
    b->y = 0;
    b->flag9 = 0;
    b->count = 0;
    b->kind = 0;
    return b;
}

// FUNCTION: 0x4b91b0
void* __stdcall BuildLensFrame(int w, int h, int lens)
{
    double scale = lens;
    GafFrame* f = AllocDepthFrame("LensFrame", w * 2, h);
    if (!f)
        return 0;
    f->width /= 2;
    unsigned short* p = (unsigned short*)f->plane0;
    // hw and hh stay short: the int w / 2 lives in the dead lens slot.
    short hw = w / 2;
    f->x = hw;
    short hh = h / 2;
    f->y = hh;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int dx = x - hw;
            int dy = y - hh;
            double dist = sqrt((double)(dx * dx + dy * dy));
            if ((int)dist >= w / 4) {
                p[y * w + x] = 0x7d00;
            } else {
                // Computed before the gain: keeps the index one shared value.
                int i = y * w + x;
                // The gain is written out at both uses, not named.
                int v = (int)(dx / ((w / 2 - dist) / scale))
                        + (w * ((int)(dy / ((w / 2 - dist) / scale)) + hh) + hw);
                p[i] = v - i;
            }
        }
    }
    return f;
}

void __stdcall CopySurfaceRect(void* dst, void* src, Rect_004b7e60* rect,
                               Rect_004b7e60* pos);

// The swap must go through a reference helper: inline, stores get forwarded.
static void SwapPtr(unsigned char*& a, unsigned char*& b)
{
    unsigned char* t = a;
    a = b;
    b = t;
}

// Draws a sprite with a colour-remap effect: the destination surface is blitted
// into the sprite's scratch buffer, the sprite's index table is remapped through
// that buffer (32000 uses the sprite's flat colour), and the result is drawn
// with DrawFrame.
//
// The two scratch buffers are swapped twice around the blit.
// FUNCTION: 0x4b9360
void __stdcall DrawLens(void* dst, GafFrame* sprite, int x, int y)
{
    SwapPtr(sprite->plane0, sprite->plane1);

    Surface surface;
    surface.width = sprite->width;
    surface.pitch = sprite->width;
    surface.height = sprite->height;
    surface.pixels = sprite->plane0;
    surface.zPriority = 10000;
    surface.colorKey = -1;
    surface.x = sprite->x;
    surface.y = sprite->y;
    surface.flag0 = 1;
    surface.flag1 = 0;
    ResetClipRect((int*)&surface);

    Rect_004b7e60 srcRect;
    srcRect.left = 0;
    srcRect.right = surface.width - 1;
    srcRect.top = 0;
    srcRect.bottom = surface.height - 1;

    Rect_004b7e60 dstRect;
    dstRect.left = x - sprite->x;
    dstRect.right = x + surface.width - sprite->x - 1;
    dstRect.top = y - sprite->y;
    dstRect.bottom = y + surface.height - sprite->y - 1;

    CopySurfaceRect(&surface, dst, &dstRect, &srcRect);

    SwapPtr(sprite->plane0, sprite->plane1);

    int n = sprite->width * sprite->height;
    unsigned char* cmap = sprite->plane1;
    unsigned short* idx = (unsigned short*)sprite->plane0;
    unsigned char* out = cmap + n;
    int i = n;
    while (i != 0) {
        short c = *idx;
        unsigned char v;
        if (c != 32000)
            v = cmap[c];
        else
            v = sprite->colour;
        *out = v;
        out++;
        cmap++;
        idx++;
        i--;
    }

    int m = sprite->width * sprite->height;
    int saved = (int)sprite->plane0;
    sprite->plane0 = sprite->plane1 + m;
    DrawFrame((Surface*)dst, sprite, x, y);
    sprite->plane0 = (unsigned char*)saved;
}
// FUNCTION: 0x4b94c0
void __stdcall GrabBackground(void* dst, GafFrame* sprite, int x, int y)
{
    Surface surface;
    surface.width = sprite->width;
    surface.height = sprite->height;
    surface.pitch = sprite->width;
    surface.pixels = sprite->plane0;
    surface.zPriority = 10000;
    surface.colorKey = -1;
    surface.x = sprite->x;
    surface.y = sprite->y;
    surface.flag0 = 1;
    surface.flag1 = 0;
    ResetClipRect((int*)&surface);

    Rect_004b7e60 srcRect;
    srcRect.left = 0;
    srcRect.right = surface.width - 1;
    srcRect.top = 0;
    srcRect.bottom = surface.height - 1;

    Rect_004b7e60 dstRect;
    dstRect.left = x - sprite->x;
    dstRect.right = surface.width + x - sprite->x - 1;
    dstRect.top = y - sprite->y;
    dstRect.bottom = surface.height + y - sprite->y - 1;

    CopySurfaceRect(&surface, dst, &dstRect, &srcRect);
}

// Clears every pixel of an 8-bit image that is not the colour key.
// FUNCTION: 0x4b96a0
void __stdcall ZeroFramePixels(GafFrame* image)
{
    int count = image->height * image->width;
    unsigned char* p = image->plane0;
    while (count--) {
        if (*p != image->colour) {
            *p = 0;
        }
        p++;
    }
}

// Remaps every pixel of an 8-bit image whose mask value is at most `level`
// (and that is not the colour key) through a table of the current palette.
// FUNCTION: 0x4b96e0
void __stdcall TintFrameBelow(GafFrame* image, unsigned char level)
{
    unsigned char* p = image->plane0;
    unsigned char* m = image->plane1;
    int count = image->height * image->width;
    Display_004b8310* pal = GetDisplay();
    while (count--) {
        if (*m <= level && *p != image->colour) {
            *p = pal->blueTable[*p];
        }
        p++;
        m++;
    }
}

// Draws a bitmap tree scaled by (sx, sy) at x, y into `dst`, or into the
// locked screen when `dst` is null. A record with a child count draws every
// child by recursion. A leaf scales its size and origin, clips the
// destination rectangle to the surface's clip rect with ClipRects, maps
// the clipped source rectangle back to bitmap pixels and then walks the
// destination pixels with 16.16 steps through the bitmap, blending every
// pixel that is not the transparent colour through the display's 256x256
// table at +0xc0.
// FUNCTION: 0x4b9740
void __stdcall DrawFrameScaledBlended(Surface* dst, GafFrame* bmp, int x, int y, double sx, double sy)
{
    Display_004b8310* d = GetDisplay();
    Surface screen;
    if (dst == 0) {
        int ok = LockScreen(&screen);
        if (ok != 0)
            dst = (Surface*)&screen;
    }
    if (bmp->count > 0) {
        for (int i = 0; i < (int)bmp->count; i++)
            DrawFrameScaledBlended(dst, ((GafFrame**)bmp->plane0)[i], x, y, sx, sy);
    } else {
        // The rects are declared before w and h: sets the order of the edge leas.
        Rect_004b7e60 src;
        Rect_004b7e60 dest;
        Rect_004b7e60 bounds;
        int w = (int)(bmp->width * sx);
        int h = (int)(bmp->height * sy);
        int dx = (int)(bmp->x * sx);
        int dy = (int)(bmp->y * sy);
        if (w > 0 && h > 0) {
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
            dst->GetClipRect(&bounds);
            ClipRects(&src, &dest, &bounds);
            if (dest.right >= dest.left && dest.bottom >= dest.top && src.right >= src.left &&
                src.bottom >= src.top) {
                src.left = (int)(src.left / sx);
                src.top = (int)(src.top / sy);
                src.right = (int)(src.right / sx);
                src.bottom = (int)(src.bottom / sy);
                // row and fy are declared together and advanced in the for header.
                int fy, row;
                for (row = dest.top, fy = src.top << 16; row <= dest.bottom; row++, fy += stepY) {
                    // col is declared before rowBase: sets the pixel address add order.
                    int srcRow = (fy >> 16) * bmp->width;
                    int col = dest.left;
                    int rowBase = row * dst->pitch;
                    for (int fx = src.left << 16; col <= dest.right; col++, fx += stepX) {
                        unsigned char c = ((unsigned char*)bmp->plane0)[(fx >> 16) + srcRow];
                        if (c != bmp->colour) {
                            unsigned char* p = rowBase + col + dst->pixels;
                            *p = d->alphaTable[(c << 8) + *p];
                        }
                    }
                }
            }
        }
    }
    if (dst == (Surface*)&screen)
        UnlockScreen(&screen);
}

// The opaque twin of 0x4b9740: draws a bitmap tree scaled by (sx, sy) at
// x, y into `dst` (or the locked screen when null) and copies every pixel that
// is not the transparent colour straight into the surface. A child whose kind
// byte (+0xb) is set is drawn by the blending version 0x4b9740, any other by
// this function again.
// FUNCTION: 0x4b9a50
void __stdcall DrawFrameScaled(Surface* dst, GafFrame* bmp, int x, int y, double sx, double sy)
{
    Surface screen;
    if (dst == 0) {
        int ok = LockScreen(&screen);
        if (ok != 0)
            dst = (Surface*)&screen;
    }
    if (bmp->count > 0) {
        for (int i = 0; i < (int)bmp->count; i++) {
            GafFrame* e = ((GafFrame**)bmp->plane0)[i];
            if (e->kind > 0)
                DrawFrameScaledBlended(dst, e, x, y, sx, sy);
            else
                DrawFrameScaled(dst, e, x, y, sx, sy);
        }
    } else {
        // The rects are declared before w and h: sets the order of the edge leas.
        Rect_004b7e60 src;
        Rect_004b7e60 dest;
        Rect_004b7e60 bounds;
        int w = (int)(bmp->width * sx);
        int h = (int)(bmp->height * sy);
        int dx = (int)(bmp->x * sx);
        int dy = (int)(bmp->y * sy);
        if (w > 0 && h > 0) {
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
            dst->GetClipRect(&bounds);
            ClipRects(&src, &dest, &bounds);
            if (dest.right >= dest.left && dest.bottom >= dest.top && src.right >= src.left &&
                src.bottom >= src.top) {
                src.left = (int)(src.left / sx);
                src.top = (int)(src.top / sy);
                src.right = (int)(src.right / sx);
                src.bottom = (int)(src.bottom / sy);
                // The original keeps the two outer loop variables in the dead
                // argument slots of x and y, so they are the parameters
                // themselves, reused once dest.left / dest.top are computed
                // from them. Both 16.16 accumulators live in the increment.
                for (x = dest.top, y = src.top << 16; x <= dest.bottom; x++, y += stepY) {
                    // col first: declaring it here is what puts rowBase in the
                    // add and col in the SIB index of the pixel store.
                    int col = dest.left;
                    int srcRow = (y >> 16) * bmp->width;
                    int rowBase = x * dst->pitch;
                    for (int fx = src.left << 16; col <= dest.right; col++, fx += stepX) {
                        unsigned char c = ((unsigned char*)bmp->plane0)[(fx >> 16) + srcRow];
                        if (c != bmp->colour) {
                            dst->pixels[rowBase + col] = c;
                        }
                    }
                }
            }
        }
    }
    if (dst == (Surface*)&screen)
        UnlockScreen(&screen);
}

// Suspected bugs:
//  - The horizontal copy count `n` is clamped to dst->width but dstCol is not
//    subtracted from it, so when dstCol > 0 the inner loop writes up to dstCol
//    bytes past the end of the destination row. The vertical clip does account
//    for dstRow. Kept as the original does.
//  - The inner loop guards on `n == 0` and not on `n <= 0`, so a negative n
//    (possible when srcCol > src->width, i.e. when x is large) counts down to
//    zero and wraps round, overwriting the row about 2^32 times.
//
// Clipped 8-bit sprite blit: copies the part of `src` that overlaps `dst`,
// skipping source pixels equal to the source colour key and writing the
// destination colour key.
// FUNCTION: 0x4b9d70
void __stdcall CutOutFrame(GafFrame* src, GafFrame* dst, int x, int y)
{
    int srcCol, dstCol, srcRow, dstRow;
    x += src->x - dst->x;
    y = (dst->y - src->y) - y;
    // Plain if/else, not an initialisation plus override: fixes the clip block.
    if (y < 0) {
        srcRow = -y;
        dstRow = 0;
    } else {
        srcRow = 0;
        dstRow = y;
    }

    dstCol = -x;
    if (dstCol < 0) { srcCol = -dstCol; dstCol = 0; } else { srcCol = 0; }

    int n;
    // Clamp stays an if/else with these arms: fixes the register allocation.
    if (src->width - srcCol > dst->width)
        n = dst->width;
    else
        n = src->width - srcCol;

    for (; srcRow < src->height; srcRow++, dstRow++) {
        if (dstRow >= dst->height)
            break;
        unsigned char* s = src->plane0 + srcRow * src->width + srcCol;
        unsigned char* d = dst->plane0 + dstRow * dst->width + dstCol;
        int i = n;
        while (i != 0) {
            if (*s != src->colour)
                *d = dst->colour;
            s++;
            d++;
            i--;
        }
    }
}

int __stdcall CompressRow(char* dest, char* src, int width, unsigned char key);

// Compresses an image row by row with CompressRow (which skips runs of the
// transparent key colour). Each row is preceded by its 16-bit compressed
// length; with a null destination it only measures. Returns the total size.
// FUNCTION: 0x4b9e60
int __stdcall CompressFrame(unsigned char* dest, GafFrame* img)
{
    int total = 0;
    unsigned char* src = img->plane0;
    int width = img->width;
    int height = img->height;
    while (height-- > 0) {
        unsigned short* len;
        if (dest) {
            len = (unsigned short*)dest;
            dest += 2;
        }
        int n = CompressRow((char*)dest, (char*)src, width, img->colour);
        src += width;
        total += n + 2;
        if (dest) {
            dest += n;
            *len = n;
        }
    }
    return total;
}

// GLOBAL: 0x51fcb0
extern unsigned char g_gafEncodeLiteralBuffer[];
// GLOBAL: 0x51fdb0
extern int g_gafEncodeOutputSize;
// FUNCTION: 0x4b9ed0
char* __stdcall EmitCopyRun(char* out, int count)
{
    int index = 0;
    do {
        int n = count;
        if (n > 0x40) {
            n = 0x40;
        }
        count -= n;
        unsigned char header = (unsigned char)(n + 0x3f);
        header <<= 2;
        if (out != 0) {
            *out++ = header;
        }
        g_gafEncodeOutputSize++;
        for (int i = 0; i < n; i++) {
            int value = g_gafEncodeLiteralBuffer[index++];
            for (unsigned int shift = 0; shift < 8; shift += 8) {
                if (out != 0) {
                    *out++ = (char)(value >> shift);
                }
                g_gafEncodeOutputSize++;
            }
        }
    } while (count > 0);
    return out;
}
// FUNCTION: 0x4b9f50
char* __stdcall EmitRepeatRun(char* out, int count, unsigned char a, unsigned char b)
{
    if (a == b) {
        do {
            int n = count;
            if (n > 0x7f) {
                n = 0x7f;
            }
            count -= n;
            unsigned char header = (unsigned char)n;
            header <<= 1;
            header |= 1;
            if (out != 0) {
                *out++ = header;
            }
            g_gafEncodeOutputSize++;
        } while (count > 0);
    } else {
        int value = a;
        do {
            int n = count;
            if (n > 0x40) {
                n = 0x40;
            }
            count -= n;
            unsigned char header = (unsigned char)(n + 0x3f);
            header <<= 2;
            header |= 2;
            if (out != 0) {
                *out++ = header;
            }
            g_gafEncodeOutputSize++;
            for (unsigned int shift = 0; shift < 8; shift += 8) {
                if (out != 0) {
                    *out++ = (char)(value >> shift);
                }
                g_gafEncodeOutputSize++;
            }
        } while (count > 0);
    }
    return out;
}

// Compresses one row of an 8-bit sprite (called by CompressFrame). The first
// loop only tests whether the whole row is the colour key; the body then
// re-reads the row from its first pixel. Bytes go into the history array
// g_gafEncodeLiteralBuffer[0..0x7f], and runs are emitted through EmitCopyRun (copy the
// buffered bytes) and EmitRepeatRun (a repeated value, or a count-only run of
// the key). The return value is the global byte counter g_gafEncodeOutputSize, so a
// null `out` only measures the row, which is how CompressFrame asks for the
// size before compressing.
// FUNCTION: 0x4ba000
int __stdcall CompressRow(char* dest, char* src, int width, unsigned char key)
{
    int runStart = 0;
    int skip;
    for (skip = 0; skip < width; skip++) {
        if (key != (unsigned char)src[skip]) {
            break;
        }
    }
    if (skip >= width) {
        return 0;
    }

    g_gafEncodeOutputSize = runStart;
    char* out = dest;
    char* p = src;
    unsigned char c = *p;
    p++;
    width--;
    unsigned char value = c;
    unsigned char prev = c;
    g_gafEncodeLiteralBuffer[0] = c;
    int n = 1;
    int state = (c == key);
    while (width) {
        width--;
        // One statement: a separate increment moves the history store.
        g_gafEncodeLiteralBuffer[n++] = c = *p++;
        value = c;
        switch (state) {
        case 0:
            if (c == key) {
                n--;
                out = EmitCopyRun(out, n);
                n = 1;
                g_gafEncodeLiteralBuffer[0] = c;
                runStart = 0;
                state = n;
                break;
            }
            if (n > 0x80) {
                n--;
                out = EmitCopyRun(out, n);
                g_gafEncodeLiteralBuffer[0] = c;
                n = 1;
                runStart = 0;
                break;
            }
            if (c == prev) {
                if (n - runStart >= 3) {
                    if (runStart > 0) {
                        out = EmitCopyRun(out, runStart);
                    }
                    state = 1;
                } else if (runStart == 0) {
                    state = 1;
                }
            } else {
                runStart = n - 1;
                break;
            }
        case 1:
            if (c == prev && n - runStart <= 0x80) {
                break;
            }
            out = EmitRepeatRun(out, n - runStart - 1, prev, key);
            runStart = 0;
            g_gafEncodeLiteralBuffer[0] = c;
            n = 1;
            state = (c == key);
            break;
        }
        prev = c;
    }
    switch (state) {
    case 0:
        EmitCopyRun(out, n);
        break;
    case 1:
        EmitRepeatRun(out, n - runStart, value, key);
        return g_gafEncodeOutputSize;
    }
    return g_gafEncodeOutputSize;
}

// Fills every pixel of an 8-bit image whose mask value is at most `level`
// with the image's colour key.
// FUNCTION: 0x4ba1b0
void __stdcall CutFrameBelow(GafFrame* image, unsigned char level)
{
    unsigned char* p = image->plane0;
    unsigned char* m = image->plane1;
    int count = image->height * image->width;
    Display_004b8310* pal = GetDisplay();
    while (count--) {
        if (*m <= level) {
            *p = image->colour;
        }
        p++;
        m++;
    }
}

// The three functions below are kept after 0x4ba1b0 and out of address order:
// at their addresses their bodies' symbol ids would move the register windows
// the other functions match in (docs/c2-regalloc.md).
// FUNCTION: 0x4b8310
void __stdcall DrawFrameLit(Surface* param_1, GafFrame* param_2, int x, int y, int param_5)
{
    Display_004b8310* d = GetDisplay();
    if ((d->flags.byte & 0x80) == 0x80) {
        Screen_004b8310 screen;
        if (param_1 == 0) {
            int locked = LockScreen(&screen.surf);
            if (locked != 0)
                param_1 = (Surface*)&screen.surf;
        }

        if (param_2 != 0) {
            if (param_2->count > 0) {
                for (int i = 0; i < param_2->count; i++)
                    DrawFrameBlended(param_1, ((GafFrame**)param_2->plane0)[i], x, y);
            } else {
                Rect_004b7e60 screen_rect;
                Rect_004b7e60 sprite_rect = { 0, 0, param_2->width - 1, param_2->height - 1 };
                int w = param_2->width;
                int h = param_2->height;
                screen_rect.left = x - param_2->x;
                screen_rect.top = y - param_2->y;
                screen_rect.right = w + screen_rect.left - 1;
                screen_rect.bottom = h + screen_rect.top - 1;
                Bounds_src_004b8310 bs;
                param_1->GetClipRect(&bs.bounds);
                ClipRects(&sprite_rect, &screen_rect, &bs.bounds);
                if (screen_rect.right >= screen_rect.left && screen_rect.bottom >= screen_rect.top
                    && sprite_rect.right >= sprite_rect.left && sprite_rect.bottom >= sprite_rect.top) {
                    if (param_2->flag9 == 0) {
                        Src_004b8310& src = bs.src;
                        src.width = param_2->width;
                        src.height = param_2->height;
                        src.pitch = param_2->width;
                        src.pixels = (GafFrame**)param_2->plane0;
                        BlitRectBlended(param_1, &src, &sprite_rect, &screen_rect, param_5, d->lightTable);
                    } else {
                        BlitCompressedLit(param_1->pixels, param_1->pitch, &screen_rect,
                            (GafFrame**)param_2->plane0, &sprite_rect, d->lightTable + (param_5 << 8));
                    }
                }
            }
        }

        if (param_1 == (Surface*)&screen.surf)
            UnlockScreen(&screen.surf);
    }
}

// FUNCTION: 0x4b90a0
void __stdcall DrawFrameDepth(GafFrame* src, GafFrame* dst, int x, int y, int level)
{
    int yoff;
    int xoff;
    xoff = dst->x - src->x + x;
    yoff = dst->y - src->y + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->plane0;
    unsigned char* sp1 = src->plane1;
    for (int row = 0; row < src->height; row++) {
        unsigned char* dp0 = xoff + dst->plane0 + dst->width * (yoff + row);
        unsigned char* dp1 = xoff + dst->plane1 + dst->width * (yoff + row);
        int n = src->width;
        while (n--) {
            unsigned char c = *sp0;
            if (c != src->colour && *dp1 <= *sp1 + level) {
                *dp0 = c;
                *dp1 = *sp1 + level;
            }
            dp0++;
            sp0++;
            dp1++;
            sp1++;
        }
    }
}

// Declared ahead of the definition, as the other files declare it: these
// symbol ids put the registers on the window DownsampleFrame matches in.
void __stdcall DownsampleFrame(GafFrame* src, GafFrame* dst);
// FUNCTION: 0x4b95a0
void __stdcall DownsampleFrame(GafFrame* src, GafFrame* dst)
{
    Display_004b8310* pal = GetDisplay();
    int row = 0;
    for (; row < dst->height; row++) {
        for (int x = 0; x < dst->width; x++) {
            unsigned char* p = src->plane0 + (row * src->width + x) * 2;
            unsigned char* table = pal->alphaTable;
            unsigned char* q = src->plane0 + (row * 2 + 1) * src->width + x * 2;
            int a = p[0];
            int b = p[1];
            int p1 = table[(a << 8) + b];
            int c = q[0];
            int d = q[1];
            int p2 = table[(c << 8) + d];
            dst->plane0[row * dst->width + x] = table[(p1 << 8) + p2];
        }
    }
}
