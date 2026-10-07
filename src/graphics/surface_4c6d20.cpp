// Decompiled by Opus, space-bunny-free, DeepSeek V4.1 Flash, deepseek-v4.1-flash, GPT-6.1-sol, mimo-v2.6-pro, Space Bunny Free, deepseek-v4.1, muse-spark-1.3-free, GPT-6, claude-sonnet-5-5, claude-opus-5-5, fledge-alpha-free, Haiku, Sonnet and Claude Opus 5.5. Names are provisional.
// Draws surfaces through the hand-written blitters and the textured span and
// polygon renderers, and the reference-counted string handle.
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
// Only for its symbol ids: DrawTexturedSpan and DrawLitTexturedSpan match only
// in a window of the symbol count (docs/c2-regalloc.md, "Symbol ids").
#include <malloc.h>
#include "../util/hapi_bank.h"

// A rectangle: the surfaces' clip rect and the blitters' source rect.
struct Rect {
    int left;
    int top;
    int right;
    int bottom;
    Rect() {}
    Rect(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
};

struct Point {
    int x;
    int y;
};

class Surface;
int __stdcall UnlockScreen(Surface* s);

// A surface header, 0x30 bytes; the pixels of an allocated image follow it.
class Surface {
public:
    int width;                         // +0x0
    int height;                        // +0x4
    int field_8;                       // +0x8
    char* pixels;                      // +0xc
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    short field_18;                    // +0x18
    short field_1a;                    // +0x1a
    Rect clip;                         // +0x1c
    unsigned int flag0 : 1;            // +0x2c bit 0
    unsigned int flag1 : 1;            // +0x2c bit 1

    Rect* GetClipRect(Rect* out);
    void Unlock() { UnlockScreen(this); }
};

struct Screen {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    // Unlock through a method of this struct: the tested pointer gets copied.
    void UnlockSurface() { surface->Unlock(0); }
};

struct Display {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen screen;                     // +0x80
    char unknown_90[0xc4 - 0x90];
    unsigned char* palette;            // +0xc4
    char unknown_c8[0xdc - 0xc8];
    int field_dc;                      // +0xdc
};

extern int g_screenLockCount;

Display* GetDisplay(void);
int __stdcall LockScreen(Surface* out);
void __cdecl BlitRect(Surface* dst, Surface* src, Rect* rect, Point* pos);
void __cdecl BlitRectKeyed(Surface* dst, Surface* src, Rect* rect, Point* pos, unsigned char transparent);
void __cdecl BlitTile32x32(Surface* dst, Surface* src, Rect* rect, Point* pos);

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline(Surface* s)
{
    Display* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// Draws through the blitter at 0x4cbdd1 (hand-written assembly in a gap
// region) into `dst`, or, when `dst` is null, into the screen: lock it with
// LockScreen, draw, then unlock.
// FUNCTION: 0x4c6d20
void __stdcall CopySurfaceRect(Surface* dst, Surface* src, Rect* rect, Point* pos)
{
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            BlitRect(&screen, src, rect, pos);
            UnlockScreenInline(&screen);
        }
    } else {
        BlitRect(dst, src, rect, pos);
    }
}

// Sibling of 0x4c6d20 / 0x4c6e70, drawing with the cdecl blitter at 0x4cbe70
// (which takes one extra byte argument, the transparent colour). Draws into
// `dst`, or, when `dst` is null, into the screen: lock it with LockScreen,
// draw, then unlock.
// FUNCTION: 0x4c6dc0
void __stdcall CopySurfaceRectKeyed(Surface* dst, Surface* src, Rect* rect, Point* pos, unsigned char transparent)
{
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            BlitRectKeyed(&screen, src, rect, pos, transparent);
            UnlockScreenInline(&screen);
        }
    } else {
        BlitRectKeyed(dst, src, rect, pos, transparent);
    }
}

// Sibling of 0x4c6d20 using the blitter at 0x4cbef1 (hand-written assembly
// in the gap region starting at 0x4cbbe0): draws into `dst`, or, when `dst`
// is null, into the screen: lock it with LockScreen, draw, then unlock.
// FUNCTION: 0x4c6e70
void __stdcall DrawTile(Surface* dst, Surface* src, Rect* rect, Point* pos)
{
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            BlitTile32x32(&screen, src, rect, pos);
            UnlockScreenInline(&screen);
        }
    } else {
        BlitTile32x32(dst, src, rect, pos);
    }
}

// Writes one radar surface into the chunked file writer: an 8-byte header of
// width and height, then one row per scan line. The read counterpart is
// 0x4c6f80, which allocates a surface of width*height and reads the rows back.
// The surface layout matches the one built by 0x4c69f0.
// Must stay: <ddraw.h> decides the multiply operand order.
// FUNCTION: 0x4c6f10
void __stdcall SaveSurface(Surface* surface, HapiBank* file)
{
    ((HapiBank*)file)->SeekBox(0);
    int header[2];
    header[0] = surface->width;
    header[1] = surface->height;
    file->WriteBox(header, 8);
    for (int i = 0; i < header[1]; i++) {
        file->WriteBox(surface->pixels + i * surface->field_8, header[0]);
    }
}

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* ptr);

