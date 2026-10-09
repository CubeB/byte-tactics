// Decompiled by space-bunny-free, Opus, Sonnet 5.5, LongCat 2.5 Preview Free, deepseek-v4.1-flash, GPT-6.1-sol, mimo-v2.6-pro, claude-opus-5-5, Space Bunny Free, Claude Sonnet 5.5, Haiku, GPT-6, deepseek-v4.1, DeepSeek V4.1 Flash, Claude Opus 5.5, space-bunny-alpha and Sonnet. Names are provisional.
// The draw module: lines, circles, polygons, fades and scanline fills into a
// surface or the locked screen. ScanFillPolygon (0x4c0330), FillFlatSpan
// (0x4c06e0) and PlotSpanEnds (0x4c0a90) stay in files of their own: their
// register allocation follows symbol ids.
#include <string.h>
// The standard headers below only move the symbol counter to the windows the
// merged functions need (docs/c2-regalloc.md).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

// A 16-byte rectangle, the argument of the clip helpers and the edge drawers.
struct Rect {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// A point of the 2D outlines.
#include "../util/point.h"

// A vertex of the 3D outlines: the position, with z the 16.16 depth.
struct Vec3 {
    int x;
    int y;
    int z;
};

// A 0x30-byte surface: the size, the pitch and the pixel buffer, with the
// clip rectangle at +0x1c. A locked screen descriptor has the same shape.
struct Surface {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
    char* pixels;                      // +0xc
    int unknown_10[8];                 // +0x10 to +0x2f
    Rect* GetClipRect(Rect* out);
};

// The software renderer's target: the row width at +0, the pixel buffer at
// +0x10 and the depth buffer at +0x14.
struct GafFrame {
    unsigned short pitch;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[0x10 - 0x4];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
    // Must stay: without an inline function in the file the row product goes to ebp.
    unsigned short Pitch() { return pitch; }
};

// One scanline of a polygon: the two ends, their 16.16 depth and shade.
struct Span {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
    int s1;                            // +0x20 (16.16)
    int s2;                            // +0x24 (16.16)
};

// A shaded polygon vertex: position, depth and shade.
struct Point_004c0c70 {
    int x;
    int y;
    int z;
    int s;
};

// The two ends of the segment being drawn.
struct Segment_004bf060 {
    Point* from;
    Point* to;
};

// The display object's fade tables at +0xc4 and the screen size at +0xd4.
struct Engine_004bf4d0 {
    char unknown_0[0xc4];
    unsigned char* fade_neg;           // +0xc4
    unsigned char* fade_pos;           // +0xc8
    char unknown_cc[0xd4 - 0xcc];
    int width;                         // +0xd4
    int height;                        // +0xd8
};

// The display object's colour table at +0xc8.
struct App_004bec70 {
    char unknown_0[0xc8];
    unsigned int* palette;             // +0xc8
};

// The display object's colour table at +0xc0.
struct App_004bed70 {
    char unknown_0[0xc0];
    unsigned int* palette;             // +0xc0
};

// The display object's shaded palette table at +0xc4.
struct App_004c0b10 {
    char unknown_0[0xc4];
    unsigned char* shade;              // +0xc4
};

// The display object's grey table at +0xcc and its flags at +0xf1.
struct Game_004bfe10 {
    char unknown_0[0xcc];
    int field_cc;                      // +0xcc
    char unknown_d0[0xf1 - 0xd0];
    unsigned char flags;               // +0xf1
};

// The display object at +0x210.
struct Obj_004c13d0 {
    char unknown_0[0x210];
    int keyColor;                      // +0x210
};

// The display object's text colours at +0x208, +0x20c and +0x210.
struct Obj {
    char unknown_0[0x208];
    int foreColor;                     // +0x208
    int backColor;                     // +0x20c
    int keyColor;                      // +0x210
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl BlitLine(Surface* dst, int x0, int y0, int x1, int y1, int color);
void __cdecl BlitLineRemapped(Surface* dst, int x0, int y0, int x1, int y1,
                              int color, unsigned int* palette);
int __stdcall GetDisplay();
int __stdcall ClipRectangle(Surface* surface, Rect* r);
void __cdecl FillSolidRect(Surface* s, Rect* r, int color);
void __cdecl RemapRect(int dst, int pitch, int w, int h, int table);
void __cdecl XorRect(Surface* s, Rect* r, int value);
int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);
int __stdcall ScanFillPolygon(Surface* surface, Point* points, int n,
                              unsigned char color);
void __stdcall PlotSpanEnds(int row, Span* span, GafFrame* surf,
                            unsigned char color);
void __stdcall FillFlatSpan(int row, Span* span, GafFrame* surf,
                            unsigned char color);


// Draws into `surface`, or into the screen (locked with LockScreen and
// unlocked with UnlockScreen) when `surface` is null. ClipLine clips the
// rectangle and BlitLine fills it. Returns the lock result on the screen
// path, so a failed lock returns 0 without ever unlocking.
// The original calls this out of line from DrawRectangle and DrawPolygon;
// without this the merged file inlines it into them.
#pragma auto_inline(off)
// FUNCTION: 0x4be950
int __stdcall DrawLine(Surface* surface, int x0, int y0, int x1, int y1,
                           int color)
{
    int ret;
    if (surface == 0) {
        Surface screen;
        ret = LockScreen(&screen);
        if (ret) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                BlitLine(&screen, x0, y0, x1, y1, color);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLine(surface, x0, y0, x1, y1, color);
        ret = 1;
    }
    return ret;
}
#pragma auto_inline(on)


// Clips the segment (x0, y0) - (x1, y1) to the rect the surface hands back
// from Surface::GetClipRect (the surface's own copy at +0x1c, fetched
// into a local), moving each end point in turn against the four edges, and
// returning 0 when an end point is on the wrong side of an edge or when the
// delta of the axis being moved is zero.
//
// The surface is never dereferenced except as `this`, so nothing in the clip
// steps comes out of it.
//
// Each end point is tested against the four edges in the order left, top,
// right, bottom, and the axis tested alternates x, y, x, y.
// FUNCTION: 0x4bea20
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1)
{
    Rect r;
    int to_right = (*x0 <= *x1);
    int downwards = (*y0 <= *y1);
    // dx is declared before dy.
    int dx = *x1 - *x0;
    int dy = *y1 - *y0;
    dst->GetClipRect(&r);
    if (*x0 < r.left) {
        if (to_right == 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y0 += (r.left - *x0) * dy / dx;
        *x0 = r.left;
    }
    if (*y0 < r.top) {
        if (downwards == 0)
            return 0;
        if (dy == 0)
            goto fail;
        *x0 += (r.top - *y0) * dx / dy;
        *y0 = r.top;
    }
    if (*x0 > r.right) {
        if (to_right != 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y0 += (r.right - *x0) * dy / dx;
        *x0 = r.right;
    }
    if (*y0 > r.bottom) {
        if (downwards != 0)
            return 0;
        if (dy == 0)
            goto fail;
        *x0 += (r.bottom - *y0) * dx / dy;
        *y0 = r.bottom;
    }
    if (*x1 < r.left) {
        if (to_right != 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y1 += (r.left - *x1) * dy / dx;
        *x1 = r.left;
    }
    if (*y1 < r.top) {
        if (downwards != 0)
            return 0;
        if (dy == 0)
            goto fail;
        *x1 += (r.top - *y1) * dx / dy;
        *y1 = r.top;
    }
    if (*x1 > r.right) {
        if (to_right == 0)
            return 0;
        if (dx == 0)
            goto fail;
        *y1 += (r.right - *x1) * dy / dx;
        *x1 = r.right;
    }
    if (*y1 > r.bottom) {
        if (downwards == 0)
            return 0;
        // Last delta test is positive and falls out to `goto fail`: keeps the
        // block's return 1 from merging with the trailing one.
        if (dy != 0) {
            *x1 += (r.bottom - *y1) * dx / dy;
            *y1 = r.bottom;
            return 1;
        }
        goto fail;
    }
    return 1;
fail:
    return 0;
}



// Sibling of 0x4bed70 and 0x4be950: draws into `surface`, or into the screen
// (locked with LockScreen and unlocked with UnlockScreen) when `surface` is
// null. The rectangle is handed to ClipLine by address, so it can clip it
// in place, and the clipped values go to BlitLineRemapped to draw. The state's
// colour table at +0xc8 is both the guard and the last argument of the draw,
// and the result is the surface that was drawn on, so a failed lock returns 0
// without ever unlocking.
// FUNCTION: 0x4bec70
Surface* __stdcall DrawLitLine(Surface* surface, int x0, int y0,
                                         int x1, int y1, int color)
{
    App_004bec70* app = (App_004bec70*)GetDisplay();
    if (!app->palette)
        return 0;
    Surface* ret;
    if (surface == 0) {
        Surface screen;
        ret = (Surface*)LockScreen(&screen);
        if (ret) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                BlitLineRemapped(&screen, x0, y0, x1, y1, color, app->palette);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLineRemapped(surface, x0, y0, x1, y1, color, app->palette);
        ret = (Surface*)1;
    }
    return ret;
}



// Sibling of 0x4be950: draws into `surface`, or into the screen (locked with
// LockScreen and unlocked with UnlockScreen) when `surface` is null. The
// extra argument to the draw is the state's colour table at +0xc0, and the
// result is the surface that was drawn on, so a failed lock returns 0 without
// ever unlocking.
// FUNCTION: 0x4bed70
Surface* __stdcall DrawBlendedLine(Surface* surface, int x0, int y0,
                                         int x1, int y1, int color)
{
    App_004bed70* app = (App_004bed70*)GetDisplay();
    Surface* ret;
    if (surface == 0) {
        Surface screen;
        ret = (Surface*)LockScreen(&screen);
        if (ret) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                BlitLineRemapped(&screen, x0, y0, x1, y1, color, app->palette);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLineRemapped(surface, x0, y0, x1, y1, color, app->palette);
        ret = (Surface*)1;
    }
    return ret;
}


// Draws one character of text: ClipLine clamps the position into the
// surface and BlitLine draws the glyph, taking both the clamped and the
// original coordinates. When `surface` is null the screen is locked with
// LockScreen instead, and if that lock fails a second surface is tried.
// UnlockScreen then unlocks whatever LockScreen left in `screen`, even on
// the fallback path where the lock failed.
// FUNCTION: 0x4bee60
int __stdcall DrawPixel(Surface* surface, int x, int y, int ch)
{
    int ret;
    if (surface == 0) {
        Surface screen;
        ret = LockScreen(&screen);
        if (ret) {
            // Declared in this order: it decides each local's stack slot.
            int y1 = y, x1 = x, y0 = y, x0 = x;
            if (&screen == 0) {
                Surface other;
                if (LockScreen(&other)) {
                    if (ClipLine(&other, &x0, &y0, &x1, &y1))
                        BlitLine(&other, x0, y0, x1, y1, ch);
                    UnlockScreen(&other);
                }
            } else {
                if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                    BlitLine(&screen, x0, y0, x1, y1, ch);
            }
            UnlockScreen(&screen);
        }
    } else {
        int y1 = y, x1 = x, y0 = y, x0 = x;
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLine(surface, x0, y0, x1, y1, ch);
        ret = 1;
    }
    return ret;
}


// Reads the pixel at (x, y) of a surface, or of the screen (locked with
// LockScreen and unlocked with UnlockScreen) when `surface` is null.
//
// When the screen cannot be locked the colour is returned uninitialised: its
// stack home is y's slot, which is why that path returns y.
// FUNCTION: 0x4befe0
unsigned int __stdcall ReadPixel(Surface* surface, int x, int y)
{
    unsigned int color;
    if (surface == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            color = (unsigned char)screen.pixels[screen.pitch * y + x];
            UnlockScreen(&screen);
        }
    } else {
        color = (unsigned char)surface->pixels[surface->pitch * y + x];
    }
    return color;
}



// Draws the polyline points[0..count-1] into `surface`, or into the screen
// (locked with LockScreen, unlocked with UnlockScreen) when `surface` is
// null. Every segment is clipped by ClipLine and drawn by BlitLine.
// Returns the lock result on the screen path, so a failed lock returns 0
// without ever unlocking; the caller-surface path returns 1.
//
// The locked branch carries a copy of the single segment drawer DrawLine
// per segment, nested lock and all, which is what /Ob2 leaves behind when the
// source calls it: the callee's own `if (surface == 0)` is the `if (&screen ==
// 0)` below, tested on an address the compiler knows is not null, so the test
// survives as `lea eax, [esp+0x14]; test eax, eax; jne` with the locked block
// as the fall-through. Keeping that test is what puts the second lock call and
// its unlock in the function at all.
//
// check.py prints MATCH (508 bytes). Two source shapes carry the match and
// both are needed:
// - the segment ends walk the array through the local Segment_004bf060. With
//   two plain `Point*` locals (however they are spelled, in either order, or
//   with an index) MSVC 5 folds them into a single induction variable biased
//   to a middle field, and then one callee-saved register is free, so the
//   result stays in ebp: the frame is 0x60, one `add reg, 8` is missing and
//   the function is 464 bytes (32.1 percent). The struct keeps both pointers
//   as induction variables (esi and edi) and spills the result to [esp+0x10],
//   which is what fixes the 0x64 frame and the 25 dwords of locals.
// - the four coordinates are declared in the order x0, y0, x1, y1 (that order
//   picks which dead argument slot each one lands in: x0 at [esp+0x78] and
//   y0 at [esp+0x84] in the locked loop, x0 at [esp+0x10] and y0 at
//   [esp+0x78] in the caller-surface loop) and assigned in the order y1, x1,
//   y0, x0, which is the order the original loads them in. The other 23 orders
//   all score 0.90 to 0.95.
// FUNCTION: 0x4bf060
int __stdcall DrawPolyline(Surface* surface, Point* points,
                           int count, int color)
{
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            Segment_004bf060 seg;
            seg.from = points;
            seg.to = points + 1;
            int n = count - 1;
            while (n > 0) {
                int x0, y0, x1, y1;
                y1 = seg.to->y;
                x1 = seg.to->x;
                y0 = seg.from->y;
                x0 = seg.from->x;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
                seg.from++;
                seg.to++;
                n--;
            }
            UnlockScreen(&screen);
        }
    } else {
        Segment_004bf060 seg;
        seg.from = points;
        seg.to = points + 1;
        int n = count - 1;
        while (n > 0) {
            int x0, y0, x1, y1;
            y1 = seg.to->y;
            x1 = seg.to->x;
            y0 = seg.from->y;
            x0 = seg.from->x;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                BlitLine(surface, x0, y0, x1, y1, color);
            seg.from++;
            seg.to++;
            n--;
        }
        result = 1;
    }
    return result;
}


