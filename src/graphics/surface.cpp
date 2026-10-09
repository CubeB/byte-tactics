// Decompiled by Opus, Haiku, GPT-5.6-Terra, Sonnet 5.5, deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, Space Bunny Free, claude-sonnet-5-5, GPT-6, claude-opus-5-5, space-bunny-free, Sonnet, DeepSeek V4.1 Flash, mimo-v2.6-pro, muse-spark-1.3-free, fledge-alpha-free and Claude Opus 5.5. Names are provisional.
// The display object's DirectDraw surfaces, the screen lock stack, the
// hand-written blitters, the textured span and polygon renderers, and the
// reference-counted string handle. The module's two files gathered in address
// order.
#include <windows.h>
#include <string.h>
#include <ddraw.h>

#include "surface.h"
#include "gaf_frame.h"

// Unused here: real declarations that keep the file's symbol count (docs/c2-regalloc.md).
void StepAllGafSequences(void);
void StartScreenFade(void);

// The display flags word at +0xf0: set through the bitfield, read whole
// (FlipScreen) or as its low byte (FillSurface).
union Flags_004c61b0 {
    unsigned short word;
    unsigned char byte;
    struct {
        unsigned short flag0 : 1;
        unsigned short flag1 : 1;
        unsigned short bit2_15 : 14;
    } bits;
};

// The display's screen surfaces at +0x80: the primary at +0x8, the locked
// surface at +0xc.
struct Screen {
    char unknown_0[0x8];
    IDirectDrawSurface* primary;       // +0x8
    IDirectDrawSurface* surface;       // +0xc
    char unknown_10[0x18 - 0x10];

    // Lock in a one-expression method: gives the separate surface reload.
    int LockSurface(DDSURFACEDESC* p) { return surface->Lock(0, p, 0x801, 0); }
    int LockPrimary(DDSURFACEDESC* p) { return primary->Lock(0, p, DDLOCK_WAIT, 0); }
    void UnlockPrimary() { primary->Unlock(0); }
    void UnlockSurface() { surface->Unlock(0); }
    void UnlockRect(LPRECT p) { surface->Unlock(p); }
};

// One entry of the screen lock stack: a surface and the flag saying which
// unlock to use (10 entries at g_screenLocks).
#pragma pack(push, 1)
struct LockEntry {
    Surface* surface;                  // +0x0
    char flag;                         // +0x4
};
#pragma pack(pop)

extern int g_screenLockCount;
extern LockEntry g_screenLocks[];

// The display object GetDisplay returns. Packed: rect_x sits at +0x196,
// which the compiler would otherwise pad to a dword boundary.
#pragma pack(push, 1)
struct Display {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    HDC offscreenDC;                   // +0x44
    HDC sourceDC;                      // +0x48
    HPALETTE hpalette;                 // +0x4c
    Surface cached;                    // +0x50
    Screen screen;                     // +0x80
    Surface* activeSurface;            // +0x98
    int haveBackBuffer;                // +0x9c
    Rect vec;                          // +0xa0
    char unknown_b0[0xb4 - 0xb0];
    IDirectDrawSurface* lastBackBuffer;  // +0xb4
    char unknown_b8[0xbc - 0xb8];
    Surface* overrideSurface;          // +0xbc
    char unknown_c0[0xc4 - 0xc0];
    unsigned char* palette;            // +0xc4
    char unknown_c8[0xd4 - 0xc8];
    int width;                         // +0xd4
    int height;                        // +0xd8
    int useOverrideSurface;            // +0xdc
    char unknown_e0[0xe4 - 0xe0];
    int field_e4;                      // +0xe4
    char unknown_e8[0xf0 - 0xe8];
    Flags_004c61b0 flags;              // +0xf0
    char unknown_f2[0x196 - 0xf2];
    int rect_x;                        // +0x196
    int rect_y;                        // +0x19a
    char unknown_19e[0x1b2 - 0x19e];
    GafFrame* bmp;                     // +0x1b2
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    Surface* saveMouse1;               // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int cursorThreadEnabled;           // +0x1ce
    int cursorOverlayEnabled;          // +0x1d2

    // Method of the display object: this is the call result.
    int LockScreen(Surface* out)
    {
        if (useOverrideSurface != 0) {
            *out = *overrideSurface;
            return 1;
        }
        if (offscreenDC != 0) {
            *out = cached;
            return 1;
        }
        if (screen.surface == 0)
            return 0;
        DDSURFACEDESC desc;
        desc.dwSize = sizeof(desc);
        // One-case switch: tests the result with test eax, eax.
        switch (screen.LockSurface(&desc)) { case 0: break; default: return 0; }
        out->width = width;
        out->height = height;
        out->pitch = desc.lPitch;
        out->pixels = (unsigned char*)desc.lpSurface;
        out->x = 0;
        out->y = 0;
        out->zPriority = 10000;
        out->colorKey = -1;
        out->flag0 = 0;
        out->clip = vec;
        if (g_screenLockCount < 10) {
            g_screenLocks[g_screenLockCount].surface = out;
            g_screenLocks[g_screenLockCount].flag = 0;
        }
        return 1;
    }