// Allocation and header set-up (the body of 0x4c6a60), kept inline.
static inline Surface* NewSurface(char* name, int w, int h)
{
    Surface* s = (Surface*)FUN_004d83b0(name, h * w + 0x30);
    s->width = w;
    s->field_8 = w;
    s->height = h;
    s->field_18 = 0;
    s->pixels = (char*)(s + 1);
    s->flag0 = 1;
    s->flag1 = 0;
    s->field_1a = 0;
    s->field_10 = 10000;
    s->field_14 = -1;
    s->clip = Rect(0, 0, w - 1, h - 1);
    return s;
}

// Reads back a surface written by 0x4c6f10: an 8-byte header of width and
// height, then one row per scan line into a freshly allocated surface.
// Must stay: <ddraw.h> decides the operand order of the row-loop multiply.
// FUNCTION: 0x4c6f80
Surface* __stdcall LoadSurface(void* file)
{
    ((HapiBank*)file)->SeekBox(0);
    int header[2];
    if (((HapiBank*)file)->ReadBox(header, 8) < 8u) {
        return 0;
    }
    Surface* s = NewSurface("Loaded Surface", header[0], header[1]);
    for (int i = 0; i < header[1]; i++) {
        if (((HapiBank*)file)->ReadBox(s->pixels + i * s->field_8, header[0]) < header[0]) {
            FUN_004d85a0(s);
            return 0;
        }
    }
    return s;
}

// GLOBAL: 0x51fe48
extern int DAT_0051fe48[];
// GLOBAL: 0x51fea0
extern int DAT_0051fea0[];

// FUNCTION: 0x4c7080
void __stdcall FUN_004c7080(int count)
{
    for (int i = 0; i < count; i++) {
        if (i == 0) {
            DAT_0051fe48[0] = count - 1;
        } else {
            DAT_0051fe48[i] = i - 1;
        }
        if (i == count - 1) {
            DAT_0051fea0[i] = 0;
        } else {
            DAT_0051fea0[i] = i + 1;
        }
    }
}

// GLOBAL: 0x51fef0
extern int DAT_0051fef0;

struct Chunk {
    int field_0;
    int field_4;
};

struct Range {
    int low;
    int high;
};

// GLOBAL: 0x51fef8
extern Chunk* DAT_0051fef8;
// GLOBAL: 0x51fefc
extern int DAT_0051fefc;
// GLOBAL: 0x51ff00
extern int DAT_0051ff00;

int __cdecl FUN_004b7381(int a, int b, int c);