//
// Draws a grid of connected lines into `surface`, or into the screen (locked
// with LockScreen, unlocked with UnlockScreen) when `surface` is null:
// `rows` polylines, where counts[r] is the number of vertices of row r and
// the vertices follow one another in `points`. Every segment is clipped by
// ClipLine and drawn by BlitLine. Like 0x4bf060 it returns the lock result
// on the screen path (a failed lock returns 0 without unlocking) and 1 on the
// caller-surface path.
// FUNCTION: 0x4bf260
int __stdcall DrawPolylines(Surface* surface, Point* points, int* counts,
                           int rows, int color)
{
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            // counts is copied before points: the original loads the arguments in that order.
            int r = rows;
            int* c = counts;
            Point* p = points;
            while (r > 0) {
                int j = 0;
                // A while loop, not a for: the latch is counter increment, then pointer bump.
                while (j < *c - 1) {
                    // Declared x0, y0, x1, y1 but assigned y1, x1, y0, x0: fixes the stack slots.
                    int x0, y0, x1, y1;
                    y1 = p[1].y;
                    x1 = p[1].x;
                    y0 = p[0].y;
                    x0 = p[0].x;
                    // Kept: the original's inlined segment drawer has its own null-surface lock.
                    if (&screen == 0) {
                        Surface inner;
                        if (LockScreen(&inner)) {
                            if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                                BlitLine(&inner, x0, y0, x1, y1, color);
                            UnlockScreen(&inner);
                        }
                    } else {
                        if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                            BlitLine(&screen, x0, y0, x1, y1, color);
                    }
                    j++;
                    p++;
                }
                p++;
                c++;
                r--;
            }
            UnlockScreen(&screen);
        }
    } else {
        int r = rows;
        int* c = counts;
        Point* p = points;
        while (r > 0) {
            int j = 0;
            while (j < *c - 1) {
                int x0, y0, x1, y1;
                y1 = p[1].y;
                x1 = p[1].x;
                y0 = p[0].y;
                x0 = p[0].x;
                if (ClipLine(surface, &x0, &y0, &x1, &y1))
                    BlitLine(surface, x0, y0, x1, y1, color);
                j++;
                p++;
            }
            p++;
            c++;
            r--;
        }
        result = 1;
    }
    return result;
}


