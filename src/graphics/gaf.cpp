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

#include "rect.h"

// Unused here: the symbol ids these declarations take keep 0x4b9740, 0x4b9d70, 0x4b90a0, 0x4b95a0 matching (docs/c2-regalloc.md).
void RegisterUnitOrders(void);
void RegisterGroundOrders(void);
void EnableAICommands(void);
void RegisterAICommands(void);
void FUN_00406f40(void);
void ResetAIPlayers(void);
void RegisterVtolOrders(void);
void StepAllGafSequences(void);
void ResetNetStats(void);
void FUN_004161f0(void);
void InitCommands(void);
int UpdatePlacementGhostValidity(void);
void RefreshSelectionOrders(void);
void DispatchOrdersPanelPageFlags(void);
void ResetCameraState(void);
void FindLocalCommander(void);
void ClampCameraPosition(void);
void ClampCameraTarget(void);
void UpdateScreenShake(void);
void UpdateCameraFollow(void);
void BeginMouseScroll(void);
void EndMouseScroll(void);
void UpdateMouseScroll(void);

// Clips `rect` to `bounds`, moving `other` by the same amounts.
// FUNCTION: 0x4b7e60
void __stdcall ClipRects(Rect* other, Rect* rect, Rect* bounds)
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

// A GAF sequence reference (handle.h's Handle): the frame index, the current
// entry's countdown, the loop byte and the sequence behind it.
#include "handle.h"

// One 8-byte entry of a GAF sequence (Thaldren's GafFrameEntry): the frame it
// names, then that frame's duration in timers.
struct GafFrameEntry {
    int frame;                         // +0x0
    unsigned short duration;           // +0x4
    unsigned short reserved;           // +0x6
};

// A GAF sequence (Thaldren's GafSequence, 0x30 bytes): its entry count, the
// loop byte, the name, then the entries from +0x28. handle.h declares it as
// GafEntry, the type behind a Handle's src and the entries the sequence
// functions advance.
struct GafEntry {
    unsigned short count;              // +0x0
    unsigned char kind;                // +0x2, loops while set
    char unknown_3[0x28 - 3];
    GafFrameEntry entries[1];          // +0x28
};

// Looks up an entry of the sequence that GetGafFrame indexes (count at +0,
// 8-byte entries from +0x28) through the reference holding an index and it.
// FUNCTION: 0x4b7ee0
int __stdcall GetGafSequenceFrame(Handle* h)
{
    int result = 0;
    if (h->src != 0)
        result = h->src->entries[h->index].frame;
    return result;
}