// FUNCTION: 0x4c70d0
void __stdcall FUN_004c70d0(int value, Range* out, int at_low, int at_high)
{
    int i = DAT_0051ff00;
    Chunk* table = DAT_0051fef8;
    int j = DAT_0051fe48[i];
    DAT_0051fefc = j;
    while (value > table[j].field_4) {
        j = DAT_0051fe48[j];
        i = DAT_0051fe48[i];
        DAT_0051fefc = j;
        DAT_0051ff00 = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    DAT_0051fef0 = size;
    if (size == 0) {
        return;
    }
    switch (i) {
    case 0: {
        // The distance is passed through a pointer to a local. It reads like a
        // leftover from the original, but it is what makes cl 5 schedule case 0
        // the way the original does.
        int span = size - offset;
        int* spanp = &span;
        out->low = 0;
        out->high = FUN_004b7381(at_high, *spanp, DAT_0051fef0);
        return; }
    case 1:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = 0;
        return;
    case 2:
        out->low = at_low;
        out->high = FUN_004b7381(at_high, offset, DAT_0051fef0);
        return;
    case 3:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = at_high;
        return;
    }
}

// GLOBAL: 0x51fe98
extern int DAT_0051fe98;
// GLOBAL: 0x51fef4
extern int DAT_0051fef4;
// GLOBAL: 0x51fe40
extern int DAT_0051fe40;

// FUNCTION: 0x4c71f0
void __stdcall FUN_004c71f0(int value, Range* out, int at_low, int at_high)
{
    int i = DAT_0051fe98;
    Chunk* table = DAT_0051fef8;
    int j = DAT_0051fea0[i];
    DAT_0051fef4 = j;
    while (value > table[j].field_4) {
        j = DAT_0051fea0[j];
        i = DAT_0051fea0[i];
        DAT_0051fef4 = j;
        DAT_0051fe98 = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    DAT_0051fe40 = size;
    if (size == 0) {
        return;
    }
    switch (i) {
    case 0:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = 0;
        return;
    case 1:
        out->low = at_low;
        {
            // The distance is passed through a pointer to a local. It reads
            // like a leftover from the original, but it is what puts case 1's
            // global reload in the 5-byte accumulator form.
            int span = size - offset;
            int* spanp = &span;
            out->high = FUN_004b7381(at_high, *spanp, DAT_0051fe40);
        }
        return;
    case 2:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = at_high;
        return;
    case 3:
        out->low = 0;
        out->high = FUN_004b7381(at_high, offset, DAT_0051fe40);
        return;
    }
}

// Scales a (possibly skewed) source rect onto a destination surface one column
// at a time. It asks the surface for its clip rect, moves the left edge of the
// destination rect forward to it (advancing the two fixed-point source steps by
// the same amount, so the source follows the clip), clips the right edge, and
// then blits: 8 bits per pixel is an inline loop, 0x10/0x20/0x40/0x80 go to
// helpers, and anything else is an inline loop with a per-row byte stride.
struct Info_4c7310 {
    unsigned short bits;
    char unknown_2[0x10 - 2];
    unsigned char* data;
};

void __cdecl BlitSpan128(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl BlitSpan64(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl BlitSpan32(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl BlitSpan16(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);

// FUNCTION: 0x4c7310
void __stdcall DrawQuadRow(int param_1, int* rect, Surface* surf, Info_4c7310* info)
{
    unsigned char* dest = (unsigned char*)surf->pixels;
    unsigned char* src = info->data;
    int rowstep = (rect[4] - rect[2]) / (rect[1] - rect[0]);
    int colstep = (rect[5] - rect[3]) / (rect[1] - rect[0]);
    Rect bounds;
    int width;
    int y;
    int x;

    surf->GetClipRect(&bounds);
    // bounds.left - rect[0] is repeated inline in both updates, no skip temporary.
    if (rect[0] < bounds.left) {
        rect[2] += rowstep * (bounds.left - rect[0]);
        rect[3] += colstep * (bounds.left - rect[0]);
        rect[0] = bounds.left;
    }
    if (rect[1] > bounds.right)
        rect[1] = bounds.right;
    width = rect[1] - rect[0];
    if (width > 0) {
        y = rect[2];
        x = rect[3];
        dest += surf->field_8 * param_1 + rect[0];
        switch (info->bits) {
        case 0x80:
            BlitSpan128(dest, src, width, y, x, rowstep, colstep);
            return;
        case 0x40:
            BlitSpan64(dest, src, width, y, x, rowstep, colstep);
            return;
        case 0x20:
            BlitSpan32(dest, src, width, y, x, rowstep, colstep);
            return;
        case 0x10:
            BlitSpan16(dest, src, width, y, x, rowstep, colstep);
            return;
        case 8: {
            int n = width;
            do {
                *dest++ = src[(y >> 16) + ((x >> 13) & ~7)];
                y += rowstep;
                x += colstep;
            } while (--n);
            break; }
        default: {
            int n = width;
            do {
                *dest++ = src[(y >> 16) + (x >> 16) * info->bits];
                y += rowstep;
                x += colstep;
            } while (--n);
            break; }
        }
    }
}

struct Quad_004c7580 {
    Point p[4];
};

struct Rec_004c7580 {
    int left;   // +0x00
    int right;  // +0x04
    int tx;     // +0x08
    int ty;     // +0x0c
    int trx;    // +0x10
    int try_;   // +0x14
    int pad[4];
};

struct Frame_004c7580 {
    unsigned short w;    // +0x00
    unsigned short h;    // +0x02
    char unknown_04[0xc];
    void* data;          // +0x10
};

void __stdcall DrawQuadRow(int y, int* rect, void* surf, void* info);

// FUNCTION: 0x4c7580
void __stdcall DrawFrameQuad(void* surf, Frame_004c7580* bmp,
                            Quad_004c7580* dst, Quad_004c7580* src)
{
    if (bmp == 0)
        return;
    if (dst == 0)
        return;

    Surface local;
    int locked;
    if (surf == 0) {
        int ok = LockScreen(&local);
        if (ok == 0)
            return;
        locked = 1;
        surf = &local;
    } else {
        locked = 0;
    }

    Quad_004c7580 tmp;
    if (src == 0) {
        src = &tmp;
        tmp.p[0].x = 0;
        tmp.p[0].y = 0;
        tmp.p[1].x = bmp->w - 1;
        tmp.p[1].y = 0;
        tmp.p[2].x = bmp->w - 1;
        tmp.p[2].y = bmp->h - 1;
        tmp.p[3].x = 0;
        tmp.p[3].y = bmp->h - 1;
    }

    int y0, y1;
    Rec_004c7580 recs[800];
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    Rect clip;
    int imin, imax;
    int i;
    Rec_004c7580* out;
    int j, k;
    int dxdy;
    int x, tx, ty, dtx, dty;

    for (i = 0; i < 4; i++) {
        int y = dst->p[i].y;
        if (y < ymin) {
            ymin = y;
            imin = i;
        }
        if (y > ymax) {
            ymax = y;
            imax = i;
        }
        if (dst->p[i].x > xmax)
            xmax = dst->p[i].x;
        int xx = dst->p[i].x;
        if (xx < xmin)
            xmin = xx;
    }

    ((Surface*)surf)->GetClipRect(&clip);
    if (xmax < clip.left) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (xmin > clip.right) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (ymax < clip.top) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (ymin > clip.bottom) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (ymin < clip.top)
        ymin = clip.top;
    if (ymax > clip.bottom)
        ymax = clip.bottom;
    if (ymax == ymin) {
        // Method wrapper, not UnlockScreen(&local): keeps this tail from merging with check 2.
        if (locked) local.Unlock();
        return;
    }

    out = recs;
    i = imin;
    for (;;) {
        j = k = i - 1;
        if (k < 0)
            k = 3;
        y0 = dst->p[i].y;
        y1 = dst->p[k].y;
        if (y1 > clip.top && y0 < y1) {
            int dy = y1 - y0;
            x = dst->p[i].x;
            dxdy = ((dst->p[k].x - x) << 16) / dy;
            x = (x << 16) + 0xffff;
            tx = src->p[i].x << 16;
            ty = src->p[i].y << 16;
            dtx = ((src->p[k].x << 16) - tx) / dy;
            dty = ((src->p[k].y << 16) - ty) / dy;
            if (y0 < clip.top) {
                x += dxdy * (clip.top - y0);
                tx += dtx * (clip.top - y0);
                ty += dty * (clip.top - y0);
                y0 = clip.top;
            }
            if (y1 > clip.bottom)
                y1 = clip.bottom;
            if (y0 < y1) {
                int n = y1 - y0;
                do {
                    out->left = x >> 16;
                    out->tx = tx;
                    out->ty = ty;
                    x += dxdy;
                    tx += dtx;
                    ty += dty;
                    out++;
                } while (--n);
            }
        }
        i = i - 1;
        if (i < 0)
            i = 3;
        if (i == imax)
            break;
    }

    out = recs;
    i = imin;
    for (;;) {
        j = (i + 1) & 3;
        k = j;
        y0 = dst->p[i].y;
        y1 = dst->p[k].y;
        if (y1 > clip.top && y0 < y1) {
            int dy = y1 - y0;
            x = dst->p[i].x;
            dxdy = ((dst->p[k].x - x) << 16) / dy;
            x = (x << 16) + 0xffff;
            tx = src->p[i].x << 16;
            ty = src->p[i].y << 16;
            dtx = ((src->p[k].x << 16) - tx) / dy;
            dty = ((src->p[k].y << 16) - ty) / dy;
            if (y0 < clip.top) {
                x += dxdy * (clip.top - y0);
                tx += dtx * (clip.top - y0);
                ty += dty * (clip.top - y0);
                y0 = clip.top;
            }
            if (y1 > clip.bottom)
                y1 = clip.bottom;
            if (y0 < y1) {
                int n = y1 - y0;
                do {
                    out->right = x >> 16;
                    out->trx = tx;
                    out->try_ = ty;
                    x += dxdy;
                    tx += dtx;
                    ty += dty;
                    out++;
                } while (--n);
            }
        }
        i = (i + 1) & 3;
        if (i == imax)
            break;
    }

    out = recs;
    for (i = ymin; i < ymax; i++) {
        if (out->right - out->left > 0)
            DrawQuadRow(i, (int*)out, surf, bmp);
        out++;
    }

    if (locked) {
unlock:
        UnlockScreen(&local);
    }
}

// MATCH. Preserve the native 0x80 case fallthrough into the 0x40 scaler.
struct Info_4c7a20 {
    unsigned short bits;          // +0x00
    char gap_2[0xe];
    unsigned char* data;          // +0x10
};

struct Surf_4c7a20 {
    unsigned short pitch;         // +0x00
    char gap_2[0xe];
    unsigned char* pixels;        // +0x10
    unsigned char* mask;          // +0x14
};

// FUNCTION: 0x4c7a20
void __stdcall DrawTexturedSpan(int row, int* span, Surf_4c7a20* surf, Info_4c7a20* info)
{
    unsigned char* mask = surf->mask;
    unsigned char* dest = surf->pixels;
    unsigned char* src = info->data;
    int width = span[1] - span[0];
    int rowstep = (span[4] - span[2]) / width;
    int colstep = (span[5] - span[3]) / width;
    int dstep = (span[7] - span[6]) / width;

    if (span[0] < 0) {
        span[2] -= rowstep * span[0];
        span[3] -= colstep * span[0];
        int clippedDepth = span[6] - dstep * span[0];
        span[0] = 0;
        span[6] = clippedDepth;
    }
    if (span[1] > surf->pitch - 1)
        span[1] = surf->pitch - 1;
    width = span[1] - span[0];
    if (width > 0) {
        int y = span[2];
        int x = span[3];
        int w = span[6];

        dest += surf->pitch * row + span[0];
        if (mask != 0) {
            mask += surf->pitch * row + span[0];
            switch (info->bits) {
            case 0x80: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 9) & ~0x7f)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 0x40: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 10) & ~0x3f)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 0x20: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 11) & ~0x1f)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 0x10: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 12) & ~0xf)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 8: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 13) & ~7)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            default: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 16) * info->bits)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            }
        }
        switch (info->bits) {
        case 0x80:
            BlitSpan128(dest, src, width, y, x, rowstep, colstep);
        case 0x40:
            BlitSpan64(dest, src, width, y, x, rowstep, colstep);
            return;
        case 0x20:
            BlitSpan32(dest, src, width, y, x, rowstep, colstep);
            return;
        case 0x10:
            BlitSpan16(dest, src, width, y, x, rowstep, colstep);
            return;
        case 8: {
            int n = width;
            do {
                *dest++ = src[(y >> 16) + ((x >> 13) & ~7)];
                y += rowstep;
                x += colstep;
            } while (--n);
            return; }
        default: {
            int n = width;
            do {
                *dest++ = src[(y >> 16) + ((x >> 16) * info->bits)];
                y += rowstep;
                x += colstep;
            } while (--n);
            return; }
        }
    }
}

