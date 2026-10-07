// Decompiled by Opus, Haiku, GPT-5.6-Terra, Sonnet 5.5, deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, Space Bunny Free, claude-sonnet-5-5, GPT-6, claude-opus-5-5, space-bunny-free and Sonnet. Names are provisional.
// The display object's DirectDraw surfaces and the screen lock stack: locking
// and unlocking the primary surface and the screen, presenting a frame,
// filling and allocating surfaces, and the surface clip rectangle.
#include <windows.h>
#include <string.h>
#include <ddraw.h>

// A 16-byte rectangle: the surface's clip, the display's cached vector.
struct Rect {
    int left;
    int top;
    int right;
    int bottom;
    Rect() {}
    Rect(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
};

// A surface header, 0x30 bytes. An allocated image's pixels follow it at
// +0x30, and its pointer is the field at +0xc.
struct Surface {
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
    void SetClipRect(Rect r);
};

// The cursor bitmap: two sizes and the offset from the cursor's position.
#pragma pack(push, 1)
struct Bitmap_004c67c0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
};
#pragma pack(pop)

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
    Surface* field_98;                 // +0x98
    int field_9c;                      // +0x9c
    Rect vec;                          // +0xa0
    char unknown_b0[0xb4 - 0xb0];
    IDirectDrawSurface* field_b4;      // +0xb4
    char unknown_b8[0xbc - 0xb8];
    Surface* field_bc;                 // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    int field_d4;                      // +0xd4
    int field_d8;                      // +0xd8
    int field_dc;                      // +0xdc
    char unknown_e0[0xe4 - 0xe0];
    int field_e4;                      // +0xe4
    char unknown_e8[0xf0 - 0xe8];
    Flags_004c61b0 flags;              // +0xf0
    char unknown_f2[0x196 - 0xf2];
    int rect_x;                        // +0x196
    int rect_y;                        // +0x19a
    char unknown_19e[0x1b2 - 0x19e];
    Bitmap_004c67c0* bmp;              // +0x1b2
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int field_1ce;                     // +0x1ce
    int field_1d2;                     // +0x1d2

    // Method of the display object: this is the call result.
    int LockScreen(Surface* out)
    {
        if (field_dc != 0) {
            *out = *field_bc;
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
        out->width = field_d4;
        out->height = field_d8;
        out->field_8 = desc.lPitch;
        out->pixels = (char*)desc.lpSurface;
        out->field_18 = 0;
        out->field_1a = 0;
        out->field_10 = 10000;
        out->field_14 = -1;
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
        out->width = field_d4;
        out->height = field_d8;
        out->field_8 = desc.lPitch;
        out->pixels = (char*)desc.lpSurface;
        out->field_18 = 0;
        out->field_1a = 0;
        out->field_10 = 10000;
        out->field_14 = -1;
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
void __stdcall DrawFrame(Surface* dst, Bitmap_004c67c0* bmp, int x, int y);
void __stdcall DrawCursor(Display* obj, Surface* dst);
int GetScreenWidth(void);
int GetScreenHeight(void);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

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
    if (d->offscreenDC == 0 && d->field_dc == 0) {
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
// FUNCTION: 0x4c5fa0
int __stdcall UnlockScreen(Surface* s)
{
    Display* d = GetDisplay();
    if (d->offscreenDC == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

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
    if (enable && obj->field_9c == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: 0x4c61f0
void __stdcall SetRestoreSurface(int param_1)
{
    Display* obj = GetDisplay();
    obj->field_98 = (Surface*)param_1;
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
            BlitSurface(&screen, d->field_98, r, r);

            Display* d2 = GetDisplay();
            if (d2->offscreenDC == 0 && d2->field_dc == 0 && d2->screen.surface != 0) {
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
    if (d->offscreenDC == 0 && d->field_dc == 0) {
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
                        BlitSurface(&screen, d2->field_98, 0, 0);
                        UnlockScreenInline();
                    }
                }
            }
        }
        d->field_b4 = d->screen.surface;
    }
    d->field_dc = 0;
}

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (r == 0) {
            DAT_0052a4ec = 0x4d41494e;
            return 0;
        }
        if (DAT_0052a4ec == 0x4d41494e)
            return r;
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
}

static inline void Unlock(LONG held)
{
    if (held == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
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
            BlitSurface(&screen, dd->field_98, 0, 0);
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
        DrawSurface(p, d->field_bc, 0, 0);
        DrawCursor(d, p);
        HDC hdc = GetDC(d->hwnd);
        SelectPalette(hdc, d->hpalette, 0);
        RealizePalette(hdc);
        BitBlt(hdc, 0, 0, p->width, d->cached.height, d->sourceDC, 0, 0, SRCCOPY);
        ReleaseDC(d->hwnd, hdc);
        Unlock(held);
        return;
    }

    if (d->field_dc != 0) {
        Desc desc;
        Surface out;
        Surface* bmp = d->field_bc;
        if (bmp->width != GetScreenWidth())
            return;
        if (bmp->height != GetScreenHeight())
            return;

        LONG held = Lock();
        desc.dwSize = sizeof(desc);
        unsigned long lr = d->screen.primary->Lock(0, (DDSURFACEDESC*)&desc, 1, 0);
        if (lr == 0) {
            out.width = d->field_d4;
            out.height = d->field_d8;
            out.field_8 = desc.lPitch;
            out.pixels = (char*)desc.lpSurface;
            DrawCursor(d, bmp);
            BlitSurface(&out, bmp, 0, 0);
            if (d->field_1ce != 0 && d->field_1d2 != 0)
                DrawSurface(bmp, (Surface*)d->field_1be, d->x, d->y);
            d->screen.primary->Unlock(0);
        } else if (lr == 0x887601c2) {
            RestoreSurfacesInline(d);
        }
        Unlock(held);
        return;
    }

    if (d->field_9c != 0 && (flags & 1) != 0) {
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
    if (obj->field_1ce != 0 && obj->field_1d2 != 0 && obj->bmp != 0) {
        obj->field_1be[0] = obj->bmp->width;
        obj->field_1be[1] = obj->bmp->height;
        obj->field_1be[2] = obj->bmp->width;
        obj->x = obj->rect_x - obj->bmp->dx;
        obj->y = obj->rect_y - obj->bmp->dy;
        DrawSurface((Surface*)obj->field_1be, dst, -obj->x, -obj->y);
        DrawFrame(dst, obj->bmp, obj->rect_x, obj->rect_y);
    }
}

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
static void set_mem(char* p, int count, int colour)
{
    memset(p, colour, count);
}

static void clear_surface(Surface* s, int colour)
{
    memset(s->pixels, colour, s->height * s->field_8);
}

static void clear_screen(Display* o, int colour)
{
    memset(o->cached.pixels, colour, o->cached.height * o->cached.field_8);
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
        if (obj->field_dc != 0) {
            Surface* s = obj->field_bc;
            set_mem(s->pixels, s->height * s->field_8, colour);
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
    d->field_dc = 1;
    d->field_bc = (Surface*)param_1;
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
    s->field_8 = a;
    s->pixels = (char*)b;
    s->field_18 = 0;
    s->field_1a = 0;
    s->field_10 = 10000;
    s->field_14 = -1;
    s->flag0 = 1;
    s->flag1 = 0;
    s->clip = Rect(0, 0, width - 1, height - 1);
}

// FUNCTION: 0x4c69f0
Surface* __stdcall AllocSurface(char* name, int width, int height)
{
    Surface* s = (Surface*)FUN_004d83b0(name, height * width + 0x30);
    Init(s, width, height, width, (int)(s + 1));
    return s;
}

// FUNCTION: 0x4c6a60
void __stdcall InitSurface(Surface* s, int width, int height, int a, int b)
{
    s->width = width;
    s->height = height;
    s->field_8 = a;
    s->pixels = (char*)b;
    s->field_18 = 0;
    s->field_1a = 0;
    s->field_10 = 10000;
    s->field_14 = -1;
    s->flag0 = 1;
    s->flag1 = 0;
    s->clip = Rect(0, 0, width - 1, height - 1);
}

// Frees an object if its "owned" flag (bit 0 of +0x2c) is set.
// FUNCTION: 0x4c6ac0
void __stdcall FreeSurface(Surface* obj)
{
    if (obj != 0 && obj->flag0) {
        FUN_004d85a0(obj);
    }
}

// FUNCTION: 0x4c6ae0
Rect* Surface::GetClipRect(Rect* out)
{
    *out = clip;
    return out;
}

// Stores a 16-byte rectangle passed by value into the object at +0x1c.
// FUNCTION: 0x4c6b10
void Surface::SetClipRect(Rect r)
{
    clip = r;
}

// FUNCTION: 0x4c6b40
void __stdcall FUN_004c6b40(int param_1)
{
    Display* d = GetDisplay();
    d->field_e4 = param_1;
}

// FUNCTION: 0x4c6b60
int FUN_004c6b60()
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
        BlitSurface(&screen, bmp, x - bmp->field_18, y - bmp->field_1a);
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
    BlitSurface(dst, bmp, x - bmp->field_18, y - bmp->field_1a);
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
            BlitSurfaceKeyed(&screen, src, x - src->field_18, y - src->field_1a, color);
            UnlockScreenInline(&screen);
        }
    } else {
        BlitSurfaceKeyed(dst, src, x - src->field_18, y - src->field_1a, color);
    }
}