// Screen fade: applies a 256 entry translate table to every pixel of `rect` in
// `surface` (or in the locked screen when `surface` is 0). `level` selects one
// of 32 fade-in tables at +0xc4 (for negative levels, offset by 32) or one of 32
// fade-out tables at +0xc8.
// FUNCTION: 0x4bf4d0
int __stdcall FadeRectangle(Surface* surface, Rect* rect, int level)
{
    Engine_004bf4d0* engine = (Engine_004bf4d0*)GetDisplay();
    Surface screen;
    Rect r;
    if (surface == 0) {
        if (!LockScreen(&screen))
            return 0;
    } else {
        memcpy(&screen, surface, sizeof(screen));
    }
    if (rect == 0) {
        r.top = 0;
        r.left = 0;
        r.right = engine->width;
        r.bottom = engine->height;
        rect = &r;
    }
    int clipped = ClipRectangle(&screen, rect) != 0;
    if (clipped) {
        // height is computed before p.
        int height = rect->bottom - rect->top + 1;
        char* p = screen.pixels + (screen.width * rect->top + rect->left);
        unsigned char* table;
        unsigned char* t;
        // t is computed in each branch, not once after the merge.
        if (level < 0) {
            if (level < -0x20)
                level = -0x20;
            table = engine->fade_neg;
            level += 0x20;
            if (table == 0)
                return 0;
            t = table + (level << 8);
        } else {
            if (level > 0x1f)
                level = 0x1f;
            table = engine->fade_pos;
            if (table == 0)
                return 0;
            t = table + (level << 8);
        }
        if (t == 0)
            return 0;
        while (height--) {
            // w is declared before next.
            int w = rect->right - rect->left + 1;
            char* next = p + screen.width;
            // Advances p itself in the body, not in a for header.
            while (w--) {
                *p = t[*p];
                p++;
            }
            p = next;
        }
    }
    if (surface == 0)
        UnlockScreen(&screen);
    return 1;
}


// FUNCTION: 0x4bf620
int __stdcall ClipRectangle(Surface* surface, Rect* r)
{
    Rect local;
    surface->GetClipRect(&local);
    if (r->right < local.left)
        return 0;
    if (r->left > local.right)
        return 0;
    if (r->bottom < local.top)
        return 0;
    if (r->top > local.bottom)
        return 0;
    if (r->left < local.left)
        r->left = local.left;
    if (r->top < local.top)
        r->top = local.top;
    if (r->right > local.right)
        r->right = local.right;
    if (r->bottom > local.bottom)
        r->bottom = local.bottom;
    if (r->left > r->right)
        return 0;
    return r->top <= r->bottom;
}



// Fills a rectangle of `surface` with a solid `color` (FillSolidRect does the
// fill). When `surface` is null the screen is locked with LockScreen,
// drawn on and unlocked with UnlockScreen. The rectangle is copied to a local
// first, because ClipRectangle clips it in place. Sibling of 0x4bec70 and
// 0x4bed70.
// FUNCTION: 0x4bf6f0
int __stdcall FillRectangle(Surface* surface, Rect* rect, int color)
{
    Rect r = *rect;
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            result = ClipRectangle(&screen, &r) != 0;
            if (result)
                FillSolidRect(&screen, &r, color);
            UnlockScreen(&screen);
        }
    } else {
        result = ClipRectangle(surface, &r) != 0;
        if (result)
            FillSolidRect(surface, &r, color);
    }
    return result;
}