// Blits one textured span with a depth (Z) buffer. For each pixel it compares
// the interpolated Z against the depth buffer and, when it passes, writes the
// palette-mapped texel and the new depth value. Source coordinates are 16.16
// fixed-point; the u/v/z/light steps are the span deltas divided by the span
// width. The span is clipped to the left edge and to the target surface's
// width. The texture width selects the texel layout: 8/16/32/64/128 pick the
// sub-texel mask, anything else uses the width itself as a row stride. With no
// depth buffer the four power-of-two formats dispatch to the FUN_004cd8xx span
// helpers and the remaining formats run inline loops.
struct Surface_004c8020 {
    unsigned short width;
    char pad[14];
    unsigned char* data;
    unsigned char* depth;
};

// FUNCTION: 0x4c8020
void __stdcall DrawLitTexturedSpan(int row, int* span, Surface_004c8020* target, Surface_004c8020* texture)
{
    unsigned char* dest = target->data;
    unsigned char* depth = target->depth;
    unsigned char* src = texture->data;
    Display* display = GetDisplay();
    int width = span[1] - span[0];
    int du = (span[4] - span[2]) / width;
    int dv = (span[5] - span[3]) / width;
    int dz = (span[7] - span[6]) / width;
    int dl = (span[9] - span[8]) / width;
    if (span[0] < 0) {
        span[2] -= du * span[0];
        span[3] -= dv * span[0];
        span[6] -= dz * span[0];
        span[8] -= dl * span[0];
        span[0] = 0;
    }
    if (span[1] > target->width - 1) span[1] = target->width - 1;
    width = span[1] - span[0];
    if (width > 0) {
        int u = span[2];
        int v = span[3];
        int z = span[6];
        int light = span[8];
        // One temporary shared by every body: keeps the loop counters in the original slots.
        int value;
        dest += target->width * row + span[0];
        if (depth) {
            depth += target->width * row + span[0];
            switch(texture->width) {
            case 128: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 9) & ~127)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 64: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 10) & ~63)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 32: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 11) & ~31)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 16: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 12) & ~15)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            case 8: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + ((v >> 13) & ~7)];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            default: {
                int n = width;
                do {
                    value = z >> 16;
                    if (*depth <= (unsigned char)value) {
                        unsigned int pixel = 0;
                        pixel = src[(u >> 16) + (v >> 16) * texture->width];
                        *dest = display->palette[pixel + ((light >> 16) * 256)];
                        *depth = (unsigned char)value;
                    }
                    v += dv; z += dz; light += dl;
                    dest++; depth++; u += du;
                } while (--n);
                return;
            }
            }
        }
        switch(texture->width) {
        case 128:
            BlitSpan128(dest, src, width, u, v, du, dv);
        case 64:
            BlitSpan64(dest, src, width, u, v, du, dv);
            return;
        case 32:
            BlitSpan32(dest, src, width, u, v, du, dv);
            return;
        case 16:
            BlitSpan16(dest, src, width, u, v, du, dv);
            return;
        case 8: {
            int n=width;
            do {
                *dest++ = src[(u >> 16) + ((v >> 13) & ~7)];
                u += du; v += dv;
            } while (--n);
            return;
        }
        default: {
            int n=width;
            do {
                *dest++ = src[(u >> 16) + (v >> 16) * texture->width];
                u += du; v += dv;
            } while (--n);
            return;
        }
        }
    }
}

