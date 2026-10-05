// Decompiled by Opus. Names are provisional.
// Re-applies the display palette: in GDI mode selects and realizes the
// HPALETTE on the window's DC, otherwise attaches the DirectDraw palette to
// the primary surface. Returns 1 on success.
#include <windows.h>
#include <ddraw.h>

struct Display_004b4f50 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    char unknown_44[0x4c - 0x44];
    HPALETTE hpalette;                 // +0x4c
    char unknown_50[0x88 - 0x50];
    IDirectDrawSurface* primary;       // +0x88
    char unknown_8c[0x94 - 0x8c];
    IDirectDrawPalette* palette;       // +0x94
};

extern Display_004b4f50* g_display;

// FUNCTION: 0x4b4f50
int ApplyPalette()
{
    Display_004b4f50* d = g_display;
    if (d->hpalette) {
        HDC dc = GetDC(g_display->hwnd);
        SelectPalette(dc, g_display->hpalette, FALSE);
        RealizePalette(dc);
        ReleaseDC(g_display->hwnd, dc);
        return 1;
    }
    HRESULT hr = E_FAIL;
    if (d->primary && d->palette)
        hr = d->primary->SetPalette(d->palette);
    return hr == DD_OK ? 1 : 0;
}