// Outlines a rectangle (four one-pixel edges), drawing into `surface`, or into
// the screen (locked with LockScreen and unlocked with UnlockScreen) when
// `surface` is null. Returns the surface that was drawn on, so a failed lock
// returns 0 without ever unlocking. Called from 0x4a16f0 with a rect and a
// palette level.
// FUNCTION: 0x4bf7b0
Surface* __stdcall DrawLitRectangle(Surface* surface, int* rect,
                                         int color)
{
    Surface* ret = 0;
    if (surface == 0) {
        Surface screen;
        ret = (Surface*)LockScreen(&screen);
        if (ret) {
            DrawLitLine(&screen, rect[0], rect[1], rect[2], rect[1], color);
            DrawLitLine(&screen, rect[2], rect[1], rect[2], rect[3], color);
            DrawLitLine(&screen, rect[0], rect[3], rect[2], rect[3], color);
            DrawLitLine(&screen, rect[0], rect[1], rect[0], rect[3], color);
            UnlockScreen(&screen);
        }
    } else {
        DrawLitLine(surface, rect[0], rect[1], rect[2], rect[1], color);
        DrawLitLine(surface, rect[2], rect[1], rect[2], rect[3], color);
        DrawLitLine(surface, rect[0], rect[3], rect[2], rect[3], color);
        DrawLitLine(surface, rect[0], rect[1], rect[0], rect[3], color);
    }
    return ret;
}


//
// Draws the outline of `r` (top, right, bottom, left edges in that order) into
// `surface`, or into the screen when `surface` is 0 (locked with
// LockScreen, unlocked with UnlockScreen). Each edge is a segment drawer of the
// same shape as DrawLine (clip with ClipLine, fill with BlitLine).
//
// Suspected original bug: the surface != 0 path returns an uninitialised
// `result`.
// FUNCTION: 0x4bf8c0
int __stdcall DrawRectangle(Surface* surface, Rect* r, int color)
{
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            {
                // Declared first, then assigned y1, x1, y0, x0 as separate statements.
                int x0, y0, x1, y1; y1 = r->top; x1 = r->right; y0 = r->top; x0 = r->left;
                // Kept as in the original: the lock arm tests the address of `screen`.
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->top; x0 = r->right;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->bottom; x0 = r->left;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->left; y0 = r->top; x0 = r->left;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            UnlockScreen(&screen);
        }
    } else {
        {
            int x0, y0, x1, y1; y1 = r->top; x1 = r->right; y0 = r->top; x0 = r->left;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                BlitLine(surface, x0, y0, x1, y1, color);
        }
        {
            int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->top; x0 = r->right;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                BlitLine(surface, x0, y0, x1, y1, color);
        }
        DrawLine(surface, r->left, r->bottom, r->right, r->bottom, color);
        DrawLine(surface, r->left, r->top, r->left, r->bottom, color);
    }
    return result;
}



// Xor-fills `rect` on `surface` (or on the locked screen when `surface` is 0)
// with the byte `value`, through XorRect. The rect is copied to a local
// first because the clip helper ClipRectangle clips it in place. Returns the
// lock result on the screen path and `value` on the caller-surface path.
//
// check.py prints MATCH (167 bytes).
//
// WARNING: the `result = ((int*)&surface)[2];` in the else arm is a codegen
// probe, not a guess at the original's spelling. It reads exactly the same
// four bytes as `value` (the third parameter's stack slot, at surface+8),
// but through a syntactically different expression, and that difference is
// the whole trick: MSVC 5 unifies two reads of the same parameter into one
// register-allocated value (live across the XorRect call, so it needs a
// callee-saved register, hence the extra `push edi`/`pop edi` and the two
// bytes short). A differently spelled read of the same slot stays two
// separate C1 values, so the else arm reloads it into esi, which is the
// shared return-value register here. With the natural `result = value;`
// this function scores 59.0 percent; with `return value;` in an early-return
// else arm it scores 87.6 percent. The original source most likely used some
// spelling this session could not guess.
//
// What the machine code fixes, and what to attack first in any rewrite:
// - one saved register only (`push esi`), so the result variable, `surface`
//   and the lock result all share esi, and the frame is 0x40 with the rect
//   copy at [esp+4] and the 0x30-byte screen at [esp+0x14];
// - a single shared tail: both paths reach `mov eax, esi`, which is why the
//   lock failure branches to 0x4bfdfe and not to its own epilogue, so the
//   result is one function-level variable assigned in both arms with the
//   `return` outside them, not an early `return` per arm;
// - the locked path copies the LockScreen result into esi at once
//   (`mov esi, eax; test esi, esi`), and the locked path's fill argument is
//   reloaded from [esp+0x50] even though the value is also read in the else
//   arm, so a parameter read in two arms is not a register variable.
//
// Tried and none of it moved the else arm: shared vs per-arm result
// variables, per-arm `return`, the value's type (int, unsigned, long, a 4-byte
// struct), a local copy of the value used for the fill and/or the return, a
// `static inline` helper for the fill and for the return, an inline helper
// around the whole else arm, `value` read through its own address
// (`*(int*)&value`, `*(&value)`), comma-operator and no-op-cast spellings,
// `+ 0`, the arms swapped, `do`/`goto` forms, a bare `#include <windows.h>`
// and an unsigned return type. deepseek-v4.1-flash had already established
// that the locked path, the two exit blocks, the 3-argument __stdcall
// signature and the 0x30-byte screen match byte for byte.
// FUNCTION: 0x4bfd60
int __stdcall XorRectangle(Surface* surface, Rect* rect, int value)
{
    Rect r = *rect;
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            if (ClipRectangle(&screen, &r))
                XorRect(&screen, &r, value);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipRectangle(surface, &r))
            XorRect(surface, &r, value);
        result = ((int*)&surface)[2];
    }
    return result;
}



