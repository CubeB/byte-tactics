// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Blits two clip rectangles from the offscreen GDI surface to the window, or,
// in DirectDraw mode, unlocks the primary surface. The first argument is not
// used by the body.
#include <windows.h>
#include <ddraw.h>

struct Screen_004c60d0 {
    IDirectDrawSurface* surface;       // +0x0

    void Unlock() { surface->Unlock(0); }
};

struct Display_004c60d0 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    HDC offscreenDC;                   // +0x44
    HDC sourceDC;                      // +0x48
    HPALETTE hpalette;                 // +0x4c
    char unknown_50[0x88 - 0x50];
    Screen_004c60d0 screen;            // +0x88
};

Display_004c60d0* FUN_004b6220(void);

// FUNCTION: 0x4c60d0
int __stdcall FUN_004c60d0(int unused, RECT* r1, RECT* r2)
{
    Display_004c60d0* d = FUN_004b6220();
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
    if (d->screen.surface == 0) {
        return 0;
    }
    d->screen.Unlock();
    return 1;
}
