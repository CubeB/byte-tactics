// Decompiled by GPT-6-Luna. Names are provisional.
// GAVE UP: Best check was 50.8%. DirectDraw wrapper layout and COM vtable
// calls differ, and this attempt does not reproduce the final ordinal calls.
#include <windows.h>
#include <ddraw.h>

class Class_0047bf70 {
public:
    void *configuration;
    char unknown_4[8];
    HWND hwnd;
    PALETTEENTRY entries[256];
    int paletteResult;
    char unknown_414[0x544 - 0x414];
    IDirectDraw *ddraw;
    DDSURFACEDESC surfaceDesc;
    int FUN_0047c150(void *);
    int FUN_0047bf70();
};

extern int __stdcall FUN_0049f710(int, void *, int);

// FUNCTION: 0x47bf70
int Class_0047bf70::FUN_0047bf70()
{
    POINT point;
    HDC hdc;
    int result;
    int i;

    point.x = 0;
    point.y = 0;
    ClientToScreen(hwnd, &point);
    result = FUN_0049f710(0, ddraw, 0);
    if (result == 0) {
        result = ddraw->SetCooperativeLevel(hwnd, 8);
        if (result != 0) {
            ddraw->Release();
            return 0;
        }

        memset(&surfaceDesc, 0, sizeof(surfaceDesc));
        surfaceDesc.dwSize = 0x6c;
        *(DWORD *)((char *)&surfaceDesc + 4) = 1;
        *(DWORD *)((char *)&surfaceDesc + 0x68) = 0x200;
        result = ddraw->CreateSurface(&surfaceDesc, (IDirectDrawSurface **)((char *)&ddraw + 4), 0);
        if (result == 0) {
            paletteResult = FUN_0047c150(*(void **)((char *)&ddraw + 4));
            if (paletteResult == 0) {
                hdc = GetDC(hwnd);
                GetSystemPaletteEntries(hdc, 0, 0x100, entries);
                for (i = 0; i < 10; ++i) entries[i].peFlags = 0;
                for (i = 10; i < 246; ++i) entries[i].peFlags = 4;
                for (i = 246; i < 256; ++i) entries[i].peFlags = 0;
                ReleaseDC(hwnd, hdc);
                ddraw->CreatePalette(4, entries, (IDirectDrawPalette **)((char *)&ddraw + 0x10), 0);
                if (*(void **)((char *)&ddraw + 0x14))
                    ((IDirectDrawSurface *)*(void **)((char *)&ddraw + 4))->SetPalette(
                        *(IDirectDrawPalette **)((char *)&ddraw + 0x10));
            }
            SetWindowPos(hwnd, 0, 0, 0, *(int *)((char *)configuration + 4),
                         *(int *)((char *)configuration + 8), 2);
        }
    }
    return 0;
}
