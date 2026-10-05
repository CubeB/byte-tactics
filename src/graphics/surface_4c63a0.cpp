// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by GPT-6, finished by claude-opus-5-5. Names are provisional.
// Presents a frame. With flag bit 1 clear it blits the cached bitmap through
// GDI. Otherwise, when +0xdc is set, it copies the bitmap at +0xbc (if its
// size matches GetScreenWidth/GetScreenHeight) into the locked primary surface;
// else it flips, or Blts the back surface to the window's client rect,
// retrying after DDERR_SURFACELOST.
// The two "MAIN" Lock()/Unlock() pairs share one Unlock body in the exe.
// That merge needs the bitmap path's Lock to keep InterlockedExchange in ebp
// and the lock result in ebx, as the first path does. Earlier attempts (up to
// 90.7%) forced it with a dead `if ((held = Lock()) == 0) held = 0;` test; the
// real cause was the surface-restore chain, which the original wrote once as
// an inline helper (RestoreSurfaces below) and used in both places. With the
// helper the function matches with a plain `LONG held = Lock();`.
// Frame: held, bmp and pt share 0x10, rect 0x18, src 0x28, out 0x38,
// screen 0x68, desc 0x98.
#include <windows.h>
#include <ddraw.h>

struct Surface {
    int data[12];
};

struct Out_004c63a0 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int pad[8];
};

#pragma pack(push, 1)
struct Screen_004c63a0 {
    char unknown_0[0x8];
    IDirectDrawSurface* primary;       // +0x08
    IDirectDrawSurface* surface;       // +0x0c
    char unknown_10[0x18 - 0x10];

    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c63a0 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    int field_44;                      // +0x44
    HDC srcDC;                         // +0x48
    HPALETTE palette;                  // +0x4c
    Out_004c63a0 cached;               // +0x50
    Screen_004c63a0 screen;            // +0x80
    Surface* field_98;                 // +0x98
    int field_9c;                      // +0x9c
    char unknown_a0[0xbc - 0xa0];
    Surface* field_bc;                 // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    int field_d4;                      // +0xd4
    int field_d8;                      // +0xd8
    int field_dc;                      // +0xdc
    char unknown_e0[0xf0 - 0xe0];
    unsigned short flags;              // +0xf0
    char unknown_f2[0x196 - 0xf2];
    int field_196;                     // +0x196
    char unknown_19a[0x1b2 - 0x19a];
    Surface* field_1b2;                // +0x1b2
    int field_1b6;                     // +0x1b6
    int field_1ba;                     // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int field_1ce;                     // +0x1ce
    int field_1d2;                     // +0x1d2
};
#pragma pack(pop)

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;
extern int g_screenLockCount;

Display_004c63a0* GetDisplay(void);
int GetScreenWidth(void);
int GetScreenHeight(void);
int __stdcall LockScreen(Surface* out);
void __stdcall DrawSurface(Surface* dst, Surface* bmp, int x, int y);
void __stdcall DrawCursor(Display_004c63a0* obj, void* dst);
void __cdecl BlitSurface(Surface* dst, Surface* src, int x, int y);

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

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline()
{
    Display_004c63a0* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// Restores both surfaces after DDERR_SURFACELOST and redraws the screen. An
// inline helper in the original: its two uses here (after the failed primary
// Lock and in the Blt retry loop) are what let MSVC give the bitmap-path
// Lock the same registers as the first path and tail-merge the two Unlocks.
static inline HRESULT RestoreSurfaces(Display_004c63a0* d)
{
    Display_004c63a0* dd = GetDisplay();
    if (dd->field_44 != 0)
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

// FUNCTION: 0x4c63a0
void FlipScreen(void)
{
    Display_004c63a0* d = GetDisplay();
    unsigned short flags = d->flags;

    if ((flags & 2) == 0) {
        LONG held = Lock();
        Out_004c63a0* p = &d->cached;
        DrawSurface((Surface*)p, d->field_bc, 0, 0);
        DrawCursor(d, p);
        HDC hdc = GetDC(d->hwnd);
        SelectPalette(hdc, d->palette, 0);
        RealizePalette(hdc);
        BitBlt(hdc, 0, 0, p->field_0, d->cached.field_4, d->srcDC, 0, 0, SRCCOPY);
        ReleaseDC(d->hwnd, hdc);
        Unlock(held);
        return;
    }

    if (d->field_dc != 0) {
        Desc desc;
        Surface out;
        Surface* bmp = d->field_bc;
        if (bmp->data[0] != GetScreenWidth())
            return;
        if (bmp->data[1] != GetScreenHeight())
            return;

        LONG held = Lock();
        desc.dwSize = sizeof(desc);
        unsigned long lr = d->screen.primary->Lock(0, (DDSURFACEDESC*)&desc, 1, 0);
        if (lr == 0) {
            out.data[0] = d->field_d4;
            out.data[1] = d->field_d8;
            out.data[2] = desc.lPitch;
            out.data[3] = (int)desc.lpSurface;
            DrawCursor(d, bmp);
            BlitSurface(&out, bmp, 0, 0);
            if (d->field_1ce != 0 && d->field_1d2 != 0)
                DrawSurface(bmp, (Surface*)d->field_1be, d->field_1b6, d->field_1ba);
            d->screen.primary->Unlock(0);
        } else if (lr == 0x887601c2) {
            RestoreSurfaces(d);
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
        hr = RestoreSurfaces(d);
        if (hr != 0)
            continue;
        return;
    }
    }
}