    // Method of the display object: this is the call result.
    int LockPrimary(Surface* out)
    {
        if (offscreenDC != 0) {
            *out = cached;
            return 1;
        }
        if (screen.primary == 0)
            return 0;
        DDSURFACEDESC desc;
        desc.dwSize = sizeof(desc);
        // One-case switch: tests the result with test eax, eax.
        switch (screen.LockPrimary(&desc)) { case 0: break; default: return 0; }
        out->width = width;
        out->height = height;
        out->pitch = desc.lPitch;
        out->pixels = (unsigned char*)desc.lpSurface;
        out->x = 0;
        out->y = 0;
        out->zPriority = 10000;
        out->colorKey = -1;
        out->flag0 = 0;
        out->clip = vec;
        return 1;
    }
};
#pragma pack(pop)

// COM interface (slot 27 = +0x6c is the tested method).
class Intf_004c6210 {
public:
    virtual int __stdcall Slot00();
    virtual int __stdcall Slot01();
    virtual int __stdcall Slot02();
    virtual int __stdcall Slot03();
    virtual int __stdcall Slot04();
    virtual int __stdcall Slot05();
    virtual int __stdcall Slot06();
    virtual int __stdcall Slot07();
    virtual int __stdcall Slot08();
    virtual int __stdcall Slot09();
    virtual int __stdcall Slot10();
    virtual int __stdcall Slot11();
    virtual int __stdcall Slot12();
    virtual int __stdcall Slot13();
    virtual int __stdcall Slot14();
    virtual int __stdcall Slot15();
    virtual int __stdcall Slot16();
    virtual int __stdcall Slot17();
    virtual int __stdcall Slot18();
    virtual int __stdcall Slot19();
    virtual int __stdcall Slot20();
    virtual int __stdcall Slot21();
    virtual int __stdcall Slot22();
    virtual int __stdcall Slot23();
    virtual int __stdcall Slot24();
    virtual int __stdcall Slot25();
    virtual int __stdcall Slot26();
    virtual int __stdcall Slot27();
};

struct Arg_004c6210 {
    int unknown_0;                     // +0x0
    Intf_004c6210* field_4;            // +0x4
    Intf_004c6210* field_8;            // +0x8
};

Display* GetDisplay(void);
int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall UnlockPrimary(Surface* unused, RECT* r1, RECT* r2);
void __cdecl BlitSurface(Surface* dst, Surface* src, int x, int y);
void __cdecl BlitSurfaceKeyed(Surface* dst, Surface* src, int x, int y, int color);
void __stdcall DrawSurface(Surface* dst, Surface* bmp, int x, int y);
void __stdcall DrawFrame(Surface* dst, GafFrame* bmp, int x, int y);
void __stdcall DrawCursor(Display* obj, Surface* dst);
int GetScreenWidth(void);
int GetScreenHeight(void);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);

// Records a surface and flag in the top slot of the screen lock stack
// (count g_screenLockCount, at most 10 entries at g_screenLocks).
// FUNCTION: 0x4c5d90
void __stdcall SetLockEntry(Surface* surface, char flag)
{
    if (g_screenLockCount < 10) {
        g_screenLocks[g_screenLockCount].surface = surface;
        g_screenLocks[g_screenLockCount].flag = flag;
    }
}

// FUNCTION: 0x4c5dc0
void __stdcall PopScreenLock(int param_1, int param_2)
{
    if (g_screenLockCount > 0) {
        g_screenLockCount--;
    }
}