struct Surface_4c8760 { unsigned short width, height; };
void __stdcall DrawTexturedSpan(int, int*, Surface_4c8760*, Surface_4c8760*);

// FUNCTION: 0x4c8760
void __stdcall DrawTexturedPolygon(Surface_4c8760* target, Surface_4c8760* texture, int* vertices, int* coords)
{
    int i, defaults[8];
    int spans[800][10];
    // Declared before the slopes: dx*y0 then loads dx first.
    int y0;
    int dv, dx, du;
    int lowY, highY, highX, lowX;
    int lowIndex, highIndex;
    if (target && texture && vertices) {
            if (!coords) {
                coords=defaults;
                defaults[0]=0; defaults[1]=0;
                defaults[2]=texture->width-1; defaults[3]=0;
                defaults[4]=texture->width-1; defaults[5]=texture->height-1;
                defaults[6]=0; defaults[7]=texture->height-1;
            }
            lowY=999999; highX=-999999; highY=-999999; lowX=999999;
            i = 0;
            while (i<4) {
                int y=vertices[i*3+1];
                if(y<lowY) { lowY=y; lowIndex=i; }
                if(y>highY) { highY=y; highIndex=i; }
                int x=vertices[i*3];
                if(x>highX) highX=x;
                if(x<lowX) lowX=x;
                i = i + 1;
            }
            if (highX>=0 && lowX<=target->width-1 && highY>=0) {
                if (lowY<=target->height-1) {
                    int bottom=target->height-1;
                    if(lowY<0) lowY=0;
                    if(highY>bottom) highY=bottom;
                    if(highY!=lowY) {
                        int y1; int x;
                        int* out;
                        int* nextVertex;
                        {
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                // Declared per walk body, not shared: sets the frame slot packing.
                                int next=index-1;
                                if (next<0) next=3;
                                int* currentVertex=vertices+index*3;
                                y0=currentVertex[1];
                                nextVertex=vertices+next*3;
                                y1=nextVertex[1];
                                if (y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
                                    x=currentVertex[0]*0x10000+0xffff;
                                    int z=currentVertex[2]*0x10000;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    int dz = (nextVertex[2]*0x10000-z)/dy;
                                    if(y0<0) {
                                        x-=y0*dx; u-=y0*du; v-=y0*dv; z-=y0*dz;
                                        y0=0;
                                    }
                                    if(y1>bottom) y1=bottom;
                                    if(y0<y1) {
                                        int n=y1-y0;
                                        do {
                                            out[0]=x>>16; out[2]=u;
                                            x+=dx;
                                            out[3]=v;
                                            out[6]=z;
                                            u+=du;
                                            v+=dv; out+=10;
                                            z+=dz;
                                        } while(--n);
                                    }
                                }
                                index--;
                                if(index<0) index=3;
                            } while(index!=highIndex);
                        }
                        {
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                int next=(index+1)&3;
                                int* currentVertex=vertices+index*3;
                                y0=currentVertex[1];
                                nextVertex=vertices+next*3;
                                y1=nextVertex[1];
                                if (y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
                                    x=currentVertex[0]*0x10000+0xffff;
                                    int z=currentVertex[2]*0x10000;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    int dz = (nextVertex[2]*0x10000-z)/dy;
                                    if (y0 < 0) {
                                        x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;
                                        y0=0;
                                    }
                                    if(y1>bottom) y1=bottom;
                                    if(y0<y1) {
                                        int n=y1-y0;
                                        do {
                                            out[1]=x>>16; x+=dx;
                                            out[4]=u;
                                            out[5]=v;
                                            out[7]=z;
                                            u+=du;
                                            v+=dv; z+=dz;
                                            out+=10;
                                        } while(--n);
                                    }
                                }
                                // Recomputed, not index=next: a copy changes the compares and loads.
                                index=(index+1)&3;
                            } while(index!=highIndex);
                        }
                        int* span=&spans[0][0];
                        for(int row=lowY;row<highY;row++) {
                            if(span[1]-span[0]>0)
                                DrawTexturedSpan(row,span,target,texture);
                            span+=10;
                        }
                    }
                }
            }
        }
}

