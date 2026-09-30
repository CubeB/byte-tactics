// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// Partial: 99.3%. Byte count is exact (1017 = 1017) and every instruction is the
// right one; only two adjacent pairs are swapped, both 2-instruction scheduler
// tie-breaks in the display-cleanup block (listed at the bottom of this file).
// The original holds the constant 0 in a callee-saved register (ebp) and tests
// every HRESULT against it with `cmp eax, ebp`, so all ten DirectDraw call
// results go through one named `HRESULT hr` local (see docs/agent-guide.md on
// 0x4b6880). That one change took this function from 87.7% to 99.3% and fixed
// the size, the missing `xor eax,eax` and the CreateSurface argument timing too.
#include <windows.h>
#include <ddraw.h>

struct Class_004c6a60;

struct Surface_004b5510 {
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
extern Display_004b5510 *DAT_0051fbd0;

int __stdcall FUN_0049f710(int guid, void *display, int zero);
void __stdcall FUN_004b4ff0(Display_004b5510 *d);
void __stdcall FUN_004c6a60(Class_004c6a60 *s, int width, int height, int a, int b);
int __stdcall FUN_004c5e70(Surface_004b5510 *s);
void __cdecl FUN_004cbbe0(Surface_004b5510 *dst, void *src, int x, int y);
int __stdcall FUN_004c5fa0(Surface_004b5510 *s);
int __stdcall FUN_004ba200(PALETTEENTRY *entries, int start, int count);

// FUNCTION: 0x4b5510
int __stdcall FUN_004b5510(int mode) {
    struct {
        int lockResult;
        HDC *dcSlot;
    } setup;
    Display_004b5510 *d;
    DDSURFACEDESC ddsd;
    BitmapInfo_004b5510 bmi;
    Surface_004b5510 surf;
    DDSCAPS caps;
    HRESULT hr;

    while (1) {
        int result = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (result == 0) {
            DAT_0052a4ec = 0x4d41494e;
            setup.lockResult = 0;
            break;
        }
        if (DAT_0052a4ec == 0x4d41494e) {
            setup.lockResult = result;
            break;
        }
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
    d = DAT_0051fbd0;
    DirectDrawState *dd = &d->draw;
    DAT_0051fbd0->draw.field_9c = 0;
    FUN_004b4ff0(DAT_0051fbd0);

    Display_004b5510 *cleanup = DAT_0051fbd0;
    setup.dcSlot = &cleanup->dc;
    if (*setup.dcSlot)
        DeleteDC(*setup.dcSlot);
    if (cleanup->hpalette)
        DeleteObject(cleanup->hpalette);
    if (cleanup->dib)
        DeleteObject(cleanup->dib);
    cleanup->hpalette = 0;
    cleanup->dib = 0;
    *setup.dcSlot = 0;
    SetWindowPos(d->hwnd, NULL, 0, 0, DAT_0051fbd0->width, DAT_0051fbd0->height,
                 SWP_NOZORDER | SWP_NOMOVE);

    if (mode != 0) {
        DAT_0051fbd0->field_f0 |= 2;

        hr = FUN_0049f710(0, &dd->ddraw, 0);
        if (hr == DD_OK) {
            hr = dd->ddraw->SetCooperativeLevel(d->hwnd, 0x53);
            if (hr == DD_OK) {
                hr = dd->ddraw->SetDisplayMode(DAT_0051fbd0->width, DAT_0051fbd0->height, 8);
                if (hr == DD_OK) {

                    ZeroMemory(&ddsd, sizeof(ddsd));
                    ddsd.dwSize = sizeof(ddsd);
                    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
                    ddsd.dwWidth = DAT_0051fbd0->width;
                    ddsd.dwHeight = DAT_0051fbd0->height;
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

                                        hr = dd->ddraw->CreatePalette(4, DAT_0051fbd0->entries,
                                                                      &dd->palette, NULL);
                                        if (hr == DD_OK) {
                                            hr = dd->primary->SetPalette(dd->palette);
                                            if (hr != DD_OK)
                                                goto fail;
                                        }

                                        if (DAT_0051fbd0->draw.field_98) {
                                            FUN_004c5e70(&surf);
                                            FUN_004cbbe0(&surf, DAT_0051fbd0->draw.field_98, 0, 0);
                                            FUN_004c5fa0(&surf);
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
        DAT_0051fbd0->field_f0 &= ~2;

        HDC hdc = GetDC(d->hwnd);
        d->dc = CreateCompatibleDC(hdc);
        ReleaseDC(d->hwnd, hdc);

        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = DAT_0051fbd0->width;
        bmi.bmiHeader.biHeight = -DAT_0051fbd0->height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biCompression = 0;
        bmi.bmiHeader.biBitCount = 8;
        bmi.bmiHeader.biSizeImage = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;
        ZeroMemory(bmi.bmiColors, sizeof(bmi.bmiColors));
        d->dib = CreateDIBSection(d->dc, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, (void **)&setup.dcSlot,
                                  NULL, 0);
        FUN_004c6a60((Class_004c6a60 *)&d->unknown_50[0], DAT_0051fbd0->width, DAT_0051fbd0->height,
                     (DAT_0051fbd0->width + 3) & ~3, (int)setup.dcSlot);
        SelectObject(d->dc, d->dib);
        SetWindowPos(d->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    FUN_004ba200(DAT_0051fbd0->entries, 0, 0x100);
    if (setup.lockResult == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 1;

fail:
    if (setup.lockResult == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 0;
}

// Still differing, both two-instruction swaps of a pair that MSVC 5 5.x emits in
// the other order, and both inside the cleanup block just after FUN_004b4ff0:
//   0x4b556d..0x4b5573  original: lea ebx,[esi+0x84] / push ecx
//                       ours:     push ecx / lea ebx,[esi+0x84]
//   0x4b55af..0x4b55b3  original: mov edx,[esp+0x10...+0x14] first, then the two
//                       hpalette/dib zero stores; ours: the two stores, then the
//                       reload of &cleanup->dc.
// Tried and rejected: `DirectDrawState *dd` declared before or after the
// field_9c store (no change), after the FUN_004b4ff0 call (worse), and declaring
// `cleanup` before the call (MSVC then copies esi to edi, 93.1%).
// Field_98 is the display's saved back buffer: FUN_004c5e70/FUN_004cbbe0/
// FUN_004c5fa0 are called on a fresh 0x30-byte surface only when it is set.
