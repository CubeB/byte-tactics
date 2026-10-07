// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by Claude Opus 5.5. Names are provisional.
//
// Switches the display between DirectDraw full screen (mode != 0: cooperative
// level, display mode, flipping primary with one back buffer, clipper, palette,
// then redraws the saved picture) and a windowed GDI DIB section (mode == 0),
// under the display lock at DAT_0052a4e8, and finally reloads the palette.


#include <windows.h>
#include <ddraw.h>

struct Class_004c6a60;

struct Surface {
    int data[12];
};

struct BitmapInfo_004b5510 {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[256];
};

struct DirectDrawState {
    IDirectDraw *ddraw;          // +0x84
    IDirectDrawSurface *primary; // +0x88
    IDirectDrawSurface *back;    // +0x8c
    IDirectDrawClipper *clipper; // +0x90
    IDirectDrawPalette *palette; // +0x94
    void *field_98;              // +0x98
    int field_9c;                // +0x9c
};

struct Display_004b5510 {
    char unknown_0[0x40];
    HWND hwnd;         // +0x40
    HBITMAP dib;       // +0x44
    HDC dc;            // +0x48
    HPALETTE hpalette; // +0x4c
    char unknown_50[0x84 - 0x50];
    DirectDrawState draw;
    char unknown_a0[0xd4 - 0xa0];
    int width;  // +0xd4
    int height; // +0xd8
    char unknown_dc[0xf0 - 0xdc];
    unsigned short field_f0; // +0xf0
    char unknown_f2[0x214 - 0xf2];
    PALETTEENTRY entries[256]; // +0x214
};

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;
extern Display_004b5510 *g_display;

int __stdcall DirectDrawCreateThunk(int guid, void *display, int zero);
void __stdcall ReleaseDirectDraw(Display_004b5510 *d);
void __stdcall InitSurface(Class_004c6a60 *s, int width, int height, int a, int b);
int __stdcall LockScreen(Surface *s);
void __cdecl BlitSurface(Surface *dst, void *src, int x, int y);
int __stdcall UnlockScreen(Surface *s);
int __stdcall SetPaletteColors(PALETTEENTRY *entries, int start, int count);

// Takes the DC by reference: the original reloads it at the block head.
static inline void FreeGdi_004b5510(Display_004b5510 *d, HDC &dc)
{
    if (dc)
        DeleteDC(dc);
    if (d->hpalette)
        DeleteObject(d->hpalette);
    if (d->dib)
        DeleteObject(d->dib);
    d->hpalette = 0;
    d->dib = 0;
    dc = 0;
}

// FUNCTION: 0x4b5510
int __stdcall SetFullScreen(int mode) {
    int lockResult;
    Display_004b5510 *d;
    DDSURFACEDESC ddsd;
    BitmapInfo_004b5510 bmi;
    Surface surf;
    DDSCAPS caps;
    HRESULT hr;

    while (1) {
        int result = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (result == 0) {
            DAT_0052a4ec = 0x4d41494e;
            lockResult = 0;
            break;
        }
        if (DAT_0052a4ec == 0x4d41494e) {
            lockResult = result;
            break;
        }
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
    d = g_display;
    DirectDrawState *dd = &d->draw;
    dd->field_9c = 0;
    ReleaseDirectDraw(g_display);

    FreeGdi_004b5510(g_display, g_display->dc);
    SetWindowPos(d->hwnd, NULL, 0, 0, g_display->width, g_display->height,
                 SWP_NOZORDER | SWP_NOMOVE);

    if (mode != 0) {
        g_display->field_f0 |= 2;

        hr = DirectDrawCreateThunk(0, &dd->ddraw, 0);
        if (hr == DD_OK) {
            hr = dd->ddraw->SetCooperativeLevel(d->hwnd, 0x53);
            if (hr == DD_OK) {
                hr = dd->ddraw->SetDisplayMode(g_display->width, g_display->height, 8);
                if (hr == DD_OK) {

                    ZeroMemory(&ddsd, sizeof(ddsd));
                    ddsd.dwSize = sizeof(ddsd);
                    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
                    ddsd.dwWidth = g_display->width;
                    ddsd.dwHeight = g_display->height;
                    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
                    ddsd.dwBackBufferCount = 1;
                    hr = dd->ddraw->CreateSurface(&ddsd, &dd->primary, NULL);
                    if (hr == DD_OK) {

                        caps.dwCaps = DDSCAPS_BACKBUFFER;
                        hr = dd->primary->GetAttachedSurface(&caps, &dd->back);
                        if (hr == DD_OK) {

                            dd->field_9c = 1;
                            hr = dd->ddraw->CreateClipper(0, &dd->clipper, NULL);
                            if (hr == DD_OK) {
                                hr = dd->clipper->SetHWnd(0, d->hwnd);
                                if (hr == DD_OK) {
                                    hr = dd->primary->SetClipper(dd->clipper);
                                    if (hr == DD_OK) {

                                        hr = dd->ddraw->CreatePalette(4, g_display->entries,
                                                                      &dd->palette, NULL);
                                        if (hr == DD_OK) {
                                            hr = dd->primary->SetPalette(dd->palette);
                                            if (hr != DD_OK)
                                                goto fail;
                                        }

                                        if (g_display->draw.field_98) {
                                            LockScreen(&surf);
                                            BlitSurface(&surf, g_display->draw.field_98, 0, 0);
                                            UnlockScreen(&surf);
                                        }
                                    } else
                                        goto fail;
                                } else
                                    goto fail;
                            } else
                                goto fail;
                        } else
                            goto fail;
                    } else
                        goto fail;
                } else
                    goto fail;
            } else
                goto fail;
        } else
            goto fail;
    } else {
        g_display->field_f0 &= ~2;

        void *bits;
        HDC hdc = GetDC(d->hwnd);
        d->dc = CreateCompatibleDC(hdc);
        ReleaseDC(d->hwnd, hdc);

        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = g_display->width;
        bmi.bmiHeader.biHeight = -g_display->height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biCompression = 0;
        bmi.bmiHeader.biBitCount = 8;
        bmi.bmiHeader.biSizeImage = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;
        ZeroMemory(bmi.bmiColors, sizeof(bmi.bmiColors));
        d->dib = CreateDIBSection(d->dc, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, &bits,
                                  NULL, 0);
        InitSurface((Class_004c6a60 *)&d->unknown_50[0], g_display->width, g_display->height,
                     (g_display->width + 3) & ~3, (int)bits);
        SelectObject(d->dc, d->dib);
        SetWindowPos(d->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    SetPaletteColors(g_display->entries, 0, 0x100);
    if (lockResult == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 1;

fail:
    if (lockResult == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 0;
}