struct Surface_4c8bb0 { unsigned short width, height; };
void __stdcall DrawLitTexturedSpan(int, int*, Surface_4c8bb0*, Surface_4c8bb0*);

// FUNCTION: 0x4c8bb0
void __stdcall DrawLitTexturedPolygon(Surface_4c8bb0* target, Surface_4c8bb0* texture, int* vertices, int* coords)
{
    int i, defaults[8];
    int spans[800][10];
    int y0;
    int dv, dx, du, dz, dl;
    int lowY, highY, highX, lowX;
    int lowIndex, highIndex;
    if (target && texture && vertices) {
            if (!coords) {
                coords=defaults;
                defaults[0]=0; defaults[1]=0;
                defaults[2]=texture->width-1; defaults[3]=0;
                defaults[4]=texture->width-1; defaults[5]=texture->height-1;
                defaults[6]=0; defaults[7]=texture->height-1;
            }
            lowY=999999; highX=-999999; highY=-999999; lowX=999999;
            i = 0;
            while (i<4) {
                int y=vertices[i*4+1];
                if(y<lowY) { lowY=y; lowIndex=i; }
                if(y>highY) { highY=y; highIndex=i; }
                int x=vertices[i*4];
                if(x>highX) highX=x;
                if(x<lowX) lowX=x;
                i = i + 1;
            }
            if (highX>=0 && lowX<=target->width-1 && highY>=0) {
                if (lowY<=target->height-1) {
                    int bottom=target->height-1;
                    if(lowY<0) lowY=0;
                    if(highY>bottom) highY=bottom;
                    if(highY!=lowY) {
                        int y1; int x;
                        int* out;
                        int* nextVertex;
                        // One variable shared by both walks: a per-walk next moves six frame slots.
                        int next;
                        {
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                next=index-1;
                                if (next<0) next=3;
                                y0=vertices[index*4+1];
                                nextVertex=vertices+next*4;
                                // Read through vertices, not nextVertex: makes the address a shared subexpression.
                                y1=vertices[next*4+1];
                                if (y1>0 && y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-vertices[index*4+0])*0x10000)/dy;
                                    x=vertices[index*4+0]*0x10000+0xffff;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    int z=vertices[index*4+2]*0x10000;
                                    int l=vertices[index*4+3]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    dz=(nextVertex[2]*0x10000-z)/dy;
                                    dl=(nextVertex[3]*0x10000-l)/dy;
                                    if(y0<0) {
                                        x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; l-=dl*y0;
                                        y0=0;
                                    }
                                    if(y1>bottom) y1=bottom;
                                    if(y0<y1) {
                                        int n=y1-y0;
                                        do {
                                            out[0]=x>>16; x+=dx; out[2]=u; u+=du; out[3]=v; v+=dv; out[6]=z; out[8]=l; z+=dz; out+=10; l+=dl;
                                        } while(--n);
                                    }
                                }
                                index--;
                                if(index<0) index=3;
                            } while(index!=highIndex);
                        }
                        {
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                next=(index+1)&3;
                                y0=vertices[index*4+1];
                                nextVertex=vertices+next*4;
                                y1=vertices[next*4+1];
                                if (y1>0 && y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-vertices[index*4+0])*0x10000)/dy;
                                    x=vertices[index*4+0]*0x10000+0xffff;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    int z=vertices[index*4+2]*0x10000;
                                    int l=vertices[index*4+3]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    dz=(nextVertex[2]*0x10000-z)/dy;
                                    dl=(nextVertex[3]*0x10000-l)/dy;
                                    if(y0<0) {
                                        x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; l-=dl*y0;
                                        y0=0;
                                    }
                                    if(y1>bottom) y1=bottom;
                                    if(y0<y1) {
                                        int n=y1-y0;
                                        do {
                                            out[1]=x>>16; x+=dx; out[4]=u; u+=du; out[5]=v; out[7]=z; v+=dv; out[9]=l; z+=dz; out+=10; l+=dl;
                                        } while(--n);
                                    }
                                }
                                index=(index+1)&3;
                            } while(index!=highIndex);
                        }
                        int* span=&spans[0][0];
                        for(int row=lowY;row<highY;row++) {
                            if(span[1]-span[0]>0)
                                DrawLitTexturedSpan(row,span,target,texture);
                            span+=10;
                        }
                    }
                }
            }
        }
}