// FUNCTION: 0x4b7f00
int __stdcall SetGafSequenceFrame(Handle* obj, int index)
{
    int result = 0;
    if (obj->src != 0 && obj->src->count > index) {
        obj->index = index;
        result = 1;
    }
    return result;
}
// FUNCTION: 0x4b7f30
int __stdcall GetGafFrame(unsigned short* seq, int frame)
{
    int result = 0;
    GafEntry* s = (GafEntry*)seq;
    if (frame >= 0 && frame < (int)*seq && seq != 0) {
        result = s->entries[frame].frame;
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

// FUNCTION: 0x4b7f70
unsigned short __stdcall GetGafFrameDuration(Handle* param1)
{
    unsigned short result;
    if (param1->src != 0) {
        result = param1->src->entries[param1->index].duration;
    } else {
        result = 0xffff;
    }
    return result;
}

#include "surface.h"

// A GAF frame header: its size and origin, the colour key, the two flag
// bytes, the child count, and the pointers to its one or two pixel planes.
// The header's members copied here: including gaf_frame.h moves
// DownsampleFrame's two byte loads (0x4b95a0, docs/c2-regalloc.md).
struct GafFrame {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    short xOffset;                     // +0x04
    short yOffset;                     // +0x06
    unsigned char transparency;        // +0x08, the key colour
    unsigned char compressed;          // +0x09
    unsigned char layers;              // +0x0a
    unsigned char blend;               // +0x0b
    int reserved;                      // +0x0c
    unsigned char* pixelsOrLayers;     // +0x10
    unsigned char* scratch;            // +0x14, the second plane
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
void __cdecl BlitRectKeyed(Surface* dst, Surface* src, Rect* rect,
                           Rect* pos, int colour);
void __cdecl BlitRect(Surface* dst, Surface* src, Rect* rect,
                      Rect* pos);
void __cdecl BlitCompressed(unsigned char* pixels, int pitch, Rect* rect,
                            void* plane, Rect* other);
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
        if (bmp->layers > 0) {
            for (int i = 0; i < (int)bmp->layers; i++) {
                GafFrame* e = ((GafFrame**)bmp->pixelsOrLayers)[i];
                if (e->blend > 0)
                    DrawFrameBlended(dst, e, x, y);
                else
                    DrawFrame(dst, e, x, y);
            }
        } else {
            Rect other;
            Rect rect;
            Rect bounds;
            Surface desc;
            other.left = 0;
            other.right = bmp->width - 1;
            other.top = 0;
            other.bottom = bmp->height - 1;
            rect.left = x - bmp->xOffset;
            rect.top = y - bmp->yOffset;
            rect.right = rect.left + bmp->width - 1;
            rect.bottom = rect.top + bmp->height - 1;
            dst->GetClipRect(&bounds);
            ClipRects(&other, &rect, &bounds);
            if (rect.right >= rect.left && rect.bottom >= rect.top &&
                other.right >= other.left && other.bottom >= other.top) {
                if (bmp->compressed == 0) {
                    desc.width = bmp->width;
                    desc.pitch = bmp->width;
                    desc.height = bmp->height;
                    desc.pixels = bmp->pixelsOrLayers;
                    BlitRectKeyed(dst, &desc, &other, &rect, bmp->transparency);
                } else {
                    BlitCompressed(dst->pixels, dst->pitch, &rect, bmp->pixelsOrLayers, &other);
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
        if (bmp->layers > 0) {
            for (int i = 0; i < (int)bmp->layers; i++) {
                GafFrame* e = ((GafFrame**)bmp->pixelsOrLayers)[i];
                if (e->blend > 0)
                    DrawFrameBlended(dst, e, x, y);
                else
                    DrawFrame(dst, e, x, y);
            }
        } else {
            Rect other;
            Rect rect;
            Rect bounds;
            Surface desc;
            other.left = 0;
            other.right = bmp->width - 1;
            other.top = 0;
            other.bottom = bmp->height - 1;
            rect.left = x - bmp->xOffset;
            rect.top = y - bmp->yOffset;
            rect.right = rect.left + bmp->width - 1;
            rect.bottom = rect.top + bmp->height - 1;
            dst->GetClipRect(&bounds);
            ClipRects(&other, &rect, &bounds);
            if (rect.right >= rect.left && rect.bottom >= rect.top &&
                other.right >= other.left && other.bottom >= other.top) {
                if (bmp->compressed == 0) {
                    desc.width = bmp->width;
                    desc.pitch = bmp->width;
                    desc.height = bmp->height;
                    desc.pixels = bmp->pixelsOrLayers;
                    BlitRect(dst, &desc, &other, &rect);
                } else {
                    BlitCompressed(dst->pixels, dst->pitch, &rect, bmp->pixelsOrLayers, &other);
                }
            }
        }
    }
    if (dst == (Surface*)&screen)
        UnlockScreen(&screen);
}

// +0xf0: the display object's state flags, read as a byte, a word or a
// bitfield. Bit 7 (has_c8) is the cached object the shadow draw needs. The
// same word as display_context.h's flags union.
union DisplayFlags {
    unsigned short word;               // +0xf0
    unsigned char byte;                // +0xf0
    struct {
        unsigned short opt : 1;        // bit 0
        unsigned short fullscreen : 1; // bit 1, display_context.h's name
        unsigned short gdi : 1;        // bit 2
        unsigned short bit3 : 1;       // bit 3
        unsigned short bit4 : 1;       // bit 4
        unsigned short has_c0 : 1;     // bit 5
        unsigned short has_c4 : 1;     // bit 6
        unsigned short has_c8 : 1;     // bit 7
        unsigned short has_cc : 1;     // bit 8
        unsigned short has_d0 : 1;     // bit 9
        unsigned short mouseThread : 1; // bit 10, display_context.h's name
        unsigned short quitting : 1;   // bit 11
        unsigned short bit12 : 4;      // bits 12 to 15
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
    DisplayFlags flags;                // +0xf0
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
    Rect bounds;
    Src_004b8310 src;
};

Display_004b8310* GetDisplay();
void __cdecl BlitRectBlended(Surface* p, Src_004b8310* src, Rect* rect,
                             Rect* pos, int colour, unsigned char* palette);
void __cdecl BlitCompressedLit(unsigned char* pixels, int pitch, Rect* drect,
                               GafFrame** src, Rect* srect,
                               unsigned char* palette);
void __cdecl BlitCompressedBlended(unsigned char* pixels, int pitch, Rect* drect,
                                   GafFrame** src, Rect* srect,
                                   unsigned char* palette);

// Draws one bitmap (`frame`, a BITMAPINFO: width/height shorts, x/y origin
// shorts, two flag bytes, a texture count and a texture array) at x, y, either
// into `surface` or, when that is null, into the screen locked with
// LockScreen and unlocked with UnlockScreen at the end (which is why the
// unlock is guarded by a comparison of `surface` with the address of the local
// surface). A record with a child count draws each child through this same
// function (recursion). A leaf builds the source rect (0, 0, w - 1, h - 1) and
// the destination rect (x - dx, y - dy, ...), clips the destination to the
// destination's own clip rect with ClipRects and blits through
// BlitRectBlended, or, when the mode byte at +9 is set, through BlitCompressedBlended.
// FUNCTION: 0x4b8500
void __stdcall DrawFrameBlended(Surface* surface, GafFrame* frame, int x, int y)
{
    Display_004b8310* d = GetDisplay();
    if ((d->flags.byte & 0x20) != 0) {
        Screen_004b8310 screen;
        if (surface == 0) {
            int locked = LockScreen(&screen.surf);
            if (locked != 0)
                surface = (Surface*)&screen.surf;
        }

        if (frame != 0) {
            if (frame->layers > 0) {
                for (int i = 0; i < frame->layers; i++)
                    DrawFrameBlended(surface, ((GafFrame**)frame->pixelsOrLayers)[i], x, y);
            } else {
                Rect screen_rect;
                Rect sprite_rect = { 0, 0, frame->width - 1, frame->height - 1 };
                int w = frame->width;
                int h = frame->height;
                screen_rect.left = x - frame->xOffset;
                screen_rect.top = y - frame->yOffset;
                screen_rect.right = w + screen_rect.left - 1;
                screen_rect.bottom = h + screen_rect.top - 1;
                Bounds_src_004b8310 bs;
                surface->GetClipRect(&bs.bounds);
                ClipRects(&sprite_rect, &screen_rect, &bs.bounds);
                if (screen_rect.right >= screen_rect.left && screen_rect.bottom >= screen_rect.top
                    && sprite_rect.right >= sprite_rect.left && sprite_rect.bottom >= sprite_rect.top) {
                    if (frame->compressed == 0) {
                        Src_004b8310& src = bs.src;
                        src.width = frame->width;
                        src.height = frame->height;
                        src.pitch = frame->width;
                        src.pixels = (GafFrame**)frame->pixelsOrLayers;
                        BlitRectBlended(surface, &src, &sprite_rect, &screen_rect,
                            frame->transparency, d->alphaTable);
                    } else {
                        BlitCompressedBlended(surface->pixels, surface->pitch, &screen_rect,
                            (GafFrame**)frame->pixelsOrLayers, &sprite_rect, d->alphaTable);
                    }
                }
            }
        }

        if (surface == (Surface*)&screen.surf)
            UnlockScreen(&screen.surf);
    }
}

void __cdecl BlitRectRemapDest(Surface* dst, Surface* src, Rect* rect,
                               Rect* pos, int colour, unsigned char* table);
void __stdcall ResetClipRect(int* surface);

// Recursively draws a bitmap tree (`frame`) at x, y into `surface`, or into
// the locked screen when `surface` is null. Runs only when the display flag
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
void __stdcall DrawFrameGray(Surface* surface, GafFrame* frame, int x, int y)
{
    Display_004b8310* d = GetDisplay();
    // Flag tested as bit 0x100 of the word: fuses into a byte test at +0xf1.
    if ((d->flags.word & 0x100) != 0 && frame->compressed == 0) {
        Surface screen;
        if (surface == 0) {
            int locked = LockScreen(&screen);
            if (locked != 0)
                surface = (Surface*)&screen;
        }
        if (frame != 0) {
            if (frame->layers > 0) {
                for (int i = 0; i < frame->layers; i++)
                    DrawFrameGray(surface, ((GafFrame**)frame->pixelsOrLayers)[i], x, y);
            } else {
                Rect other;
                Rect rect;
                other.left = 0;
                other.right = frame->width - 1;
                other.top = 0;
                other.bottom = frame->height - 1;
                rect.left = x - frame->xOffset;
                rect.top = y - frame->yOffset;
                rect.right = rect.left + frame->width - 1;
                rect.bottom = rect.top + frame->height - 1;
                Rect bounds;
                surface->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    Surface s;
                    s.width = frame->width;
                    s.height = frame->height;
                    s.pitch = frame->width;
                    s.pixels = (unsigned char*)frame->pixelsOrLayers;
                    s.zPriority = 10000;
                    s.colorKey = -1;
                    s.x = frame->xOffset;
                    s.y = frame->yOffset;
                    s.flag0 = 1;
                    s.flag1 = 0;
                    ResetClipRect((int*)&s);
                    BlitRectRemapDest(surface, &s, &other, &rect, frame->transparency, d->grayTable);
                }
            }
        }
        if (surface == (Surface*)&screen)
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
    if (bmp->compressed == 0) {
        if (dst == 0) {
            int ok = LockScreen(&screen);
            if (ok != 0)
                dst = (Surface*)&screen;
        }
        if (bmp != 0) {
            if (bmp->layers > 0) {
                for (int i = 0; i < (int)bmp->layers; i++) {
                    GafFrame* e = ((GafFrame**)bmp->pixelsOrLayers)[i];
                    EraseFrameDithered(dst, e, x, y, parity);
                }
            } else {
                Rect rect;
                Rect other;
                Rect bounds;
                other.left = 0;
                other.top = 0;
                other.right = bmp->width - 1;
                other.bottom = bmp->height - 1;
                rect.left = x - bmp->xOffset;
                rect.top = y - bmp->yOffset;
                rect.right = rect.left + bmp->width - 1;
                rect.bottom = rect.top + bmp->height - 1;
                dst->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    // `sy` must be a plain local here, not other.top++: pins stack slots and loop order.
                    for (int yy = rect.top, sy = other.top; yy <= rect.bottom; yy++, sy++) {
                        unsigned char* s = (unsigned char*)bmp->pixelsOrLayers
                            + sy * bmp->width + other.left;
                        unsigned char* d = dst->pixels
                            + yy * dst->pitch + rect.left;
                        unsigned char* end = d + rect.right - rect.left;
                        if (((yy + rect.left + parity) & 1) != 0) {
                            d++;
                            s++;
                        }
                        while (d <= end) {
                            if (*s != bmp->transparency)
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

// Initialises a drawing surface description from a frame: width, height,
// pitch (= width) and pixels, then resets its clip rectangle through
// ResetClipRect.
// FUNCTION: 0x4b8a80
void __stdcall SurfaceFromFrame(Surface* dst, GafFrame* src)
{
    dst->width = src->width;
    dst->height = src->height;
    dst->pitch = src->width;
    dst->pixels = src->pixelsOrLayers;
    dst->zPriority = 10000;
    dst->colorKey = -1;
    dst->x = src->xOffset;
    dst->y = src->yOffset;
    dst->flag1 = 0;
    dst->flag0 = 1;
    ResetClipRect((int*)dst);
}

// Unused here: real declarations that hold the symbol ids the local surface
// view used to take, so the frame functions keep their allocation
// (docs/c2-regalloc.md).
Surface* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(Surface* obj);
int __stdcall SetPageFlipping(int enable);

// Unused here: real functions declared to keep the file's symbol count.
void __stdcall DrawBattleFrame(int param_1, int param_2);
void __stdcall HAPI_FindClose(int handle);

// Initialises a frame from a surface description. The width is written
// twice in the original (first the surface's width, then its pitch).
// FUNCTION: 0x4b8ae0
void __stdcall FrameFromSurface(GafFrame* dst, Surface* src)
{
    dst->width = src->width;
    dst->height = src->height;
    dst->width = src->pitch;
    dst->pixelsOrLayers = src->pixels;
    dst->xOffset = src->x;
    dst->yOffset = src->y;
    dst->compressed = 0;
    dst->transparency = 0xff;
    dst->layers = 0;
    dst->blend = 0;
}

// The current entry's duration, or 0xffff when there is no sequence.
static inline unsigned short Value_004b8b30(GafEntry* s, unsigned short i)
{
    if (s)
        return s->entries[i].duration;
    return 0xffff;
}
// FUNCTION: 0x4b8b30
void __stdcall InitGafSequence(Handle* ref, GafEntry* src, int index)
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
int __stdcall StepGafSequence(Handle* ref)
{
    GafEntry* src = ref->src;
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
            ref->value = src->entries[ref->index].duration;
            return 1;
        }
        ref->value--;
    }
    return 0;
}

// The advancing view of a sequence reference: the timer value is signed here,
// so a step can take it below zero (handle.h's value is unsigned).
struct Ref_004b8bf0 {
    unsigned short index;           // +0x0
    short value;                    // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    GafEntry* src;                  // +0x8
};

static inline unsigned short Value_004b8bf0(Ref_004b8bf0* r)
{
    if (r->src)
        return r->src->entries[r->index].duration;
    return 0xffff;
}

// Same frame-sequence layout as 0x4b8b30: advances the reference by `step`
// time units, moving to the next entry whenever its countdown runs out and
// wrapping around (or dropping the sequence when it doesn't loop).
// FUNCTION: 0x4b8bf0
void __stdcall AdvanceGafSequence(Ref_004b8bf0* ref, short step)
{
    GafEntry* src = ref->src;
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
// Stays its own view: adding the name to GafEntry moves 0x4b9d70, 0x4b90a0 and
// 0x4b95a0, and idpad.py finds no id count that brings all three back.
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
    p->pixelsOrLayers = (unsigned char*)(p + 1);
    p->scratch = 0;
    p->xOffset = 0;
    p->yOffset = 0;
    p->compressed = 0;
    p->layers = 0;
    p->blend = 0;
    return p;
}
// FUNCTION: 0x4b8e00 ?AllocDepthFrame@@YGPAUGafFrame@@IHH@Z
GafFrame* __stdcall AllocDepthFrame(unsigned int heap, int width, int height)
{
    int size = height * width;
    GafFrame* b = (GafFrame*)GameAllocIgnoreTag((const char*)heap, size * 2 + sizeof(GafFrame));
    unsigned char* p = (unsigned char*)(b + 1);
    b->pixelsOrLayers = p;
    b->height = height;
    p += size;
    b->scratch = p;
    b->width = width;
    b->xOffset = 0;
    b->yOffset = 0;
    b->compressed = 0;
    b->layers = 0;
    b->blend = 0;
    return b;
}
// FUNCTION: 0x4b8e50
void __stdcall ClearFrame(GafFrame* b, int color)
{
    if (b->compressed == 0) {
        memset(b->pixelsOrLayers, color, b->width * b->height);
        if (b->scratch != 0) {
            memset(b->scratch, 0, b->width * b->height);
        }
    }
}

void __cdecl BlitRectShadow(Surface* dst, Surface* src, Rect* rect,
                            Rect* pos, int colour, unsigned char* table);
void __cdecl BlitCompressedShadow(unsigned char* pixels, int pitch, Rect* rect,
                                  void* plane, Rect* other, unsigned char* table);

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
    if (d->flags.bits.has_c8 == 1) {
        Surface screen;
        if (dst == 0) {
            int ok = LockScreen(&screen);
            if (ok != 0)
                dst = (Surface*)&screen;
        }
        if (bmp != 0) {
            if (bmp->layers > 0) {
                for (int i = 0; i < (int)bmp->layers; i++) {
                    GafFrame* e = ((GafFrame**)bmp->pixelsOrLayers)[i];
                    DrawFrameShadow(dst, e, x, y);
                }
            } else if (bmp->pixelsOrLayers != 0) {
                Rect other;
                Rect rect;
                Rect bounds;
                Surface desc;
                other.left = 0;
                other.right = bmp->width - 1;
                other.top = 0;
                other.bottom = bmp->height - 1;
                rect.left = x - bmp->xOffset;
                rect.top = y - bmp->yOffset;
                rect.right = rect.left + bmp->width - 1;
                rect.bottom = rect.top + bmp->height - 1;
                dst->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    if (bmp->compressed == 0) {
                        desc.width = bmp->width;
                        desc.pitch = bmp->width;
                        desc.height = bmp->height;
                        desc.pixels = bmp->pixelsOrLayers;
                        BlitRectShadow(dst, &desc, &other, &rect, bmp->transparency, d->lightTable);
                    } else {
                        BlitCompressedShadow(dst->pixels, dst->pitch, &rect, bmp->pixelsOrLayers, &other, d->lightTable);
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
    b->pixelsOrLayers = p;
    b->height = height;
    p += size;
    b->scratch = p;
    b->xOffset = 0;
    b->yOffset = 0;
    b->compressed = 0;
    b->layers = 0;
    b->blend = 0;
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
    unsigned short* p = (unsigned short*)f->pixelsOrLayers;
    // hw and hh stay short: the int w / 2 lives in the dead lens slot.
    short hw = w / 2;
    f->xOffset = hw;
    short hh = h / 2;
    f->yOffset = hh;
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

void __stdcall CopySurfaceRect(void* dst, void* src, Rect* rect,
                               Rect* pos);

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
    SwapPtr(sprite->pixelsOrLayers, sprite->scratch);

    Surface surface;
    surface.width = sprite->width;
    surface.pitch = sprite->width;
    surface.height = sprite->height;
    surface.pixels = sprite->pixelsOrLayers;
    surface.zPriority = 10000;
    surface.colorKey = -1;
    surface.x = sprite->xOffset;
    surface.y = sprite->yOffset;
    surface.flag0 = 1;
    surface.flag1 = 0;
    ResetClipRect((int*)&surface);

    Rect srcRect;
    srcRect.left = 0;
    srcRect.right = surface.width - 1;
    srcRect.top = 0;
    srcRect.bottom = surface.height - 1;

    Rect dstRect;
    dstRect.left = x - sprite->xOffset;
    dstRect.right = x + surface.width - sprite->xOffset - 1;
    dstRect.top = y - sprite->yOffset;
    dstRect.bottom = y + surface.height - sprite->yOffset - 1;

    CopySurfaceRect(&surface, dst, &dstRect, &srcRect);

    SwapPtr(sprite->pixelsOrLayers, sprite->scratch);

    int n = sprite->width * sprite->height;
    unsigned char* cmap = sprite->scratch;
    unsigned short* idx = (unsigned short*)sprite->pixelsOrLayers;
    unsigned char* out = cmap + n;
    int i = n;
    while (i != 0) {
        short c = *idx;
        unsigned char v;
        if (c != 32000)
            v = cmap[c];
        else
            v = sprite->transparency;
        *out = v;
        out++;
        cmap++;
        idx++;
        i--;
    }

    int m = sprite->width * sprite->height;
    int saved = (int)sprite->pixelsOrLayers;
    sprite->pixelsOrLayers = sprite->scratch + m;
    DrawFrame((Surface*)dst, sprite, x, y);
    sprite->pixelsOrLayers = (unsigned char*)saved;
}
// FUNCTION: 0x4b94c0
void __stdcall GrabBackground(void* dst, GafFrame* sprite, int x, int y)
{
    Surface surface;
    surface.width = sprite->width;
    surface.height = sprite->height;
    surface.pitch = sprite->width;
    surface.pixels = sprite->pixelsOrLayers;
    surface.zPriority = 10000;
    surface.colorKey = -1;
    surface.x = sprite->xOffset;
    surface.y = sprite->yOffset;
    surface.flag0 = 1;
    surface.flag1 = 0;
    ResetClipRect((int*)&surface);

    Rect srcRect;
    srcRect.left = 0;
    srcRect.right = surface.width - 1;
    srcRect.top = 0;
    srcRect.bottom = surface.height - 1;

    Rect dstRect;
    dstRect.left = x - sprite->xOffset;
    dstRect.right = surface.width + x - sprite->xOffset - 1;
    dstRect.top = y - sprite->yOffset;
    dstRect.bottom = surface.height + y - sprite->yOffset - 1;

    CopySurfaceRect(&surface, dst, &dstRect, &srcRect);
}

// Clears every pixel of an 8-bit image that is not the colour key.
// FUNCTION: 0x4b96a0
void __stdcall ZeroFramePixels(GafFrame* image)
{
    int count = image->height * image->width;
    unsigned char* p = image->pixelsOrLayers;
    while (count--) {
        if (*p != image->transparency) {
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
    unsigned char* p = image->pixelsOrLayers;
    unsigned char* m = image->scratch;
    int count = image->height * image->width;
    Display_004b8310* pal = GetDisplay();
    while (count--) {
        if (*m <= level && *p != image->transparency) {
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
    if (bmp->layers > 0) {
        for (int i = 0; i < (int)bmp->layers; i++)
            DrawFrameScaledBlended(dst, ((GafFrame**)bmp->pixelsOrLayers)[i], x, y, sx, sy);
    } else {
        // The rects are declared before w and h: sets the order of the edge leas.
        Rect src;
        Rect dest;
        Rect bounds;
        int w = (int)(bmp->width * sx);
        int h = (int)(bmp->height * sy);
        int dx = (int)(bmp->xOffset * sx);
        int dy = (int)(bmp->yOffset * sy);
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
                        unsigned char c = ((unsigned char*)bmp->pixelsOrLayers)[(fx >> 16) + srcRow];
                        if (c != bmp->transparency) {
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
    if (bmp->layers > 0) {
        for (int i = 0; i < (int)bmp->layers; i++) {
            GafFrame* e = ((GafFrame**)bmp->pixelsOrLayers)[i];
            if (e->blend > 0)
                DrawFrameScaledBlended(dst, e, x, y, sx, sy);
            else
                DrawFrameScaled(dst, e, x, y, sx, sy);
        }
    } else {
        // The rects are declared before w and h: sets the order of the edge leas.
        Rect src;
        Rect dest;
        Rect bounds;
        int w = (int)(bmp->width * sx);
        int h = (int)(bmp->height * sy);
        int dx = (int)(bmp->xOffset * sx);
        int dy = (int)(bmp->yOffset * sy);
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
                        unsigned char c = ((unsigned char*)bmp->pixelsOrLayers)[(fx >> 16) + srcRow];
                        if (c != bmp->transparency) {
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
    x += src->xOffset - dst->xOffset;
    y = (dst->yOffset - src->yOffset) - y;
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
        unsigned char* s = src->pixelsOrLayers + srcRow * src->width + srcCol;
        unsigned char* d = dst->pixelsOrLayers + dstRow * dst->width + dstCol;
        int i = n;
        while (i != 0) {
            if (*s != src->transparency)
                *d = dst->transparency;
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
    unsigned char* src = img->pixelsOrLayers;
    int width = img->width;
    int height = img->height;
    while (height-- > 0) {
        unsigned short* len;
        if (dest) {
            len = (unsigned short*)dest;
            dest += 2;
        }
        int n = CompressRow((char*)dest, (char*)src, width, img->transparency);
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
    unsigned char* p = image->pixelsOrLayers;
    unsigned char* m = image->scratch;
    int count = image->height * image->width;
    Display_004b8310* pal = GetDisplay();
    while (count--) {
        if (*m <= level) {
            *p = image->transparency;
        }
        p++;
        m++;
    }
}

// The three functions below are kept after 0x4ba1b0 and out of address order:
// at their addresses their bodies' symbol ids would move the register windows
// the other functions match in (docs/c2-regalloc.md).
// FUNCTION: 0x4b8310
void __stdcall DrawFrameLit(Surface* surface, GafFrame* frame, int x, int y, int paletteRow)
{
    Display_004b8310* d = GetDisplay();
    if ((d->flags.byte & 0x80) == 0x80) {
        Screen_004b8310 screen;
        if (surface == 0) {
            int locked = LockScreen(&screen.surf);
            if (locked != 0)
                surface = (Surface*)&screen.surf;
        }

        if (frame != 0) {
            if (frame->layers > 0) {
                for (int i = 0; i < frame->layers; i++)
                    DrawFrameBlended(surface, ((GafFrame**)frame->pixelsOrLayers)[i], x, y);
            } else {
                Rect screen_rect;
                Rect sprite_rect = { 0, 0, frame->width - 1, frame->height - 1 };
                int w = frame->width;
                int h = frame->height;
                screen_rect.left = x - frame->xOffset;
                screen_rect.top = y - frame->yOffset;
                screen_rect.right = w + screen_rect.left - 1;
                screen_rect.bottom = h + screen_rect.top - 1;
                Bounds_src_004b8310 bs;
                surface->GetClipRect(&bs.bounds);
                ClipRects(&sprite_rect, &screen_rect, &bs.bounds);
                if (screen_rect.right >= screen_rect.left && screen_rect.bottom >= screen_rect.top
                    && sprite_rect.right >= sprite_rect.left && sprite_rect.bottom >= sprite_rect.top) {
                    if (frame->compressed == 0) {
                        Src_004b8310& src = bs.src;
                        src.width = frame->width;
                        src.height = frame->height;
                        src.pitch = frame->width;
                        src.pixels = (GafFrame**)frame->pixelsOrLayers;
                        BlitRectBlended(surface, &src, &sprite_rect, &screen_rect, paletteRow, d->lightTable);
                    } else {
                        BlitCompressedLit(surface->pixels, surface->pitch, &screen_rect,
                            (GafFrame**)frame->pixelsOrLayers, &sprite_rect, d->lightTable + (paletteRow << 8));
                    }
                }
            }
        }

        if (surface == (Surface*)&screen.surf)
            UnlockScreen(&screen.surf);
    }
}

// FUNCTION: 0x4b90a0
void __stdcall DrawFrameDepth(GafFrame* src, GafFrame* dst, int x, int y, int level)
{
    int yoff;
    int xoff;
    xoff = dst->xOffset - src->xOffset + x;
    yoff = dst->yOffset - src->yOffset + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->pixelsOrLayers;
    unsigned char* sp1 = src->scratch;
    for (int row = 0; row < src->height; row++) {
        unsigned char* dp0 = xoff + dst->pixelsOrLayers + dst->width * (yoff + row);
        unsigned char* dp1 = xoff + dst->scratch + dst->width * (yoff + row);
        int n = src->width;
        while (n--) {
            unsigned char c = *sp0;
            if (c != src->transparency && *dp1 <= *sp1 + level) {
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
            unsigned char* p = src->pixelsOrLayers + (row * src->width + x) * 2;
            unsigned char* table = pal->alphaTable;
            unsigned char* q = src->pixelsOrLayers + (row * 2 + 1) * src->width + x * 2;
            int a = p[0];
            int b = p[1];
            int p1 = table[(a << 8) + b];
            int c = q[0];
            int d = q[1];
            int p2 = table[(c << 8) + d];
            dst->pixelsOrLayers[row * dst->width + x] = table[(p1 << 8) + p2];
        }
    }
}