// FUNCTION: 0x4c5de0
int GetScreenLockCount(void)
{
    return g_screenLockCount;
}

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline(Surface* s)
{
    Display* d = GetDisplay();
    if (d->offscreenDC == 0 && d->useOverrideSurface == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// Pops every entry of the screen lock stack (count g_screenLockCount, entries
// written by LockScreen at g_screenLocks), unlocking each one. The screen
// unlock is UnlockScreen inlined.
// FUNCTION: 0x4c5df0
void UnlockAllScreens(void)
{
    while (g_screenLockCount > 0) {
        int i = g_screenLockCount;
        if (g_screenLocks[g_screenLockCount].flag)
            UnlockPrimary(g_screenLocks[g_screenLockCount - 1].surface, 0, 0);
        else
            UnlockScreenInline(g_screenLocks[g_screenLockCount - 1].surface);
        if (i == g_screenLockCount)
            g_screenLockCount--;
    }
}

// Locks the DirectDraw surface and fills a 0x30-byte surface descriptor,
// registering it in the screen lock stack (g_screenLockCount entries at
// g_screenLocks) so UnlockAllScreens can unlock it later. When the display is
// already locked (+0xdc) or using the cached descriptor (+0x44) it copies
// the cached block instead.
// The original's callers call this out of line.
#pragma auto_inline(off)
// FUNCTION: 0x4c5e70
int __stdcall LockScreen(Surface* out)
{
    return GetDisplay()->LockScreen(out);
}
#pragma auto_inline(on)

// Unlocks the screen surface locked by LockScreen. The Unlock call goes
// through a method of the embedded screen struct, which is why the surface
// pointer is re-read.
// The original's callers (Surface::Unlock in DrawFrameQuad) call this out of
// line.
#pragma auto_inline(off)
// FUNCTION: 0x4c5fa0
int __stdcall UnlockScreen(Surface* s)
{
    Display* d = GetDisplay();
    if (d->offscreenDC == 0 && d->useOverrideSurface == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}
#pragma auto_inline(on)

// Fills a 0x30-byte surface descriptor. When the GDI offscreen path is active
// (field 0x44 of the display object is set) it copies the cached descriptor at
// +0x50; otherwise it locks the primary DirectDraw surface, builds the
// descriptor from the locked DDSURFACEDESC and from the block at +0xa0, and
// clears bit 0 of the flag at +0x2c.
// FUNCTION: 0x4c5ff0
int __stdcall LockPrimary(Surface* out)
{
    return GetDisplay()->LockPrimary(out);
}

// Blits two clip rectangles from the offscreen GDI surface to the window, or,
// in DirectDraw mode, unlocks the primary surface. The first argument is not
// used by the body.
// FUNCTION: 0x4c60d0
int __stdcall UnlockPrimary(Surface* unused, RECT* r1, RECT* r2)
{
    Display* d = GetDisplay();
    if (d->offscreenDC != 0) {
        HDC dc = GetDC(d->hwnd);
        SelectPalette(dc, d->hpalette, FALSE);
        RealizePalette(dc);
        if (r1 != 0) {
            BitBlt(dc, r1->left, r1->top, r1->right - r1->left,
                   r1->bottom - r1->top, d->sourceDC, r1->left, r1->top,
                   SRCCOPY);
        }
        if (r2 != 0) {
            BitBlt(dc, r2->left, r2->top, r2->right - r2->left,
                   r2->bottom - r2->top, d->sourceDC, r2->left, r2->top,
                   SRCCOPY);
        }
        ReleaseDC(d->hwnd, dc);
        return 1;
    }
    if (d->screen.primary == 0) {
        return 0;
    }
    d->screen.UnlockPrimary();
    return 1;
}

// Sets bit 0 of the display flags word at +0xf0; returns 0 only when the
// bit is set while the field at +0x9c is still zero.
// FUNCTION: 0x4c61b0
int __stdcall SetPageFlipping(int enable)
{
    Display* obj = GetDisplay();
    obj->flags.bits.flag0 = enable;
    if (enable && obj->haveBackBuffer == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: 0x4c61f0
void __stdcall SetRestoreSurface(int param_1)
{
    Display* obj = GetDisplay();
    obj->activeSurface = (Surface*)param_1;
}

// Locks the screen into a local surface, blits the bitmap at display+0x98
// into it at (0, 0), then unlocks. Both objects at arg+4 and arg+8 are
// COM-like interfaces; slot 27 (+0x6c) is tested first and its non-zero
// result aborts the blit.
// FUNCTION: 0x4c6210
int __stdcall RestoreSurfaces(Arg_004c6210* arg)
{
    Display* d = GetDisplay();
    if (d->offscreenDC != 0)
        return 0;

    int r = arg->field_4->Slot27();
    if (r == 0) {
        r = arg->field_8->Slot27();
        if (r == 0) {
            Surface screen;
            LockScreen(&screen);
            BlitSurface(&screen, d->activeSurface, r, r);

            Display* d2 = GetDisplay();
            if (d2->offscreenDC == 0 && d2->useOverrideSurface == 0 && d2->screen.surface != 0) {
                d2->screen.UnlockRect((LPRECT)r);
                if (g_screenLockCount > 0)
                    g_screenLockCount--;
            }
        }
    }
    return r;
}

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline()
{
    Display* d = GetDisplay();
    if (d->offscreenDC == 0 && d->useOverrideSurface == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// Frame recovery for a lost DirectDraw surface: when bit 1 of the display
// flags at +0xf0 is set, check whether the primary surface at +0x88 reports
// DDERR_SURFACELOST, restore the two surfaces at screen+0x8 and screen+0xc,
// lock the screen with LockScreen, blit the object at +0x98 onto it with
// BlitSurface, then unlock with the +0x80 method. The screen surface at +0x8c
// is saved to +0xb4 and +0xdc is always cleared.
// FUNCTION: 0x4c62c0
void RestoreScreen(void)
{
    Display* d = GetDisplay();
    if (d->flags.bits.flag1) {
        if (d->screen.primary->IsLost() == DDERR_SURFACELOST) {
            Display* d2 = GetDisplay();
            if (d2->offscreenDC == 0) {
                if (d->screen.primary->Restore() == 0) {
                    if (d->screen.surface->Restore() == 0) {
                        Surface screen;
                        LockScreen(&screen);
                        BlitSurface(&screen, d2->activeSurface, 0, 0);
                        UnlockScreenInline();
                    }
                }
            }
        }
        d->lastBackBuffer = d->screen.surface;
    }
    d->useOverrideSurface = 0;
}

extern LONG g_gfxBlitLockHeld;
extern LONG g_gfxBlitLockOwner;
extern HANDLE g_gfxBlitLockEvent;

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&g_gfxBlitLockHeld, 0x4d41494e);
        if (r == 0) {
            g_gfxBlitLockOwner = 0x4d41494e;
            return 0;
        }
        if (g_gfxBlitLockOwner == 0x4d41494e)
            return r;
        WaitForSingleObject(g_gfxBlitLockEvent, INFINITE);
    }
}

static inline void Unlock(LONG held)
{
    if (held == 0) {
        g_gfxBlitLockOwner = 0;
        InterlockedExchange(&g_gfxBlitLockHeld, 0);
        SetEvent(g_gfxBlitLockEvent);
    }
}

// Restores both surfaces after DDERR_SURFACELOST and redraws the screen.
// Must stay an inline helper used in both places: gives matching registers.
static inline HRESULT RestoreSurfacesInline(Display* d)
{
    Display* dd = GetDisplay();
    if (dd->offscreenDC != 0)
        return 0;
    HRESULT hr = d->screen.primary->Restore();
    if (hr == 0) {
        hr = d->screen.surface->Restore();
        if (hr == 0) {
            Surface screen;
            LockScreen(&screen);
            BlitSurface(&screen, dd->activeSurface, 0, 0);
            UnlockScreenInline();
        }
    }
    return hr;
}

struct Desc {
    DWORD dwSize, dwFlags, height, width;
    LONG lPitch;
    DWORD backbuffers, mipmaps, alpha, reserved;
    void* lpSurface;
    char fields[68];
};

// Presents a frame. With flag bit 1 clear it blits the cached bitmap through
// GDI. Otherwise, when +0xdc is set, it copies the bitmap at +0xbc (if its
// size matches GetScreenWidth/GetScreenHeight) into the locked primary surface;
// else it flips, or Blts the back surface to the window's client rect,
// retrying after DDERR_SURFACELOST.
// FUNCTION: 0x4c63a0
void FlipScreen(void)
{
    Display* d = GetDisplay();
    unsigned short flags = d->flags.word;

    if ((flags & 2) == 0) {
        LONG held = Lock();
        Surface* p = &d->cached;
        DrawSurface(p, d->overrideSurface, 0, 0);
        DrawCursor(d, p);
        HDC hdc = GetDC(d->hwnd);
        SelectPalette(hdc, d->hpalette, 0);
        RealizePalette(hdc);
        BitBlt(hdc, 0, 0, p->width, d->cached.height, d->sourceDC, 0, 0, SRCCOPY);
        ReleaseDC(d->hwnd, hdc);
        Unlock(held);
        return;
    }

    if (d->useOverrideSurface != 0) {
        Desc desc;
        Surface out;
        Surface* bmp = d->overrideSurface;
        if (bmp->width != GetScreenWidth())
            return;
        if (bmp->height != GetScreenHeight())
            return;

        LONG held = Lock();
        desc.dwSize = sizeof(desc);
        unsigned long lr = d->screen.primary->Lock(0, (DDSURFACEDESC*)&desc, 1, 0);
        if (lr == 0) {
            out.width = d->width;
            out.height = d->height;
            out.pitch = desc.lPitch;
            out.pixels = (unsigned char*)desc.lpSurface;
            DrawCursor(d, bmp);
            BlitSurface(&out, bmp, 0, 0);
            if (d->cursorThreadEnabled != 0 && d->cursorOverlayEnabled != 0)
                DrawSurface(bmp, d->saveMouse1, d->x, d->y);
            d->screen.primary->Unlock(0);
        } else if (lr == 0x887601c2) {
            RestoreSurfacesInline(d);
        }
        Unlock(held);
        return;
    }

    if (d->haveBackBuffer != 0 && (flags & 1) != 0) {
        d->screen.primary->Flip(0, 1);
        return;
    }

    {
    RECT rect;
    POINT pt;
    GetClientRect(d->hwnd, &rect);
    pt.x = 0;
    pt.y = 0;
    RECT src = rect;
    ClientToScreen(d->hwnd, &pt);
    OffsetRect(&rect, pt.x, pt.y);

    int hr;
    while (1) {
        hr = d->screen.primary->Blt(&rect, d->screen.surface, &src, 0x1000000, 0);
        if (hr == 0)
            return;
        if (hr != 0x887601c2)
            continue;
        hr = RestoreSurfacesInline(d);
        if (hr != 0)
            continue;
        return;
    }
    }
}

// Same input/mouse object family as 0x4c2870 (see that file): when the two
// enable flags at +0x1ce and +0x1d2 are set and a bitmap at +0x1b2 exists,
// fills the destination rectangle at +0x1be from the bitmap's size, offsets
// the object by the bitmap's half size, blits it from `dst` and then draws
// the bitmap at the stored rectangle.
// FUNCTION: 0x4c67c0
void __stdcall DrawCursor(Display* obj, Surface* dst)
{
    if (obj->cursorThreadEnabled != 0 && obj->cursorOverlayEnabled != 0 && obj->bmp != 0) {
        obj->saveMouse1->width = obj->bmp->width;
        obj->saveMouse1->height = obj->bmp->height;
        obj->saveMouse1->pitch = obj->bmp->width;
        obj->x = obj->rect_x - obj->bmp->xOffset;
        obj->y = obj->rect_y - obj->bmp->yOffset;
        DrawSurface(obj->saveMouse1, dst, -obj->x, -obj->y);
        DrawFrame(dst, obj->bmp, obj->rect_x, obj->rect_y);
    }
}

// Only for its symbol ids: FillSurface matches only in a window of the
// symbol count (docs/c2-regalloc.md, "Symbol ids"). It also declares the
// memset the fills below call.
#include <memory.h>

// What the driver is handed: 0x64 bytes, of which two fields are set.
struct Fade_004c6890 {
    int amount;                        // +0x0
    char unknown_4[0x50 - 0x4];
    int colour;                        // +0x50
    char unknown_54[0x64 - 0x54];      // never touched
};

// The driver's table of __stdcall function pointers, each of which takes the
// driver object itself as its first argument.
struct Table_004c6890 {
    void* slot0;                        // +0x00
    void* slot1;                        // +0x04
    void* slot2;                        // +0x08
    void* slot3;                        // +0x0c
    void* slot4;                        // +0x10
    int (__stdcall* fade)(void* self, int a, int b, int c, int colour, Fade_004c6890* fade);  // +0x14
};

struct Driver_004c6890 {
    Table_004c6890* table;              // +0x0
};

// The three fills are written through these little helpers because the order in
// which the count and the fill value are evaluated decides which of the two
// count operands the inlined imul takes, and the original has a different
// order in each of the three branches.
static void set_mem(unsigned char* p, int count, int colour)
{
    memset(p, colour, count);
}

static void clear_surface(Surface* s, int colour)
{
    memset(s->pixels, colour, s->height * s->pitch);
}

static void clear_screen(Display* o, int colour)
{
    memset(o->cached.pixels, colour, o->cached.height * o->cached.pitch);
}

// Fills a surface with one colour byte. With no surface it uses the one the
// screen object was given by SetOffscreenSurface (+0xbc, when +0xdc is set), else its
// own framebuffer, unless bit 1 of the flags byte at +0xf0 sends the work to
// the driver's function table, which then takes the object as its first
// argument. Returns 0 only when that call reports a failure.
// FUNCTION: 0x4c6890
int __stdcall FillSurface(Surface* surface, int colour)
{
    Display* obj = GetDisplay();
    int ret = 1;
    if (surface == 0) {
        if (obj->useOverrideSurface != 0) {
            Surface* s = obj->overrideSurface;
            set_mem(s->pixels, s->height * s->pitch, colour);
        } else if (!(obj->flags.byte & 2)) {
            clear_screen(obj, colour);
        } else {
            Fade_004c6890 fade;
            fade.amount = 100;
            fade.colour = colour;
            if (((Driver_004c6890*)obj->screen.surface)->table->fade(
                    (Driver_004c6890*)obj->screen.surface, 0, 0, 0, 0x1000400, &fade) != 0)
                ret = 0;
            return ret;
        }
    } else if (surface->flag0) {
        clear_surface(surface, colour);
    } else {
        Fade_004c6890 fade;
        fade.amount = 100;
        fade.colour = colour;
        if (((Driver_004c6890*)obj->screen.surface)->table->fade(
                (Driver_004c6890*)obj->screen.surface, 0, 0, 0, 0x1000400, &fade) != 0)
            ret = 0;
    }
    return ret;
}

// FUNCTION: 0x4c69a0
void __stdcall SetOffscreenSurface(int param_1)
{
    Display* d = GetDisplay();
    d->useOverrideSurface = 1;
    d->overrideSurface = (Surface*)param_1;
}

// FUNCTION: 0x4c69c0
void __stdcall ResetClipRect(int* param_1)
{
    Rect q;
    q.left = 0;
    q.top = 0;
    q.right = param_1[0] - 1;
    q.bottom = param_1[1] - 1;
    *(Rect*)(param_1 + 7) = q;
}

// Allocates an image of width x height bytes with its header (pixels follow
// the 0x30-byte header); the header set-up is the body of InitSurface,
// inlined.
static inline void Init(Surface* s, int width, int height, int a, int b)
{
    s->width = width;
    s->height = height;
    s->pitch = a;
    s->pixels = (unsigned char*)b;
    s->x = 0;
    s->y = 0;
    s->zPriority = 10000;
    s->colorKey = -1;
    s->flag0 = 1;
    s->flag1 = 0;
    s->clip = Rect(0, 0, width - 1, height - 1);
}

// FUNCTION: 0x4c69f0
Surface* __stdcall AllocSurface(char* name, int width, int height)
{
    Surface* s = (Surface*)GameAllocIgnoreTag(name, height * width + 0x30);
    Init(s, width, height, width, (int)(s + 1));
    return s;
}

// FUNCTION: 0x4c6a60
void __stdcall InitSurface(Surface* s, int width, int height, int a, int b)
{
    s->width = width;
    s->height = height;
    s->pitch = a;
    s->pixels = (unsigned char*)b;
    s->x = 0;
    s->y = 0;
    s->zPriority = 10000;
    s->colorKey = -1;
    s->flag0 = 1;
    s->flag1 = 0;
    s->clip = Rect(0, 0, width - 1, height - 1);
}

// Frees an object if its "owned" flag (bit 0 of +0x2c) is set.
// FUNCTION: 0x4c6ac0
void __stdcall FreeSurface(Surface* obj)
{
    if (obj != 0 && obj->flag0) {
        GameFreeThunk(obj);
    }
}

// The original's callers (DrawQuadRow, DrawFrameQuad) call this out of line.
#pragma auto_inline(off)
// FUNCTION: 0x4c6ae0
Rect* Surface::GetClipRect(Rect* out)
{
    *out = clip;
    return out;
}
#pragma auto_inline(on)

// Stores a 16-byte rectangle passed by value into the object at +0x1c.
// FUNCTION: 0x4c6b10
void Surface::SetClipRect(Rect r)
{
    clip = r;
}

// FUNCTION: 0x4c6b40
void __stdcall SetDisplayFieldE4(int param_1)
{
    Display* d = GetDisplay();
    d->field_e4 = param_1;
}

// FUNCTION: 0x4c6b60
int GetDisplayFieldE4()
{
    return GetDisplay()->field_e4;
}

// Blits a bitmap with the hand-written routine BlitSurface. If `dst` is
// null the screen is locked with LockScreen and used as the destination;
// if `bmp` is null the locked screen is used as the source instead. When one
// of the two pointers is null the blit is offset by the bitmap's half width
// and half height (the shorts at +0x18 and +0x1a).
// FUNCTION: 0x4c6b70
void __stdcall DrawSurface(Surface* dst, Surface* bmp, int x, int y)
{
    // Two flat top-level ifs, each ending in its own unlock and return.
    if (dst == 0) {
        if (bmp == 0)
            return;
        Surface screen;
        if (LockScreen(&screen) == 0)
            return;
        BlitSurface(&screen, bmp, x - bmp->x, y - bmp->y);
        UnlockScreenInline(0);
        return;
    }
    if (bmp == 0) {
        Surface screen;
        if (LockScreen(&screen) == 0)
            return;
        BlitSurface(dst, &screen, x, y);
        UnlockScreenInline(0);
        return;
    }
    BlitSurface(dst, bmp, x - bmp->x, y - bmp->y);
}

// Copies `src` to `dst` at (x, y) with a transparent colour through the blitter
// at 0x4cbcd5, or, when `dst` is null, onto the screen: lock it with
// LockScreen, blit, then unlock. The unlock is UnlockScreen inlined.
// FUNCTION: 0x4c6c50
void __stdcall DrawSurfaceKeyed(Surface* dst, Surface* src, int x, int y, int color)
{
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            BlitSurfaceKeyed(&screen, src, x - src->x, y - src->y, color);
            UnlockScreenInline(&screen);
        }
    } else {
        BlitSurfaceKeyed(dst, src, x - src->x, y - src->y, color);
    }
}

// The second part of the module (0x4c6d20 and up). Its includes come after the
// first part's code, not at the top, so the first part's functions keep their
// symbol ids.
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
// Only for its symbol ids: DrawTexturedSpan and DrawLitTexturedSpan match only
// in a window of the symbol count (docs/c2-regalloc.md, "Symbol ids").
#include <malloc.h>
#include "../util/hapi_bank.h"

struct Point {
    int x;
    int y;
};

void __cdecl BlitRect(Surface* dst, Surface* src, Rect* rect, Point* pos);
void __cdecl BlitRectKeyed(Surface* dst, Surface* src, Rect* rect, Point* pos, unsigned char transparent);
void __cdecl BlitTile32x32(Surface* dst, Surface* src, Rect* rect, Point* pos);

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
    file->SeekBox(0);
    int header[2];
    header[0] = surface->width;
    header[1] = surface->height;
    file->WriteBox(header, 8);
    for (int i = 0; i < header[1]; i++) {
        file->WriteBox(surface->pixels + i * surface->pitch, header[0]);
    }
}

void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(void* ptr);

// Allocation and header set-up (the body of 0x4c6a60), kept inline.
static inline Surface* NewSurface(char* name, int w, int h)
{
    Surface* s = (Surface*)GameAllocIgnoreTag(name, h * w + 0x30);
    s->width = w;
    s->pitch = w;
    s->height = h;
    s->x = 0;
    s->pixels = (unsigned char*)(s + 1);
    s->flag0 = 1;
    s->flag1 = 0;
    s->y = 0;
    s->zPriority = 10000;
    s->colorKey = -1;
    s->clip = Rect(0, 0, w - 1, h - 1);
    return s;
}

// Reads back a surface written by 0x4c6f10: an 8-byte header of width and
// height, then one row per scan line into a freshly allocated surface.
// Must stay: <ddraw.h> decides the operand order of the row-loop multiply.
// FUNCTION: 0x4c6f80
Surface* __stdcall LoadSurface(HapiBank* file)
{
    file->SeekBox(0);
    int header[2];
    if (file->ReadBox(header, 8) < 8u) {
        return 0;
    }
    Surface* s = NewSurface("Loaded Surface", header[0], header[1]);
    for (int i = 0; i < header[1]; i++) {
        if (file->ReadBox(s->pixels + i * s->pitch, header[0]) < header[0]) {
            GameFreeThunk(s);
            return 0;
        }
    }
    return s;
}

// GLOBAL: 0x51fe48
extern int g_edgeListPrev[];
// GLOBAL: 0x51fea0
extern int g_edgeListNext[];

// FUNCTION: 0x4c7080
void __stdcall InitSurfaceIndexFreeList(int count)
{
    for (int i = 0; i < count; i++) {
        if (i == 0) {
            g_edgeListPrev[0] = count - 1;
        } else {
            g_edgeListPrev[i] = i - 1;
        }
        if (i == count - 1) {
            g_edgeListNext[i] = 0;
        } else {
            g_edgeListNext[i] = i + 1;
        }
    }
}

// GLOBAL: 0x51fef0
extern int g_edgeSpan;

struct Chunk {
    int field_0;
    int field_4;
};

struct Range {
    int low;
    int high;
};

// GLOBAL: 0x51fef8
extern Chunk* g_edgeTable;
// GLOBAL: 0x51fefc
extern int g_edgeHighIndex;
// GLOBAL: 0x51ff00
extern int g_edgeLowIndex;

int __cdecl FUN_004b7381(int a, int b, int c);

// FUNCTION: 0x4c70d0
void __stdcall InterpEdgeUvFromScanline(int value, Range* out, int at_low, int at_high)
{
    int i = g_edgeLowIndex;
    Chunk* table = g_edgeTable;
    int j = g_edgeListPrev[i];
    g_edgeHighIndex = j;
    while (value > table[j].field_4) {
        j = g_edgeListPrev[j];
        i = g_edgeListPrev[i];
        g_edgeHighIndex = j;
        g_edgeLowIndex = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    g_edgeSpan = size;
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
        out->high = FUN_004b7381(at_high, *spanp, g_edgeSpan);
        return; }
    case 1:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = 0;
        return;
    case 2:
        out->low = at_low;
        out->high = FUN_004b7381(at_high, offset, g_edgeSpan);
        return;
    case 3:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = at_high;
        return;
    }
}

