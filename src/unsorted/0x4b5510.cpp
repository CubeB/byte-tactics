// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Switches the display between the DirectDraw fullscreen path (mode != 0) and
// the GDI/DIB windowed path (mode == 0), rebuilding the surfaces, clipper and
// palette. Returns 1 on success, 0 after releasing the 'MAIN' display lock.
//
// PARTIAL (33.3%). Structure and most of the body are right; what still
// differs:
//   - frame is 0x4cc, original 0x4d0 (one missing 4-byte local; the original
//     caches &display->dc in a slot and reloads it, MSVC here folds it away).
//   - original keeps 0 in ebp for the whole function (xor ebp,ebp at entry,
//     then cmp eax,ebp / push ebp / mov [field],ebp); here ebp holds `locked`
//     and the zero uses are immediates (test eax,eax, push 0), which shifts
//     most of the instruction stream. `locked` needs to land in a stack slot
//     to free ebp.
//   - ETAs at 0x4b554b/0x4b5557 differ in which break point spills `locked`.
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

struct Display_004b5510 {
    char unknown_0[0x40];
    HWND hwnd;                        // +0x40
    HBITMAP dib;                      // +0x44
    HDC dc;                           // +0x48
    HPALETTE hpalette;                // +0x4c
    char unknown_50[0x84 - 0x50];
    IDirectDraw* ddraw;               // +0x84
    IDirectDrawSurface* primary;      // +0x88
    IDirectDrawSurface* back;         // +0x8c
    IDirectDrawClipper* clipper;      // +0x90
    IDirectDrawPalette* palette;      // +0x94
    void* field_98;                   // +0x98
    int field_9c;                     // +0x9c
    char unknown_a0[0xd4 - 0xa0];
    int width;                        // +0xd4
    int height;                       // +0xd8
    char unknown_dc[0xf0 - 0xdc];
    unsigned short field_f0;          // +0xf0
    char unknown_f2[0x214 - 0xf2];
    PALETTEENTRY entries[256];        // +0x214
};

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;
extern Display_004b5510* DAT_0051fbd0;

int __stdcall FUN_0049f710(int guid, void* display, int zero);
void __stdcall FUN_004b4ff0(Display_004b5510* d);
void __stdcall FUN_004c6a60(Class_004c6a60* s, int width, int height, int a, int b);
int __stdcall FUN_004c5e70(Surface_004b5510* s);
void __cdecl FUN_004cbbe0(Surface_004b5510* dst, void* src, int x, int y);
int __stdcall FUN_004c5fa0(Surface_004b5510* s);
int __stdcall FUN_004ba200(PALETTEENTRY* entries, int start, int count);

// FUNCTION: 0x4b5510
int __stdcall FUN_004b5510(int mode)
{
    int locked;
    Display_004b5510* d;
    DDSURFACEDESC ddsd;
    BitmapInfo_004b5510 bmi;
    Surface_004b5510 surf;

    while (1) {
        locked = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (locked == 0) {
            DAT_0052a4ec = 0x4d41494e;
            break;
        }
        if (DAT_0052a4ec == 0x4d41494e)
            break;
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }

    d = DAT_0051fbd0;
    d->field_9c = 0;
    FUN_004b4ff0(d);

    d = DAT_0051fbd0;
    HDC* pdc = &d->dc;
    if (*pdc)
        DeleteDC(*pdc);
    if (d->hpalette)
        DeleteObject(d->hpalette);
    if (d->dib)
        DeleteObject(d->dib);
    d->hpalette = 0;
    d->dib = 0;
    *pdc = 0;
    SetWindowPos(d->hwnd, NULL, 0, 0, DAT_0051fbd0->width,
                 DAT_0051fbd0->height, SWP_NOZORDER | SWP_NOMOVE);

    if (mode != 0) {
        DAT_0051fbd0->field_f0 |= 2;

        if (FUN_0049f710(0, &d->ddraw, 0) != 0)
            goto fail;
        if (d->ddraw->SetCooperativeLevel(d->hwnd, 0x53) != DD_OK)
            goto fail;
        if (d->ddraw->SetDisplayMode(d->width, d->height, 8) != DD_OK)
            goto fail;

        ZeroMemory(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.dwWidth = d->width;
        ddsd.dwHeight = d->height;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
        ddsd.dwBackBufferCount = 1;
        if (d->ddraw->CreateSurface(&ddsd, &d->primary, NULL) != DD_OK)
            goto fail;

        DDSCAPS caps;
        caps.dwCaps = DDSCAPS_BACKBUFFER;
        if (d->primary->GetAttachedSurface(&caps, &d->back) != DD_OK)
            goto fail;

        if (d->ddraw->CreateClipper(0, &d->clipper, NULL) != DD_OK)
            goto fail;
        d->field_9c = 1;
        if (d->clipper->SetHWnd(0, d->hwnd) != DD_OK)
            goto fail;
        if (d->primary->SetClipper(d->clipper) != DD_OK)
            goto fail;

        if (d->ddraw->CreatePalette(4, d->entries, &d->palette, NULL) == DD_OK) {
            if (d->primary->SetPalette(d->palette) != DD_OK)
                goto fail;
        }

        if (d->field_98) {
            FUN_004c5e70(&surf);
            FUN_004cbbe0(&surf, d->field_98, 0, 0);
            FUN_004c5fa0(&surf);
        }
    } else {
        DAT_0051fbd0->field_f0 &= ~2;

        HDC hdc = GetDC(d->hwnd);
        d->dc = CreateCompatibleDC(hdc);
        ReleaseDC(d->hwnd, hdc);

        void* bits;
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = d->width;
        bmi.bmiHeader.biHeight = -d->height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biCompression = 0;
        bmi.bmiHeader.biBitCount = 8;
        bmi.bmiHeader.biSizeImage = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;
        ZeroMemory(bmi.bmiColors, sizeof(bmi.bmiColors));
        d->dib = CreateDIBSection(d->dc, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, &bits, NULL, 0);
        FUN_004c6a60((Class_004c6a60*)&d->unknown_50[0], d->width, d->height,
                     (d->width + 3) & ~3, (int)bits);
        SelectObject(d->dc, d->dib);
        SetWindowPos(d->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    FUN_004ba200(d->entries, 0, 0x100);
    if (locked == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 1;

fail:
    if (locked == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 0;
}