// Translates every pixel of `rect` in `surface` through the byte table at
// g_game+0xcc (RemapRect), or in the locked screen (LockScreen /
// UnlockScreen) when `surface` is 0. The rect is copied to a local first
// because the clip helper ClipRectangle clips it in place. The locked path
// returns the lock result.
//
// check.py prints MATCH (267 bytes).
//
// THE LEVER (Space Bunny Free, #4733). Everything up to here was 74.9% and 2
// bytes short, and every attempt to fix it treated the locked arm's `dst`
// arithmetic as the problem. It is not: the arithmetic was already right. The
// only difference in the locked arm was that MSVC 5 folded the `screen.pixels`
// load into the final add (`add eax, dword ptr [esp + 0x34]`) instead of
// loading it into a register first (`mov edx, [esp + 0x34]; add eax, edx`),
// and that one decision moved the whole function's register allocation, the
// else arm's `lea`, the load order and the 2 bytes at once.
//
// The fold is decided by DECLARATION ORDER, and only that. Declaring the
// locked screen local BEFORE the rect copy, at function scope:
//
//     Surface screen;     <- moved up from inside the `if`
//     Rect r = *rect;
//
// compiles to 267 bytes and every instruction matches, including the whole
// else arm and the tail. It does not move the frame (still `sub esp, 0x40`,
// `screen` still at [esp+0x18] and `r` at [esp+8]), because MSVC 5 allocates
// a local's slot at its first use in the generated code, not at its
// declaration: the rect copy still codegens first, so the layout is unchanged
// and only C1's internal local/NT ordering changes. A micro-probe isolates
// it (build/scratch/0x4bfe10/micro4.cpp, w4 against w5): two declarations that
// generate identical code and an identical frame, differing only in which of
// `S screen;` and `Rect r = *rect;` is written first, produce the two different
// locked arms. Getting the same effect by moving an unused local, `result`,
// `status`, `g`, or the rect copy does NOT work, and neither does splitting
// the copy into four field assignments.
//
// Previous rounds' dead ends, all measured here and all still dead: every
// parenthesisation and every permutation of the three `dst` addends (MSVC 5
// canonicalises `a + b - c`, so all 6 orders are byte-identical); `int`,
// `unsigned`, `long`, `unsigned char*`, `char*`, `void*` and no-cast types for
// `pitch` and `pixels`; a `static inline` getter for either field, for
// `g->field_cc`, and for the whole `dst`; a `static inline` wrapper around the
// call; `dst`, `pitch`, `pixels`, `w`, `h`, `table`, a `Surface*` and a
// `Rect*` each as a local; declaration order of `result`/`status` (12 orders),
// extra dummy locals, and an uninitialised local of each type; `#include
// <windows.h>` and the header sets; `__cdecl`/variadic/extern "C" prototypes
// for RemapRect; and the two wrong-value shapes that reach the original's
// register allocation without being kept (`r.top * screen.pitch + r.left +
// screen.pitch`, 93.3%, and `r.top * (int)screen.pixels + r.left + (int)
// screen.pitch`, 94.9% and 267 bytes). The 94.9% shape matters as evidence:
// putting `pixels` in the multiply and `pitch` in the add forces C1 to
// materialise the pixels load too, which is why it also reaches the original's
// allocation, and it is what pointed at the fold being the real difference.
//
// Suspected original bug: the caller-surface path returns an uninitialised
// local (`status`, warning C4700). `rect` is dead after the copy, so MSVC 5
// puts `status` in rect's parameter slot, which is why the original ends that
// path with `mov esi, [esp + 0x50]` and in practice returns the rect pointer.
// The one `return result;` at the end is what the machine code wants: the
// lock-success path's separate epilogue in the original is MSVC's tail
// duplication, and writing that `return` explicitly moves `push esi` to the
// top. See the matched siblings 0x4be950 and 0x4bfd60 for the same idiom.
// FUNCTION: 0x4bfe10
int __stdcall GrayRectangle(Surface* surface, Rect* rect)
{
    Surface screen;
    Rect r = *rect;
    Game_004bfe10* g = (Game_004bfe10*)GetDisplay();
    if ((g->flags & 1) == 0)
        return 0;
    int result;
    int status;
    if (surface == 0) {
        result = LockScreen(&screen);
        if (result != 0) {
            if (ClipRectangle(&screen, &r))
                RemapRect(r.top * screen.pitch + r.left + (int)screen.pixels,
                             screen.pitch, r.right - r.left + 1,
                             r.bottom - r.top + 1, g->field_cc);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipRectangle(surface, &r))
            RemapRect((int)surface->pixels + r.top * surface->pitch + r.left,
                         surface->pitch, r.right - r.left + 1,
                         r.bottom - r.top + 1, g->field_cc);
        result = status;
    }
    return result;
}



// Clears a dither pattern inside the clipped rectangle `rect` of `surface`, or
// of the screen (locked with LockScreen and unlocked with UnlockScreen) when
// `surface` is null. The rect is copied to a local first because the clip
// helper ClipRectangle clips it in place. Rows alternate between the two byte
// masks (which half of each dword is kept) according to the phase parity
// `(i + phase) & 1`, the aligned middle is done a dword at a time, and the
// unaligned byte ends are filled by stepping 2 bytes at a time. Returns 1 once
// the pattern is laid down, 0 when the screen cannot be locked.
//
// MATCH. The whole match hinged on giving the trailing byte fill ONE shared
// loop after the if/else instead of spelling the loop out in both arms: with
// two copies, `x2` and the loop counter `i` swap their callee-saved registers
// (edi/ebp instead of ebp/edi) and the function comes out 14 bytes too long.
// Sharing the loop between the two `pe`/`pb` assignments fixes the register
// priority as well as the block layout.
// FUNCTION: 0x4bff20
int __stdcall DitherRectangle(Surface* surface, Rect* rect, int phase)
{
    Surface screen;
    if (surface == 0) {
        if (LockScreen(&screen))
            surface = &screen;
        else
            return 0;
    }
    Rect r = *rect;
    if (ClipRectangle(surface, &r)) {
        int x1 = (r.left + 3) & ~3;
        int x2 = (r.right + 3) & ~3;
        for (int i = r.top; i <= r.bottom; i++) {
            unsigned char* base = (unsigned char*)surface->pixels + i * surface->pitch + x1;
            unsigned char* end = base + x2 - x1;
            unsigned char* p = base + ((r.left + i + phase) & 1) + r.left - x1;
            while (p < base) {
                *p = 0;
                p += 2;
            }
            unsigned char* pe;         // last byte of the trailing fill
            unsigned char* pb;         // where that fill starts
            if ((i + phase) & 1) {
                unsigned int* q = (unsigned int*)base;
                unsigned int* qe = (unsigned int*)end;
                while (q < qe) {
                    *q &= 0xff00ff;
                    q++;
                }
                pe = (unsigned char*)q + r.right - x2;
                pb = (unsigned char*)q + 1;
            } else {
                unsigned int* q = (unsigned int*)base;
                unsigned int* qe = (unsigned int*)end;
                while (q < qe) {
                    *q &= 0xff00ff00;
                    q++;
                }
                pe = (unsigned char*)q + r.right - x2;
                pb = (unsigned char*)q;
            }
            while (pb <= pe) {
                *pb = 0;
                pb += 2;
            }
        }
    }
    if (surface == &screen)
        UnlockScreen(&screen);
    return 1;
}


static inline void Draw_004c0070(Surface* surface, int x0, int y0,
                                 int x1, int y1, int color)
{
    if (surface == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                BlitLine(&screen, x0, y0, x1, y1, color);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLine(surface, x0, y0, x1, y1, color);
    }
}


// Draws the outline of a circle of `radius` around (xc, yc) as 32 segments,
// sweeping the angle from 0x800 to 0x10000 in steps of 0x800. The first point
// is (xc + radius, yc), where the sine/cosine tables are 1 and 0, and each
// following point comes from FUN_004b7123 (x) and FUN_004b70ef (y). The
// per-segment draw lives in an inlined helper: it is that helper's parameters
// that get the four coordinate slots, while the loop keeps the carried point
// in registers, which is what produces the slot shuffle at the top of the
// body. When `surface` is null the helper locks the screen with LockScreen
// and unlocks it with UnlockScreen, once per segment; a failed lock draws
// nothing but the loop still runs to completion.
// FUNCTION: 0x4c0070
void __stdcall DrawCircle(Surface* surface, int xc, int yc, int radius,
                            int color)
{
    int x0 = xc + radius;
    int y0 = yc;
    for (int angle = 0x800; angle <= 0x10000; angle += 0x800) {
        int x1 = FUN_004b7123(angle, radius) + xc;
        int y1 = FUN_004b70ef(angle, radius) + yc;
        Draw_004c0070(surface, x0, y0, x1, y1, color);
        x0 = x1;
        y0 = y1;
    }
}