// Append of the reference-counted string handle: concatenates other's
// characters onto this handle's, allocating a new block whose first int is
// the reference count, then releasing the old block. The handle points at the
// characters; the count is the int just before them (see 0x4c91a0, 0x4c9290,
// 0x4c93b0, 0x4c93f0). The class is named after this address because it has
// no name in data/symbols.csv.
class Class_004c90b0 {
public:
    char* ptr;              // refcount lives in the dword before ptr

    bool IsEmpty() const { return *ptr == 0; }

    Class_004c90b0* Append(const Class_004c90b0& other);
};

// FUNCTION: 0x4c90b0
Class_004c90b0* Class_004c90b0::Append(const Class_004c90b0& other)
{
    // Bool helper, not a plain pointer test: gives the original emptiness-test code.
    if (!other.IsEmpty()) {
        // Declared before the strlen locals: sets the second strcpy destination encoding.
        char* chars;
        int n = (int)strlen(ptr);
        int m = (int)strlen(other.ptr);
        int len = n + m + 1;
        int* lp = &len;                   // opaque store: keeps the +1 and +5 apart
        *lp += 5;
        int* block = (int*)malloc(len);
        *block = 1;
        chars = (char*)(block + 1);
        strcpy(chars, ptr);
        strcpy(chars + n, other.ptr);
        ((int*)ptr)[-1]--;
        int* old = (int*)ptr - 1;
        if (((int*)ptr)[-1] == 0) {
            free(old);
        }
        ptr = chars;
    }
    return this;
}

extern int g_emptyStringRefs;
extern void* g_emptyString;

class Class_004c9180
{
public:
    void* vtable;
    Class_004c9180();
};

// FUNCTION: 0x4c9180
Class_004c9180::Class_004c9180()
{
    g_emptyStringRefs++;
    vtable = &g_emptyString;
}

// Copy constructor of a reference-counted string handle: the handle points at
// character data whose reference count is stored just before it. The
// assignment operator of the same handle is at 0x4c93b0.
class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

// FUNCTION: 0x4c91a0
Class_004c91a0::Class_004c91a0(const Class_004c91a0& other)
{
    ptr = other.ptr;
    ((int*)ptr)[-1]++;
}

// Constructor of the reference-counted string handle from a C string (see
// 0x4c9180 for the default constructor, 0x4c91a0 for the copy constructor and
// 0x4c93b0 for assignment). The handle points at the characters; the
// reference count is the int just before them. An empty or null string shares
// the global empty string, whose count is g_emptyStringRefs. The class is named
// after this address because data/symbols.csv maps one name per constructor
// (Class_004c91a0::Class_004c91a0 is already the copy constructor).
class Class_004c91b0 {
public:
    char* ptr;

    Class_004c91b0(const char* text);
};

// FUNCTION: 0x4c91b0
Class_004c91b0::Class_004c91b0(const char* text)
{
    char* chars;
    if (text == 0 || *text == 0) {
        g_emptyStringRefs++;
        chars = (char*)&g_emptyString;
    } else {
        int* block = (int*)malloc(strlen(text) + 1 + sizeof(int));
        *block = 1;
        chars = (char*)(block + 1);
        strcpy(chars, text);
    }
    ptr = chars;
}

// Constructor of the reference-counted string handle (see 0x4c91b0) from the
// first len characters of a string. A null string shares the global empty
// string, whose count is g_emptyStringRefs. The class is named after this address
// because data/symbols.csv maps one name per constructor.
class Class_004c9230 {
public:
    char* ptr;