// GLOBAL: 0x51fe98
extern int g_edgeLowIndexB;
// GLOBAL: 0x51fef4
extern int g_edgeHighIndexB;
// GLOBAL: 0x51fe40
extern int g_edgeSpanB;

// FUNCTION: 0x4c71f0
void __stdcall InterpEdgeUvFromScanline_B(int value, Range* out, int at_low, int at_high)
{
    int i = g_edgeLowIndexB;
    Chunk* table = g_edgeTable;
    int j = g_edgeListNext[i];
    g_edgeHighIndexB = j;
    while (value > table[j].field_4) {
        j = g_edgeListNext[j];
        i = g_edgeListNext[i];
        g_edgeHighIndexB = j;
        g_edgeLowIndexB = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    g_edgeSpanB = size;
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
            out->high = FUN_004b7381(at_high, *spanp, g_edgeSpanB);
        }
        return;
    case 2:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = at_high;
        return;
    case 3:
        out->low = 0;
        out->high = FUN_004b7381(at_high, offset, g_edgeSpanB);
        return;
    }
}

// Scales a (possibly skewed) source rect onto a destination surface one column
// at a time. It asks the surface for its clip rect, moves the left edge of the
// destination rect forward to it (advancing the two fixed-point source steps by
// the same amount, so the source follows the clip), clips the right edge, and
// then blits: 8 bits per pixel is an inline loop, 0x10/0x20/0x40/0x80 go to
// helpers, and anything else is an inline loop with a per-row byte stride.
// Unused here: real functions declared to keep the file's symbol count.
void __stdcall AccumulateScreenShake(int dx, int dy, int value);
void __stdcall ActivatePlayerGadgets(char* prefix);
void AddDownloadBuildOptions();