// Draws a dashed circle: sweeps `angle` from `step` to 0x10000 in steps of
// step = 0x10000 / n and draws the segment from the previous point to the
// current point only when the counter `start` is odd (the caller passes 0 or
// 1, see 0x4670fd). FUN_004b7123 / FUN_004b70ef are the sine and cosine table
// lookups (angle, radius); ClipLine clips the segment and BlitLine
// draws it. When `surface` is null the screen is locked with LockScreen and
// unlocked with UnlockScreen.
//
// Three things get this to byte-identical (the 62.0% version had all three
// wrong):
//  - the counter is NOT incremented in the test. The original tests `i & 1`
//    and increments `i` after the per-iteration stores at the loop tail, which
//    also puts i in a frame slot (E-0x38) instead of an argument slot.
//  - `angle` and `step` are both initialised from the division
//    (`int angle = 0x10000 / n; int step = 0x10000 / n;`); the compiler CSEs
//    the idiv and emits `mov ebp, eax; mov [step], eax; cmp ebp, 0x10000`.
//    With `int step = angle` it stores ebp instead and the layout shifts.
//  - the order of the four per-iteration copies (and of the tail statements)
//    drives MSVC 5's store scheduling and scratch-register rotation. Copying
//    y1, x1, y0, x0 (in that order) and writing the tail as px = x; py = y;
//    i++; angle += step; reproduces the original's `mov [esp+0x68], edi` /
//    `mov [esp+0x10], eax` order and the tail's `mov ecx,[step]; mov edx,[i]`.
// FUNCTION: 0x4c01a0
void __stdcall DrawDashedCircle(Surface* surface, int cx, int cy, int radius,
                            int color, int n, int start)
{
    Surface screen;
    int px = cx + radius;
    int py = cy;
    int angle = 0x10000 / n;
    int step = 0x10000 / n;
    int i;
    if (angle > 0x10000)
        return;
    i = start;
    while (angle <= 0x10000) {
        int x = FUN_004b7123(angle, radius) + cx;
        int y = FUN_004b70ef(angle, radius) + cy;
        if (i & 1) {
            int y1 = y;
            int x1 = x;
            int y0 = py;
            int x0 = px;
            if (surface == 0) {
                if (LockScreen(&screen)) {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                    UnlockScreen(&screen);
                }
            } else {
                if (ClipLine(surface, &x0, &y0, &x1, &y1))
                    BlitLine(surface, x0, y0, x1, y1, color);
            }
        }
        px = x;
        py = y;
        i++;
        angle += step;
    }
}


// FUNCTION: 0x4c0310
void __stdcall FillPolygon(Surface* surface, Point* points, int n, unsigned char color)
{
    ScanFillPolygon(surface, points, n, color);
}


// Draws a closed polygon outline: a line between each pair of consecutive
// points, then one from the last point back to the first.
// FUNCTION: 0x4c07b0
int __stdcall DrawPolygon(Surface* surface, Vec3* pts, int count, int color)
{
    for (int i = 0; i < count - 1; i++)
        DrawLine(surface, pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y, color);
    DrawLine(surface, pts[count - 1].x, pts[count - 1].y, pts[0].x, pts[0].y, color);
    return 1;
}


// FUNCTION: 0x4c0820
int __stdcall DrawPolygonEdges(GafFrame* surf, Vec3* pts, int count,
                           int color) {
    // color stays int, not unsigned char: the final call's register pairing.
    Span spans[2048];
    Span* out;
    Vec3* b;
    int x, dx;
    int ymin = 999999;
    int xmax = -999999;
    int ymax = -999999;
    int xmin = 999999;
    int iymin, iymax;
    int y;
    Span* s;
    int i;
    Vec3* p;
    i = 0;
    if (count > 0) {
        p = pts;
        do {
            // Only the first test of each pair uses py/px; the second re-reads the field.
            int py = p->y;
            if (py < ymin) {
                ymin = py;
                iymin = i;
            }
            if (p->y > ymax) {
                ymax = p->y;
                iymax = i;
            }
            int px = p->x;
            if (px > xmax)
                xmax = px;
            if (p->x < xmin)
                xmin = p->x;
            i++;
            p++;
        } while (i < count);
    }
    if (ymax == ymin)
        return 0;
    {
        out = spans;
        int i = iymin;
        do {
            int j = i - 1;
            if (j < 0) {
                j = count;
                j--;
            }
            int y0 = pts[i].y;
            b = &pts[j];
            int y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
                int dy = ((b->z << 16) - y) / h;
                int rows = b->y - y0;
                do {
                    out->x1 = x >> 16;
                    out->z1 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--rows);
            }
            i = i - 1;
            if (i < 0) {
                i = count;
                i--;
            }
        } while (i != iymax);
    }
    {
        int i = iymin;
        out = spans;
        do {
            int j = i + 1;
            if (j >= count)
                j = 0;
            int y0 = pts[i].y;
            b = &pts[j];
            int y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
                int dy = ((b->z << 16) - y) / h;
                int rows = b->y - y0;
                do {
                    out->x2 = x >> 16;
                    out->z2 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--rows);
            }
            i = i + 1;
            if (i >= count)
                i = 0;
        } while (i != iymax);
    }
    {
        s = spans;
        y = ymin;
        while (y < ymax) {
            if (s->x2 - s->x1 > 0)
                PlotSpanEnds(y, s, surf, color);
            s++;
            y++;
        }
    }
    return 1;
}



