// Decompiled by GPT-6-Luna, finished by Space Bunny Free. Names are provisional.
// Sets the movie player up: asks DirectDraw for full screen exclusive on the
// player window, builds the primary surface and a palette out of the system
// palette, sizes the window to the movie, and when the movie has a new palette
// hands it to the Smacker decoder. Returns 1 unless DirectDraw refuses.
#include <windows.h>
#include <ddraw.h>

// The DirectDraw wrapper the player holds: the interface, the primary surface
// and the palette built from it, plus the mode it was set to.
class DDraw_0047bf70 {
public:
    IDirectDraw *lpDD;                 // +0x00
    IDirectDrawSurface *lpSurface;     // +0x04
    int width;                         // +0x08
    int height;                        // +0x0c
    IDirectDrawPalette *lpPalette;     // +0x10
};

// The game's DDSURFACEDESC is 0x6c bytes, so it is not the DirectDraw 1 layout
// in <ddraw.h> here; only the fields this function writes are named.
struct SurfaceDesc_0047bf70 {
    DWORD dwSize;                      // +0x00
    DWORD dwFlags;                     // +0x04
    DWORD dwHeight;                    // +0x08
    DWORD dwWidth;                     // +0x0c
    DWORD dwPitch;                     // +0x10
    char unknown_14[0x68 - 0x14];
    DWORD dwCaps;                      // +0x68, DDSCAPS_PRIMARYSURFACE
};

// The open Smacker movie (smackw32.dll's SMK struct).
struct Smk_0047bf70 {
    DWORD unknown_0;                   // +0x00
    DWORD width;                       // +0x04
    DWORD height;                      // +0x08
    char unknown_c[0x68 - 0xc];
    DWORD newPalette;                  // +0x68
    unsigned char rgb[256][3];         // +0x6c
    DWORD unknown_36c;                 // +0x36c
    unsigned short field_370;          // +0x370
};

// The object smackw32.dll ordinal 2 hands back; its fields are not named
// because no other call site reads them.
struct Smk_0047bf70_b {
    char unknown_0[0x2c];
    DWORD field_2c;                    // +0x2c
    char unknown_30[0x3c - 0x30];
    DWORD field_3c;                    // +0x3c
    char unknown_40[0x43c - 0x40];
    DWORD field_43c;                   // +0x43c
};

class Class_0047bf70 {
public:
    Smk_0047bf70 *video;               // +0x000
    int counter;                       // +0x004
    int stopped;                       // +0x008
    HWND hwnd;                         // +0x00c
    PALETTEENTRY entries[256];         // +0x010
    int paletteResult;                 // +0x410
    char unknown_414[0x544 - 0x414];
    DDraw_0047bf70 *ddraw;             // +0x544
    SurfaceDesc_0047bf70 surfaceDesc;  // +0x548

    int SetupDirectDraw();
};

// The same object seen as the class the pixel format query belongs to
// (0x47c150): it is a member of this one in the original.
class Class_0047c150 {
public:
    int GetBlitMode(void *surface);
};

extern int __stdcall FUN_0049f710(int guid, void *display, int zero);

// smackw32.dll, imported by ordinal, so the exe holds no name for these three.
// Ordinal 2 opens the decoder state (HWND, HDC, 640, 480, 0, 0), ordinal 5
// takes the movie's 256 colour palette, and ordinal 25 is given the movie and
// three fields of that state.
extern "C" __declspec(dllimport) Smk_0047bf70_b *__stdcall DAT_004fc410(HWND hwnd, HDC hdc, int width, int height, int flags, int background);
extern "C" __declspec(dllimport) void __stdcall DAT_004fc414(Smk_0047bf70_b *smk, unsigned char *rgb, unsigned short flag);
extern "C" __declspec(dllimport) void __stdcall DAT_004fc418(Smk_0047bf70 *smk, DWORD *field_3c, DWORD field_2c, DWORD field_43c);

// FUNCTION: 0x47bf70
int Class_0047bf70::SetupDirectDraw()
{
    POINT point;
    int i;
    HDC hdc;

    point.x = 0;
    point.y = 0;
    ClientToScreen(hwnd, &point);
    if (FUN_0049f710(0, ddraw, 0) != 0)
        goto fail;
    if (ddraw->lpDD->SetCooperativeLevel(hwnd, 8) != 0) {
        ddraw->lpDD->Release();
        goto fail;
    }
    memset(&surfaceDesc, 0, sizeof(surfaceDesc));
    surfaceDesc.dwSize = sizeof(surfaceDesc);
    surfaceDesc.dwFlags = 1;
    surfaceDesc.dwCaps = 0x200;
    if (ddraw->lpDD->CreateSurface((LPDDSURFACEDESC)&surfaceDesc, &ddraw->lpSurface, 0) == 0) {
        paletteResult = ((Class_0047c150 *)this)->GetBlitMode(ddraw->lpSurface);
        if (paletteResult == 0) {
            hdc = GetDC(hwnd);
            GetSystemPaletteEntries(hdc, 0, 0x100, entries);
            for (i = 0; i < 10; ++i) entries[i].peFlags = 0;
            for (i = 10; i < 246; ++i) entries[i].peFlags = 4;
            for (i = 246; i < 256; ++i) entries[i].peFlags = 0;
            ReleaseDC(hwnd, hdc);
            if (ddraw->lpDD->CreatePalette(4, entries, &ddraw->lpPalette, 0) == 0)
                ddraw->lpSurface->SetPalette(ddraw->lpPalette);
        }
        SetWindowPos(hwnd, 0, 0, 0, video->width, video->height, 2);
    }
    if (video->newPalette) {
        Smk_0047bf70_b *smk = DAT_004fc410(hwnd, 0, 0x280, 0x1e0, 0, 0);
        DAT_004fc414(smk, &video->rgb[0][0], video->field_370);
        DAT_004fc418(video, &smk->field_3c, smk->field_2c, smk->field_43c);
    }
    return 1;
fail:
    return 0;
}