void __cdecl BlitSpan128(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl BlitSpan64(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl BlitSpan32(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl BlitSpan16(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);

// FUNCTION: 0x4c7310
void __stdcall DrawQuadRow(int param_1, int* rect, Surface* surf, GafFrame* info)
{
    unsigned char* dest = (unsigned char*)surf->pixels;
    unsigned char* src = info->pixelsOrLayers;
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
        dest += surf->pitch * param_1 + rect[0];
        switch (info->width) {
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
                *dest++ = src[(y >> 16) + (x >> 16) * info->width];
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

// Unused here: real functions declared to keep the file's symbol count.
int __stdcall AssignPlayerColor(int from, int to, int group);
void __stdcall AddAnimplayPointer(void* item);
int AllocFeatureSpot();

void __stdcall DrawQuadRow(int y, int* rect, void* surf, void* info);

// FUNCTION: 0x4c7580
void __stdcall DrawFrameQuad(void* surf, GafFrame* bmp,
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
        tmp.p[1].x = bmp->width - 1;
        tmp.p[1].y = 0;
        tmp.p[2].x = bmp->width - 1;
        tmp.p[2].y = bmp->height - 1;
        tmp.p[3].x = 0;
        tmp.p[3].y = bmp->height - 1;
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
        if (locked) UnlockScreen(&local);
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
// Unused here: real functions declared to keep the file's symbol count.
int __stdcall BroadcastPacket(int id, unsigned char* packet, int size);
void __stdcall AddCdActivitySample(int param_1);
int AllocScoreTables();

// Unused here: real functions declared to keep the file's symbol count.
void __stdcall BuildEntryGuiName(char* dest, unsigned short index, int n);
int __stdcall AddNetPlayer(int param_1);
void ApplyBrightnessAndVolume();

// FUNCTION: 0x4c7a20
void __stdcall DrawTexturedSpan(int row, int* span, GafFrame* surf, GafFrame* info)
{
    unsigned char* mask = surf->scratch;
    unsigned char* dest = surf->pixelsOrLayers;
    unsigned char* src = info->pixelsOrLayers;
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
    if (span[1] > surf->width - 1)
        span[1] = surf->width - 1;
    width = span[1] - span[0];
    if (width > 0) {
        int y = span[2];
        int x = span[3];
        int w = span[6];

        dest += surf->width * row + span[0];
        if (mask != 0) {
            mask += surf->width * row + span[0];
            switch (info->width) {
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
                        *dest = src[(y >> 16) + ((x >> 16) * info->width)];
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
        switch (info->width) {
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
                *dest++ = src[(y >> 16) + ((x >> 16) * info->width)];
                y += rowstep;
                x += colstep;
            } while (--n);
            return; }
        }
    }
}

// Only for its symbol ids: DrawLitTexturedSpan matches only in a window of
// the symbol count (docs/c2-regalloc.md, "Symbol ids").
#include <float.h>

// Blits one textured span with a depth (Z) buffer. For each pixel it compares
// the interpolated Z against the depth buffer and, when it passes, writes the
// palette-mapped texel and the new depth value. Source coordinates are 16.16
// fixed-point; the u/v/z/light steps are the span deltas divided by the span
// width. The span is clipped to the left edge and to the target surface's
// width. The texture width selects the texel layout: 8/16/32/64/128 pick the
// sub-texel mask, anything else uses the width itself as a row stride. With no
// depth buffer the four power-of-two formats dispatch to the FUN_004cd8xx span
// helpers and the remaining formats run inline loops.
// FUNCTION: 0x4c8020
void __stdcall DrawLitTexturedSpan(int row, int* span, GafFrame* target, GafFrame* texture)
{
    unsigned char* dest = target->pixelsOrLayers;
    unsigned char* depth = target->scratch;
    unsigned char* src = texture->pixelsOrLayers;
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

void __stdcall DrawTexturedSpan(int, int*, GafFrame*, GafFrame*);

// FUNCTION: 0x4c8760
void __stdcall DrawTexturedPolygon(GafFrame* target, GafFrame* texture, int* vertices, int* coords)
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

void __stdcall DrawLitTexturedSpan(int, int*, GafFrame*, GafFrame*);

// FUNCTION: 0x4c8bb0
void __stdcall DrawLitTexturedPolygon(GafFrame* target, GafFrame* texture, int* vertices, int* coords)
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
// share the same shape). data/symbols.csv names this address and its class
// Class_004c9390::ReleaseRef, and every caller (map_list.cpp, 0x432c00.cpp,
// 0x488a00.cpp, 0x4b75d0.cpp) calls it as a plain method of that class.

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