// Sibling of 0x4c0a90 (which plots the two end points). Fills the pixels
// between the span ends: walks the depth ramp at +0x18 and the shade ramp at
// +0x20 (both 16.16) one step per pixel, does the depth test against the
// surface's depth buffer when it exists, and looks the destination colour up
// in the 32 x 256 shaded palette table at app+0xc4.
//
// MATCH, 352 of 352 bytes (was 55.8 percent, 344 bytes). Claude Sonnet 5.5 pass
// (#694) did four changes:
//  1. Compiler state: the function is very sensitive to it (with N unused
//     `extern int` declarations the score runs between 52 and 62 percent and
//     the size between 334 and 348 bytes, with a period of 32 in N), and
//     `#include <windows.h>` alone (or `<stdio.h>` with `<math.h>`) reaches the
//     state that makes the rest work. headers.py reports the same state for many
//     sets.
//  2. The span width is a named local, `int w = span->x2 - span->x1;` used by both
//     divisions (55.8 to 89.3 percent; x1/x2 named locals, the width written out
//     twice, and reading x2 first all stay at 37 percent). It puts x2 in ebx and
//     x1 in ebp as the original does.
//  3. No `color & 0xff` local: the original reloads and masks the colour argument
//     inside the depth loop (`mov ebx, [esp+0x2c]; and ebx, 0xff`). The old
//     hoisted `int ci = color & 0xff;` was a hack that flipped the ebx/ebp choice
//     of the head; with the width local it is not needed and it costs the frame
//     size (0x10 instead of the original's 0xc).
//  4. `d++` before `z += dz` in the depth loop (96.1 to 98.4 percent; the other
//     23 orders of the four increments score 95 to 97.6).
//
// Resolved (deepseek-v4.1-flash): the last two instructions, the depth pointer
// offset. The original does `add edx, ebp; add edi, edx` (row * pitch, shared
// with the colour pointer, plus start), every spelling that reads the cached
// `start` local gives `add ebp, edx; add edi, ebp` instead. Writing the offset
// as `d += row * surf->pitch + span->x1;` (dereferencing the span rather than
// the cached start, same value because start is span->x1 after the clamp)
// flips the commutative add destination to edx and matches.
//
// Inert: `(app->shade + (si << 8))[color]` and `*(app->shade + (si << 8) + color)`
// for the shade lookup, do/while for the depth loop (73.2 percent), `for (; count;
// count--)`, an `int zi`, a `w`-less head with cached z1/s1/x1/x2 locals
// (47 to 52 percent).
//
// deepseek-v4.1-flash pass: not a compiler-state artefact and not operand
// order. A sweep of 0 to 549 unused `extern int dummyN;` declarations never
// reaches MATCH (only 0.888 to 0.924, worse). For the depth offset, `d +=
// row*pitch; d += start;` in either order, `d += row*pitch + start`, `d +=
// start + row*pitch`, `d = d + row*pitch + start`, `d = (unsigned char*)((int)d
// + ...)`, `&d[...]` and assigning a fresh `q` local all give `add ebp, edx`.
// Only reading `span->x1` in place of the cached local moves the destination
// into edx. An `int off` local changes the frame (4 locals) and drops to 80.8
// percent.
// FUNCTION: 0x4c0b10
void __stdcall FillShadedSpan(int row, Span* span, GafFrame* surf, unsigned char color)
{
    unsigned char* p = surf->bits;
    unsigned char* d = surf->depth;
    App_004c0b10* app = (App_004c0b10*)GetDisplay();
    int w = span->x2 - span->x1;
    int dz = (span->z2 - span->z1) / w;
    int ds = (span->s2 - span->s1) / w;
    if (span->x1 < 0) {
        span->z1 = span->z1 - dz * span->x1;
        span->s1 = span->s1 - ds * span->x1;
        span->x1 = 0;
    }
    if (span->x2 > (int)surf->pitch - 1)
        span->x2 = surf->pitch - 1;
    int start = span->x1;
    int count = span->x2 - start;
    if (count > 0) {
        int z = span->z1;
        int s = span->s1;
        p += row * surf->pitch;
        p += start;
        if (d != 0) {
            d += row * surf->pitch + span->x1;
            while (count--) {
                unsigned char zi = z >> 16;
                if (*d <= zi) {
                    int si = s >> 16;
                    *p = app->shade[(si << 8) + color];
                    *d = zi;
                }
                p++;
                d++;
                z += dz;
                s += ds;
            }
        } else {
            while (count--) {
                int si = s >> 16;
                *p++ = app->shade[(si << 8) + color];
                s += ds;
            }
        }
    }
}