    Class_004c9230(const char* text, int len);
};

// FUNCTION: 0x4c9230
Class_004c9230::Class_004c9230(const char* text, int len)
{
    if (text == 0) {
        g_emptyStringRefs++;
        ptr = (char*)&g_emptyString;
    } else {
        int* block = (int*)malloc(len + 1 + sizeof(int));
        *block = 1;
        char* chars = (char*)(block + 1);
        strncpy(chars, text, len);
        chars[len] = 0;
        ptr = chars;
    }
}

class Class_004c9290 {
public:
    char* data;              // refcount lives in the dword before data

    char* GetUnique()
    {
        int len = (int)strlen(data);
        if (*(int*)(data - 4) != 1) {
            int* block = (int*)malloc(len + 5);
            *block = 1;
            char* copy = (char*)(block + 1);
            strcpy(copy, data);
            (*(int*)(data - 4))--;
            if (*(int*)(data - 4) == 0) {
                free(data - 4);
            }
            data = copy;
            return copy;
        }
        return data;
    }

    Class_004c9290* MakeLower();
};

// FUNCTION: 0x4c9290
Class_004c9290* Class_004c9290::MakeLower()
{
    _strlwr(GetUnique());
    return this;
}

class Class_004c9310 {
public:
    char* data;              // refcount lives in the dword before data

    char* GetUnique()
    {
        int len = (int)strlen(data);
        if (*(int*)(data - 4) != 1) {
            int* block = (int*)malloc(len + 5);
            *block = 1;
            char* copy = (char*)(block + 1);
            strcpy(copy, data);
            (*(int*)(data - 4))--;
            if (*(int*)(data - 4) == 0) {
                free(data - 4);
            }
            data = copy;
            return copy;
        }
        return data;
    }

    Class_004c9310* MakeUpper();
};

// FUNCTION: 0x4c9310
Class_004c9310* Class_004c9310::MakeUpper()
{
    _strupr(GetUnique());
    return this;
}

// Reference-count decrement and free for a reference-counted string handle
// (see the copy constructor at 0x4c91a0 and assignment at 0x4c93b0, which
// share the same shape). data/symbols.csv already names this address and
// class independently (Class_004c9390::ReleaseRef), and every existing
// caller (map_list.cpp, 0x432c00.cpp, 0x488a00.cpp, 0x4b75d0.cpp) already
// calls it that way as a plain method, so that established name is kept
// here rather than renamed to Class_004c91a0::~Class_004c91a0.

extern "C" void __cdecl free(void*);

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

// FUNCTION: 0x4c9390
void Class_004c9390::ReleaseRef()
{
    ((int*)data)[-1]--;
    int* p = (int*)data - 1;
    if (((int*)data)[-1] == 0) {
        free(p);
    }
}

// Assignment of a C string to the reference-counted string handle (see
// 0x4c91b0 for the constructor from a C string and 0x4c93b0 for assignment
// from another handle): releases the old characters, then shares the global
// empty string or copies the text into a new block whose first int is the
// reference count.
class Class_004c93f0 {
public:
    char* ptr;

    Class_004c93f0* AssignText(const char* text);
};

// FUNCTION: 0x4c93f0
Class_004c93f0* Class_004c93f0::AssignText(const char* text)
{
    // The release, phrased as in the destructor body 0x4c9390.
    ((int*)ptr)[-1]--;
    int* old = (int*)ptr - 1;
    if (((int*)ptr)[-1] == 0)
        free(old);
    char* chars;
    if (text == 0 || *text == 0) {
        g_emptyStringRefs++;
        chars = (char*)&g_emptyString;
    } else {
        int* block = (int*)malloc(strlen(text) + 1 + sizeof(int));
        *block = 1;
        chars = (char*)(block + 1);
        strcpy(chars, text);
    }
    ptr = chars;
    return this;
}

// Substring of the reference-counted string handle (see 0x4c9180 for the
// default constructor and 0x4c9230 for the constructor from the first len
// characters of a string): returns a new handle for ptr[start..end), with
// start clamped to 0 and end to the string length. The class is named after this address.
class Class_004c9490 {
public:
    char* ptr;

    Class_004c9490()
    {
        g_emptyStringRefs++;
        ptr = (char*)&g_emptyString;
    }
    Class_004c9490(const char* text, int len)
    {
        if (text == 0) {
            g_emptyStringRefs++;
            ptr = (char*)&g_emptyString;
        } else {
            int* block = (int*)malloc(len + 1 + sizeof(int));
            *block = 1;
            char* chars = (char*)(block + 1);
            strncpy(chars, text, len);
            chars[len] = 0;
            ptr = chars;
        }
    }

    Class_004c9490 SubString(int start, int end) const;
};

// FUNCTION: 0x4c9490
Class_004c9490 Class_004c9490::SubString(int start, int end) const
{
    int len = strlen(ptr);
    if (start < 0)
        start = 0;
    if (end > len)
        end = len;
    if (start >= end)
        return Class_004c9490();
    return Class_004c9490(ptr + start, end - start);
}