// FUNCTION: 0x4c0c70
int __stdcall FillShadedPolygon(GafFrame* surf, Point_004c0c70* pts, int n,
                           unsigned char color) {
    // Shared by both edge walks; z and shade values stay block-local.
    int y0, y1, x, dx, dz;
    Span spans[2048];
    // Bounds declared in the order maxX, minY, maxY, minX.
    int maxX = -999999;
    int minY = 999999;
    int maxY = -999999;
    int minX = 999999;
    int minYi;
    int maxYi;
    int i;
    GafFrame* sf = surf;
    // Scan index initialised before the n > 0 guard, incremented before p.
    int scanIndex = 0;
    if (n > 0) {
        Point_004c0c70* p = pts;
        do {
            if (p->y < minY) {
                minY = p->y;
                minYi = scanIndex;
            }
            if (p->y > maxY) {
                maxY = p->y;
                maxYi = scanIndex;
            }
            if (p->x > maxX)
                maxX = p->x;
            if (p->x < minX)
                minX = p->x;
            scanIndex++;
            p++;
        } while (scanIndex < n);
    }
    if (minX > (int)sf->pitch - 1)
        return 0;
    if (maxY < 0)
        return 0;
    // Height tested inline here and reused for maxRow: the two loads must CSE.
    if (minY > (int)sf->height - 1)
        return 0;
    int maxRow = (int)sf->height - 1;
    if (minY < 0)
        minY = 0;
    if (maxY > maxRow)
        maxY = maxRow;
    if (maxY == minY)
        return 0;
    {
        Span* sp = spans;
        i = minYi;
        for (;;) {
            int j = i - 1;
            if (j < 0)
                j = n - 1;
            Point_004c0c70* p = &pts[i];
            Point_004c0c70* q = &pts[j];
            y0 = p->y;
            y1 = q->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                x = p->x;
                dx = ((q->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = p->z << 16;
                int s = p->s << 16;
                dz = ((q->z << 16) - z) / dy;
                int ds = ((q->s << 16) - s) / dy;
                if (y0 < 0) {
                    x -= dx * y0;
                    z -= dz * y0;
                    s -= ds * y0;
                    y0 = 0;
                }
                if (y1 > maxRow)
                    y1 = maxRow;
                if (y0 < y1) {
                    int count = y1 - y0;
                    do {
                        sp->x1 = x >> 16;
                        x += dx;
                        sp->z1 = z;
                        sp->s1 = s;
                        z += dz;
                        s += ds;
                        sp++;
                    } while (--count);
                }
            }
            i = i - 1;
            if (i < 0)
                i = n - 1;
            if (i == maxYi)
                break;
        }
        sp = spans;
        i = minYi;
        for (;;) {
            int j = i + 1;
            if (j >= n)
                j = 0;
            Point_004c0c70* p = &pts[i];
            Point_004c0c70* q = &pts[j];
            y0 = p->y;
            y1 = q->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                x = p->x;
                dx = ((q->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = p->z << 16;
                int s = p->s << 16;
                dz = ((q->z << 16) - z) / dy;
                int ds = ((q->s << 16) - s) / dy;
                if (y0 < 0) {
                    x -= dx * y0;
                    z -= dz * y0;
                    s -= ds * y0;
                    y0 = 0;
                }
                if (y1 > maxRow)
                    y1 = maxRow;
                if (y0 < y1) {
                    int count = y1 - y0;
                    do {
                        sp->x2 = x >> 16;
                        x += dx;
                        sp->z2 = z;
                        sp->s2 = s;
                        z += dz;
                        s += ds;
                        sp++;
                    } while (--count);
                }
            }
            i = i + 1;
            if (i >= n)
                i = 0;
            if (i == maxYi)
                break;
        }
    }
    {
        Span* sp = spans;
        for (i = minY; i < maxY; i++) {
            if (sp->x2 - sp->x1 > 0)
                FillShadedSpan(i, sp, surf, color);
            sp++;
        }
    }
    return 1;
}


// Scanline filler, the sibling of 0x4c0c70 (which also fills a shade channel):
// it finds the extreme y vertices, then walks the vertex ring from the top one
// backwards to the bottom one filling the left end of each row, walks it
// forwards for the right end, and hands every row to FillFlatSpan. The 2048
// entry span array is what puts the frame at 0x14028.
// FUNCTION: 0x4c1000
int __stdcall FillFlatPolygon(GafFrame* surf, Vec3* pts, int n, int color)
{
    int y0, y1, x, dx, dz;
    Span spans[2048];
    int maxX = -999999;
    int minY = 999999;
    int maxY = -999999;
    int minX = 999999;
    int minYi;
    int maxYi;
    int i;
    // One function-scope j shared by both walks; each walk has its own block-scoped i.
    int j;
    GafFrame* sf = surf;
    int scanIndex = 0;
    if (n > 0) {
        Vec3* p = pts;
        do {
            if (p->y < minY) {
                minY = p->y;
                minYi = scanIndex;
            }
            if (p->y > maxY) {
                maxY = p->y;
                maxYi = scanIndex;
            }
            if (p->x > maxX)
                maxX = p->x;
            if (p->x < minX)
                minX = p->x;
            scanIndex++;
            p++;
        } while (scanIndex < n);
    }
    if (minX > (int)sf->pitch - 1)
        return 0;
    if (maxY < 0)
        return 0;
    // Height tested inline here and reused for maxRow: the two loads must CSE.
    if (minY > (int)sf->height - 1)
        return 0;
    int maxRow = (int)sf->height - 1;
    if (minY < 0)
        minY = 0;
    if (maxY > maxRow)
        maxY = maxRow;
    if (maxY == minY)
        return 0;
    {
        Span* sp = spans;
        {
            int i = minYi;
            for (;;) {
                j = i - 1;
                if (j < 0)
                    j = n - 1;
                // pts[i] and pts[j] indexed directly: named pointers would re-pack the frame.
                y0 = pts[i].y;
                y1 = pts[j].y;
                if (y0 < y1) {
                    int dy = y1 - y0;
                    x = pts[i].x;
                    dx = ((pts[j].x - x) << 16) / dy;
                    x = (x << 16) + 0xffff;
                    int z = pts[i].z << 16;
                    dz = ((pts[j].z << 16) - z) / dy;
                    if (y0 < 0) {
                        x -= dx * y0;
                        z -= dz * y0;
                        y0 = 0;
                    }
                    if (y1 > maxRow)
                        y1 = maxRow;
                    if (y0 < y1) {
                        int count = y1 - y0;
                        do {
                            sp->x1 = x >> 16;
                            sp->z1 = z;
                            x += dx;
                            z += dz;
                            sp++;
                        } while (--count);
                    }
                }
                i = i - 1;
                if (i < 0)
                    i = n - 1;
                if (i == maxYi)
                    break;
            }
        }
        sp = spans;
        {
            int i = minYi;
            for (;;) {
                j = i + 1;
                if (j >= n)
                    j = 0;
                y0 = pts[i].y;
                y1 = pts[j].y;
                if (y0 < y1) {
                    int dy = y1 - y0;
                    x = pts[i].x;
                    dx = ((pts[j].x - x) << 16) / dy;
                    x = (x << 16) + 0xffff;
                    int z = pts[i].z << 16;
                    dz = ((pts[j].z << 16) - z) / dy;
                    if (y0 < 0) {
                        x -= dx * y0;
                        z -= dz * y0;
                        y0 = 0;
                    }
                    if (y1 > maxRow)
                        y1 = maxRow;
                    if (y0 < y1) {
                        int count = y1 - y0;
                        do {
                            sp->x2 = x >> 16;
                            sp->z2 = z;
                            x += dx;
                            z += dz;
                            sp++;
                        } while (--count);
                    }
                }
                i = i + 1;
                if (i >= n)
                    i = 0;
                if (i == maxYi)
                    break;
            }
        }
    }
    {
        Span* sp = spans;
        for (i = minY; i < maxY; i++) {
            if (sp->x2 - sp->x1 > 0)
                FillFlatSpan(i, sp, surf, color);
            sp++;
        }
    }
    return 1;
}


// Returns 1 if (px, py) lies strictly inside the convex polygon pts[0..n-1]
// (every edge has the point on the same side), 0 otherwise or if n < 3.
// Without a header such as <string.h>, MSVC computes the two products in the
// other order (found with tools/headers.py).
// FUNCTION: 0x4c1320
int __stdcall PointInPolygon(Point* pts, int n, int px, int py)
{
    if (n < 3)
        return 0;
    for (int i = 0; i < n; i++) {
        Point* a = &pts[i];
        Point* b = &pts[(i + 1) % n];
        if ((b->y - a->y) * (px - a->x) <= (b->x - a->x) * (py - a->y))
            return 0;
    }
    return 1;
}


// FUNCTION: 0x4c13a0
void __stdcall SetTextColors(int param_1, int param_2)
{
    void* eax = (void*)GetDisplay();
    if (param_1 != -1) {
        *(int*)((unsigned char*)eax + 0x208) = param_1;
    }
    if (param_2 != -1) {
        *(int*)((unsigned char*)eax + 0x20c) = param_2;
    }
}


// Stores a value at +0x210 of the object GetDisplay returns; the setter
// for the getter GetTextKeyColor.
// FUNCTION: 0x4c13d0
void __stdcall SetTextKeyColor(int value)
{
    Obj_004c13d0* obj = (Obj_004c13d0*)GetDisplay();
    obj->keyColor = value;
}


// FUNCTION: 0x4c13f0
int GetTextKeyColor()
{
    Obj* obj = (Obj*)GetDisplay();
    return obj->keyColor;
}


// FUNCTION: 0x4c1400
int GetTextForeColor()
{
    Obj* obj = (Obj*)GetDisplay();
    return obj->foreColor;
}


// FUNCTION: 0x4c1410
int GetTextBackColor()
{
    Obj* obj = (Obj*)GetDisplay();
    return obj->backColor;
}


// FUNCTION: 0x4c1420
void __stdcall SetFont(int param_1)
{
    if (param_1 != 0) {
        int eax = GetDisplay();
        *(int*)(eax + 0x204) = param_1;
    }
}


// FUNCTION: 0x4c1440
int GetFont()
{
    return *(int*)(GetDisplay() + 0x204);
}


// FUNCTION: 0x4c1450
int GetFontHeight()
{
    int temp = GetDisplay();
    int ptr = *(int*)(temp + 0x204);
    unsigned int result = 0;
    result = *(unsigned char*)ptr;
    return result;
}


// FUNCTION: 0x4c1470
unsigned char __stdcall FontHeight(void* ptr)
{
    unsigned int result = 0;
    result = *(unsigned char*)ptr;
    return (unsigned char)result;
}
