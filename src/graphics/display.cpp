// Decompiled by Opus, space-bunny-free, GPT-6.1-sol, deepseek-v4.1, deepseek-v4.1-flash, Space Bunny Free, Claude Fable 5.1, DeepSeek V4.1 Flash, GPT-6, Claude Opus 5.5, Sonnet, muse-spark-1.3-free, Haiku and Sonnet 5.5. Names are provisional.
// The display module: the display context (window, DirectDraw and GDI set-up,
// palette, screen size, flags and work area), the installed DirectX version
// check, the display mode list, the timer table and the frame rate counter.
#include <windows.h>
#include <ddraw.h>
#include <string.h>
#include <stdlib.h>

// A 16-byte rectangle: the display's current and start mode, and the argument
// of PointInRect.
struct Rect_004b6720 {
    int left;
    int top;
    int right;
    int bottom;
};

// +0xf0: the display context's state flags. Set through the bitfield and read
// whole by SetFullScreen.
union Flags_4b5980 {
    unsigned short value;
    struct {
        unsigned short opt : 1;        // bit 0
        unsigned short bit1 : 1;       // bit 1, full screen
        unsigned short gdi : 1;        // bit 2
        unsigned short bit3 : 1;       // bit 3
        unsigned short bit4 : 1;       // bit 4
        unsigned short has_c0 : 1;     // bit 5
        unsigned short has_c4 : 1;     // bit 6
        unsigned short has_c8 : 1;     // bit 7
        unsigned short has_cc : 1;     // bit 8
        unsigned short has_d0 : 1;     // bit 9
        unsigned short sound_opt : 1;  // bit 10
        unsigned short quitting : 1;   // bit 11
        unsigned short bit12 : 4;      // bits 12 to 15
    } bits;
};

// The frame counter at +0x1da: milliseconds accumulated since the last sample,
// the last tick, the frames since then and the rate of the last second.
struct FrameCounter_4b66a0 {
    int accum;                       // +0x0
    unsigned int lastTick;           // +0x4
    int frames;                      // +0x8
    int rate;                        // +0xc

    void Tick()
    {
        unsigned int now = GetTickCount();
        accum += now - lastTick;
        lastTick = now;
        frames++;
        if (accum > 2000) {
            accum = 1000;
        }
        if (accum > 1000) {
            rate = frames;
            accum -= 1000;
            frames = 0;
        }
    }
};

// The display's DirectDraw objects at +0x84.
struct DirectDrawState {
    IDirectDraw *ddraw;          // +0x84
    IDirectDrawSurface *primary; // +0x88
    IDirectDrawSurface *back;    // +0x8c
    IDirectDrawClipper *clipper; // +0x90
    IDirectDrawPalette *palette; // +0x94
    void *field_98;              // +0x98
    int field_9c;                // +0x9c
};

// The display context at g_display. Packed to 2: the mode struct and the two
// ints after it sit at 0x1ea, 0x1fa and 0x1fe, and videoFlags is a word at
// +0x202.
#pragma pack(push, 2)
struct App_4b5980 {
    HINSTANCE hInstance;                 // +0x00
    int nCmdShow;                        // +0x04
    char* className;                     // +0x08
    char* title;                         // +0x0c
    WNDPROC wndProc;                     // +0x10
    unsigned short menuId;               // +0x14
    // Pads WNDCLASSA to +0x18 by hand, for the same packing reason.
    char unknown_16[2];
    WNDCLASSA wc;                        // +0x18
    HWND hwnd;                           // +0x40
    HBITMAP dib;                         // +0x44
    HDC dc;                              // +0x48
    HPALETTE hpalette;                   // +0x4c
    char unknown_50[0x80 - 0x50];
    void (__cdecl *callback)(int, int, int); // +0x80
    DirectDrawState draw;                // +0x84
    Rect_004b6720 mode;                  // +0xa0
    HFONT font;                          // +0xb0
    char unknown_b4[0xc4 - 0xb4];
    int obj_c4;                          // +0xc4
    int obj_c8;                          // +0xc8
    char unknown_cc[0xd4 - 0xcc];
    int width;                           // +0xd4
    int height;                          // +0xd8
    int unknown_dc;                      // +0xdc
    int active;                          // +0xe0
    char unknown_e4[0xe8 - 0xe4];
    int tickScale;                       // +0xe8, ticks per second
    int wa_left;                         // +0xec, work area left
    Flags_4b5980 flags;                  // +0xf0, work area top
    char wa_rest[0xfc - 0xf2];
    char unknown_fc[0x1da - 0xfc];
    FrameCounter_4b66a0 counter;         // +0x1da
    Rect_004b6720 startMode;             // +0x1ea
    int startWidth;                      // +0x1fa
    int startHeight;                     // +0x1fe
    unsigned short videoFlags;           // +0x202
    char unknown_204[0x214 - 0x204];
    PALETTEENTRY entries[256];           // +0x214
    float scale;                         // +0x614
    void** items;                        // +0x618
    int itemCount;                       // +0x61c
    int availPhys;                       // +0x620
    int unknown_624;                     // +0x624
    char unknown_628[0x728 - 0x628];
    unsigned char unknown_728;           // +0x728
};
#pragma pack(pop)

// The installed DirectX version, in four 16-bit halves.
struct DXVersion {
    unsigned int majhi;
    unsigned int majlo;
    unsigned int minhi;
    unsigned int minlo;
    // memset zero: the four halves come out as copies of one zero register.
    DXVersion() { memset(this, 0, sizeof(*this)); }
};

typedef int (__stdcall *FN_DIRECTXSETUPGETVERSION)(DWORD* major, DWORD* minor);

// One display mode, as the EnumModesCallback enum callback stores it.
struct Mode_004b5330 {
    DWORD width;                       // +0x0
    DWORD height;                      // +0x4
    DWORD refreshRate;                 // +0x8
};

// The caller's list: a count and the array of modes to fill.
struct ModeList_004b5330 {
    int count;                         // +0x0
    Mode_004b5330* modes;              // +0x4
};

// A point the mouse code starts from.
struct View_4b5980 {
    int x;
    int y;
    int z;
    int unknown_0c;
    int unknown_10;
    int unknown_14;
};

struct Surface {
    int data[12];
};

struct BitmapInfo_004b5510 {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[256];
};

// 24 byte event packet shared with SetCurrentMouseEvent and PushMouseEvent.
struct Event_4b5cc0 {
    int x;          // +0x00
    int y;          // +0x04
    int buttons;    // +0x08
    int time;       // +0x0c
    int message;    // +0x10
    int flag;       // +0x14
};

// One entry of the timer table at g_timerSlots: ten slots, then g_timerCount
// counts them.
struct Timer_4b63f0 {
    void* callback;                    // +0
    int id;                            // +4, the handle the callback removes
    int interval;                      // +8, -1 when the slot is free
    int countdown;                     // +0xc
};

typedef void (__stdcall *TimerCb_4b63f0)(int);

extern App_4b5980* g_display;
extern void (__cdecl *g_closeHandler)(int);
extern int g_closeHandlerArg;
extern Timer_4b63f0 g_timerSlots[10];
extern int g_timerSlot0Interval;       // the interval field of the first slot
extern int g_timerCount;
extern unsigned int g_lastTimerTick;   // last tick count, in game ticks
extern LONG g_gfxBlitLockHeld;
extern LONG g_gfxBlitLockOwner;
extern HANDLE g_gfxBlitLockEvent;

void SaveStartDirectory(void);
void __stdcall SetCurrentMouseEvent(int* p);
void __stdcall InitKeyQueue(int size);
void __stdcall InitMouse(int count, int start);
int __stdcall AllocShadeTable(App_4b5980* d);
int __stdcall AllocAlphaTable(App_4b5980* d);
int __stdcall AllocLightTable(App_4b5980* d);
int __stdcall AllocGrayTable(App_4b5980* d);
int __stdcall AllocBlueTable(App_4b5980* d);
long __stdcall WindowProc(HWND hwnd, unsigned int msg, unsigned int wparam, unsigned int lparam);
void __stdcall PushKeyCode(int v);
void __stdcall HandleVirtualKey(int v, int flag);
void __stdcall PushMouseEvent(Event_4b5cc0* ev);
int __stdcall DirectDrawCreateThunk(int guid, void *display, int zero);
void __stdcall InitSurface(Surface *s, int width, int height, int a, int b);
int __stdcall LockScreen(Surface *s);
void __cdecl BlitSurface(Surface *dst, void *src, int x, int y);
int __stdcall UnlockScreen(Surface *s);
int __stdcall SetPaletteColors(PALETTEENTRY *entries, int start, int count);
void DestroyPreCleanup(void);
void ShutdownMouse(void);
void __stdcall FreeAlphaTable(App_4b5980* obj);
void __stdcall FreeShadeTable(App_4b5980* obj);
void __stdcall FreeLightTable(App_4b5980* obj);
void __stdcall FreeGrayTable(App_4b5980* obj);
void __stdcall FreeBlueTable(App_4b5980* obj);
void __stdcall HAPI_CloseArchive(void* item);
void __cdecl GameFreeThunk(void* p);

// Re-applies the display palette: in GDI mode selects and realizes the
// HPALETTE on the window's DC, otherwise attaches the DirectDraw palette to
// the primary surface. Returns 1 on success.
// FUNCTION: 0x4b4f50
int ApplyPalette()
{
    App_4b5980* d = g_display;
    if (d->hpalette) {
        HDC dc = GetDC(g_display->hwnd);
        SelectPalette(dc, g_display->hpalette, FALSE);
        RealizePalette(dc);
        ReleaseDC(g_display->hwnd, dc);
        return 1;
    }
    HRESULT hr = E_FAIL;
    if (d->draw.primary && d->draw.palette)
        hr = d->draw.primary->SetPalette(d->draw.palette);
    return hr == DD_OK ? 1 : 0;
}

// Stores a callback and the argument it will be called with.
// FUNCTION: 0x4b4fd0
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param)
{
    g_closeHandler = callback;
    g_closeHandlerArg = param;
}

// Releases the display's DirectDraw objects (palette, surfaces, then the
// DirectDraw object itself) and clears the pointers.
// FUNCTION: 0x4b4ff0
void __stdcall ReleaseDirectDraw(App_4b5980* d)
{
    if (d->draw.ddraw) {
        if (d->draw.palette) {
            d->draw.palette->Release();
            d->draw.palette = 0;
        }
        if (d->draw.back) {
            d->draw.back->Release();
            d->draw.back = 0;
        }
        if (d->draw.primary) {
            d->draw.primary->Release();
            d->draw.primary = 0;
        }
        if (d->draw.clipper) {
            d->draw.clipper->Release();
            d->draw.clipper = 0;
        }
        d->draw.ddraw->Release();
        d->draw.ddraw = 0;
    }
}

// Reads the installed DirectX version: first through dsetup.dll's
// DirectXSetupGetVersion, then, if that fails, through
// HKLM\Software\Microsoft\DirectX (the "InstalledVersion" DWORD on NT, the
// "Version" string on Win9x), and compares the result with the wanted version.
// The caller (0x4263b0) asks for 4.05.00.0155 (DirectX 5), or 3 on NT.
//
// One thing in the original still looks like Cavedog's own bug, kept as it is:
// the "installed major version differs" arm at 0x4b5233, reached when the major
// half is not the wanted one, compares the major half against argument 2 (the
// minor half) instead of against argument 1.
// FUNCTION: 0x4b5070
int __stdcall CheckDirectXVersion(int want0, int want1, int want2, int want3, int want4)
{
    int isNT;
    DXVersion v;
    DWORD status;
    HMODULE lib;

    // status zeroed before LoadLibraryA and isNT after it: orders the two top stores.
    status = 0;
    lib = LoadLibraryA("dsetup.dll");
    isNT = 0;
    if (lib) {
        FARPROC proc = GetProcAddress(lib, "DirectXSetupGetVersion");
        if (proc) {
            DWORD dwMaj = 0;
            DWORD dwMin = 0;

            status = ((FN_DIRECTXSETUPGETVERSION)proc)(&dwMaj, &dwMin);
            if (status) {
                // This order fixes the register assignment of the four halves.
                v.majhi = HIWORD(dwMaj);
                v.minhi = HIWORD(dwMin);
                v.majlo = LOWORD(dwMaj);
                v.minlo = LOWORD(dwMin);
            }
        }
        FreeLibrary(lib);
    }
    if (!status) {
        OSVERSIONINFOA osvi;
        DWORD type;
        DWORD size;
        LONG err;
        char version[30];
        HKEY hKey;

        memset(&v, 0, sizeof(v));
        isNT = 0;
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        if (GetVersionExA(&osvi)) {
            isNT = osvi.dwPlatformId == 2;
        }
        hKey = 0;
        err = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectX", 0, KEY_READ, &hKey);
        if (err == 0) {
            // Separate from status: it takes status's dead frame slot.
            DWORD installed = 0;

            if (isNT) {
                size = 4;
                err = RegQueryValueExA(hKey, "InstalledVersion", 0, &type, (LPBYTE)&installed, &size);
            } else {
                DWORD len = sizeof(version);
                err = RegQueryValueExA(hKey, "Version", 0, &type, (LPBYTE)version, &len);
            }
            RegCloseKey(hKey);
            if (err) {
                goto fail;
            }
            if (isNT) {
                v.majlo = installed & 0xff;
            } else {
                v.majhi = atoi(strtok(version, "."));
                v.majlo = atoi(strtok(0, "."));
                v.minhi = atoi(strtok(0, "."));
                v.minlo = atoi(strtok(0, "."));
            }
        } else {
            goto fail;
        }
    }
    if (isNT) {
        return v.majlo >= (unsigned int)want4;
    }
    if (v.majhi == (unsigned int)want0) {
        if (v.majlo == (unsigned int)want1) {
            if (v.minhi == (unsigned int)want2) {
                return v.minlo >= (unsigned int)want3;
            }
            return v.minhi >= (unsigned int)want2;
        }
        return v.majlo >= (unsigned int)want1;
    }
    return v.majhi >= (unsigned int)want1;
fail:
    memset(&v, 0, sizeof(v));
    return 0;
}

// FUNCTION: 0x4b52e0
void __stdcall InitDisplayDefaults(App_4b5980* p)
{
    p->videoFlags &= 0xfc00;
    p->startWidth = 640;
    p->startHeight = 480;
    p->startMode.left = 0;
    p->startMode.top = 0;
    p->startMode.right = 639;
    p->startMode.bottom = 479;
    p->scale = 1.0f;
}

// IDirectDraw::EnumDisplayModes callback: records every 8-bit display mode
// (width, height, refresh rate) in the caller's list.
// FUNCTION: 0x4b5330
HRESULT __stdcall EnumModesCallback(DDSURFACEDESC* desc, void* context)
{
    ModeList_004b5330* list = (ModeList_004b5330*)context;
    Mode_004b5330* mode = &list->modes[list->count];
    if (desc->ddpfPixelFormat.dwRGBBitCount == 8) {
        mode->width = desc->dwWidth;
        mode->height = desc->dwHeight;
        mode->refreshRate = desc->dwRefreshRate;
        list->count++;
    }
    return DDENUMRET_OK;
}

// Fills the list with the display modes to offer. In GDI mode the modes come
// from a fixed table plus the two big desktop sizes when the screen is big
// enough; in DirectDraw mode they come from EnumDisplayModes. Returns 1.
// FUNCTION: 0x4b5370
int __stdcall GetDisplayModes(ModeList_004b5330* list)
{
    if (g_display->dib) {
        int width;
        int height;
        list->count = 0;
        list->modes[list->count].width = 0x280;
        list->modes[list->count].height = 0x1e0;
        list->modes[list->count].refreshRate = 0;
        list->count++;
        list->modes[list->count].width = 0x320;
        list->modes[list->count].height = 0x258;
        list->modes[list->count].refreshRate = 0;
        list->count++;
        list->modes[list->count].width = 0x400;
        list->modes[list->count].height = 0x300;
        list->modes[list->count].refreshRate = 0;
        list->count++;
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        if (width >= 0x500 && height >= 0x400) {
            list->modes[list->count].width = 0x500;
            list->modes[list->count].height = 0x400;
            list->modes[list->count].refreshRate = 0;
            list->count++;
        }
        if (width >= 0x640 && height >= 0x4b0) {
            list->modes[list->count].width = 0x640;
            list->modes[list->count].height = 0x4b0;
            list->modes[list->count].refreshRate = 0;
            list->count++;
        }
        return 1;
    }

    LPDIRECTDRAW2 ddraw2 = 0;
    int result = 1;
    DWORD hr;
    list->count = 0;
    hr = g_display->draw.ddraw->QueryInterface(IID_IDirectDraw2, (LPVOID*)&ddraw2);
    if (hr == 0) {
        hr = ddraw2->EnumDisplayModes(0, 0, list, (LPDDENUMMODESCALLBACK)EnumModesCallback);
        if (hr != 0)
            result = 0;
    } else {
        result = 0;
    }
    if (ddraw2)
        ddraw2->Release();
    return result;
}

// Switches the display between DirectDraw full screen (mode != 0: cooperative
// level, display mode, flipping primary with one back buffer, clipper, palette,
// then redraws the saved picture) and a windowed GDI DIB section (mode == 0),
// under the display lock at g_gfxBlitLockHeld, and finally reloads the palette.
//
// Takes the DC by reference: the original reloads it at the block head.
static inline void FreeGdi_004b5510(App_4b5980 *d, HDC &dc)
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
    App_4b5980 *d;
    DDSURFACEDESC ddsd;
    BitmapInfo_004b5510 bmi;
    Surface surf;
    DDSCAPS caps;
    HRESULT hr;

    while (1) {
        int result = InterlockedExchange(&g_gfxBlitLockHeld, 0x4d41494e);
        if (result == 0) {
            g_gfxBlitLockOwner = 0x4d41494e;
            lockResult = 0;
            break;
        }
        if (g_gfxBlitLockOwner == 0x4d41494e) {
            lockResult = result;
            break;
        }
        WaitForSingleObject(g_gfxBlitLockEvent, INFINITE);
    }
    d = g_display;
    DirectDrawState *dd = &d->draw;
    dd->field_9c = 0;
    ReleaseDirectDraw(g_display);

    FreeGdi_004b5510(g_display, g_display->dc);
    SetWindowPos(d->hwnd, NULL, 0, 0, g_display->width, g_display->height,
                 SWP_NOZORDER | SWP_NOMOVE);

    if (mode != 0) {
        g_display->flags.value |= 2;

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
        g_display->flags.value &= ~2;

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
        InitSurface((Surface *)&d->unknown_50[0], g_display->width, g_display->height,
                     (g_display->width + 3) & ~3, (int)bits);
        SelectObject(d->dc, d->dib);
        SetWindowPos(d->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    SetPaletteColors(g_display->entries, 0, 0x100);
    if (lockResult == 0) {
        g_gfxBlitLockOwner = 0;
        InterlockedExchange(&g_gfxBlitLockHeld, 0);
        SetEvent(g_gfxBlitLockEvent);
    }
    return 1;

fail:
    if (lockResult == 0) {
        g_gfxBlitLockOwner = 0;
        InterlockedExchange(&g_gfxBlitLockHeld, 0);
        SetEvent(g_gfxBlitLockEvent);
    }
    return 0;
}

// FUNCTION: 0x4b5910
void ToggleFullScreen()
{
    if (g_display->flags.bits.bit1) {
        SetFullScreen(0);
    } else {
        SetFullScreen(1);
    }
}

// FUNCTION: 0x4b5940
void __stdcall SetResolution(int x, int y)
{
    g_display->width = x;
    g_display->height = y;
    SetFullScreen(g_display->flags.bits.bit1);
}

// Suspected original bug: the work area rectangle fetched with
// SystemParametersInfoA(SPI_GETWORKAREA) at +0xec overlaps the flag word at
// +0xf0, which is the rectangle's `top`, and the flag word is overwritten
// three instructions later, so the fetch is pointless.
// FUNCTION: 0x4b5980
int __stdcall InitEnvironment(App_4b5980* d)
{
    g_display = d;
    MEMORYSTATUS mem;
    mem.dwLength = 0x20;
    GlobalMemoryStatus(&mem);
    d->availPhys = mem.dwTotalPhys;
    SystemParametersInfoA(0x5e, 0, (LPRECT)&g_display->wa_left, TRUE);
    SystemParametersInfoA(0x5d, 0, 0, TRUE);
    d->counter.accum = 0;
    d->counter.lastTick = GetTickCount();
    d->counter.frames = 0;
    d->counter.rate = 0;
    d->draw.ddraw = 0;
    d->draw.primary = 0;
    d->draw.back = 0;
    d->draw.clipper = 0;
    d->draw.palette = 0;
    d->draw.field_98 = 0;
    d->unknown_624 = 0;
    d->unknown_dc = 0;
    // Three plain bitfield statements, not one whole-word expression.
    d->flags.bits.quitting = 0;
    d->hwnd = 0;
    d->flags.bits.sound_opt = (d->videoFlags >> 9) & 1;
    InitKeyQueue(0x1e);
    InitMouse(0x14, d->flags.bits.sound_opt);
    View_4b5980 view;
    view.x = 0;
    view.y = 0;
    view.z = 0;
    d->unknown_728 = 0;
    SaveStartDirectory();
    SetCurrentMouseEvent((int *)&view);

    d->items = 0;
    d->itemCount = 0;
    // w is declared before h.
    int w = d->startWidth;
    int h = d->startHeight;
    d->width = w;
    d->height = h;
    // bits 1..8 of the video flags become bits 2..9 of the flag word
    // Named int local: adds the temporary the original's second flag update has.
    int subsys = (d->videoFlags & 0x1fe) << 1;
    d->flags.value = (d->flags.value & 0xfc03) | subsys | 1;
    if (d->flags.bits.has_c4) {
        AllocShadeTable(d);
    } else {
        d->obj_c4 = 0;
    }
    if (d->flags.bits.has_c0) {
        AllocAlphaTable(d);
    }
    if (d->flags.bits.has_c8) {
        AllocLightTable(d);
    } else {
        d->obj_c8 = 0;
    }
    if (d->flags.bits.has_cc) {
        AllocGrayTable(d);
    }
    if (d->flags.bits.has_d0) {
        AllocBlueTable(d);
    }
    if (d->flags.bits.gdi) {
        d->dc = 0;
        d->hpalette = 0;
        d->dib = 0;
        d->callback = 0;
        d->mode = d->startMode;
        d->wc.lpfnWndProc = (WNDPROC)WindowProc;
        d->wc.style = 8;
        d->wc.hInstance = d->hInstance;
        d->wc.lpszClassName = d->className;
        d->wc.hIcon = LoadIconA(d->hInstance, IDI_APPLICATION);
        // IDC_ARROW passed as the raw Win16 value 103.
        d->wc.hCursor = LoadCursorA(d->hInstance, (LPCSTR)103);
        d->wc.lpszMenuName = (LPSTR)d->menuId;
        d->wc.cbClsExtra = 0;
        d->wc.cbWndExtra = 0;
        d->wc.hbrBackground = GetStockObject(BLACK_BRUSH);
        unsigned int cls = RegisterClassA(&d->wc);
        if (cls != 0) {
            d->hwnd = CreateWindowExA(WS_EX_APPWINDOW, d->className, d->title,
                                      WS_POPUP | WS_VISIBLE | WS_SYSMENU,
                                      CW_USEDEFAULT, CW_USEDEFAULT, w, h,
                                      0, 0, d->hInstance, 0);
            if (d->hwnd != 0) {
                ShowWindow(d->hwnd, d->nCmdShow);
                UpdateWindow(d->hwnd);
                int r = SetFullScreen(d->videoFlags & 1);
                if (r != 0) {
                    return 1;
                }
            }
        }
        ReleaseDirectDraw(d);
        if (d->dc) {
            DeleteDC(d->dc);
        }
        if (d->hpalette) {
            DeleteObject(d->hpalette);
        }
        if (d->dib) {
            DeleteObject(d->dib);
        }
        d->dc = 0;
        d->hpalette = 0;
        d->dib = 0;
        MessageBoxA(d->hwnd, "Error:  Environment Initialization Failed!\nCheck your DirectX setup", d->title, 0);
        DestroyWindow(d->hwnd);
        return 0;
    }
    return 1;
}

// Window procedure of the main application window: translates the custom
// display messages and forwards the rest to the default handler.
// FUNCTION: 0x4b5cc0
long __stdcall WindowProc(HWND hwnd, unsigned int msg, unsigned int wparam,
                            unsigned int lparam)
{
    Event_4b5cc0 e;
    switch (msg) {
    case WM_CREATE:
        return 0;
    case WM_DESTROY:
        if (g_display->flags.bits.bit1)
            SetFullScreen(0);
        PostQuitMessage(0);
        return 0;
    case WM_ACTIVATE:
        if ((unsigned short)wparam == 0)
            g_display->active = 0;
        else
            g_display->active = 1;
        return 0;
    case WM_CLOSE:
        if (g_closeHandler != 0) {
            g_closeHandler(g_closeHandlerArg);
            return 0;
        }
        DestroyWindow(hwnd);
        return 0;
    case WM_KEYDOWN:
        HandleVirtualKey(wparam, 0);
        return 0;
    case WM_CHAR:
        PushKeyCode(wparam);
        return 0;
    case WM_SYSKEYDOWN:
        HandleVirtualKey(wparam, 1);
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    case WM_SYSCOMMAND:
        if (wparam == 0xf100)
            return 0;
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    case WM_MOUSEMOVE:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * g_display->tickScale / 1000;
        e.flag = 0;
        e.message = msg;
        SetCurrentMouseEvent((int*)&e);
        return 0;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * g_display->tickScale / 1000;
        e.flag = 0;
        break;
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * g_display->tickScale / 1000;
        e.flag = 1;
        break;
    case 0x219:
        if (g_display->callback != 0)
            g_display->callback(0x219, wparam, lparam);
        return 1;
    case 0x30f: {
        // Single result local and one `return ok;`: keeps the return path folded.
        long ok;
        if (g_display->hpalette) {
            HDC dc = GetDC(g_display->hwnd);
            SelectPalette(dc, g_display->hpalette, FALSE);
            RealizePalette(dc);
            ReleaseDC(g_display->hwnd, dc);
            ok = 1;
        }
        else {
            HRESULT hr = E_FAIL;
            if (g_display->draw.primary && g_display->draw.palette)
                hr = g_display->draw.primary->SetPalette(g_display->draw.palette);
            ok = hr == DD_OK ? 1 : 0;
        }
        return ok;
    }
    case 0x311:
        if (g_display->hwnd == (HWND)wparam)
            return 0;
        if (g_display->hpalette) {
            HDC dc = GetDC(g_display->hwnd);
            SelectPalette(dc, g_display->hpalette, FALSE);
            RealizePalette(dc);
            ReleaseDC(g_display->hwnd, dc);
            return 0;
        }
        if (g_display->draw.primary && g_display->draw.palette)
            g_display->draw.primary->SetPalette(g_display->draw.palette);
        return 0;
    case 0x3b9:
        if (g_display->callback != 0)
            g_display->callback(0x3b9, wparam, lparam);
        return 1;
    default:
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    e.message = msg;
    PushMouseEvent(&e);
    return 0;
}

// Shuts a display object down: stops the input thread, frees the item list and
// the five cached objects, releases the DirectDraw surfaces and the GDI
// objects, then asks for the work area again.
// FUNCTION: 0x4b6110
void __stdcall ShutdownEnvironment(App_4b5980* d)
{
    DestroyPreCleanup();
    ShutdownMouse();
    for (int i = 0; i < d->itemCount; i++)
        HAPI_CloseArchive(d->items[i]);
    if (d->items)
        GameFreeThunk(d->items);
    if (d->flags.bits.has_c4)
        FreeShadeTable(d);
    if (d->flags.bits.has_c0)
        FreeAlphaTable(d);
    if (d->flags.bits.has_c8)
        FreeLightTable(d);
    if (d->flags.bits.has_cc)
        FreeGrayTable(d);
    if (d->flags.bits.has_d0)
        FreeBlueTable(d);
    ReleaseDirectDraw(d);
    if (d->dc)
        DeleteDC(d->dc);
    if (d->hpalette)
        DeleteObject(d->hpalette);
    if (d->dib)
        DeleteObject(d->dib);
    d->dc = 0;
    d->hpalette = 0;
    d->dib = 0;
    SystemParametersInfoA(0x5d, (unsigned int)g_display->wa_left, 0, 1);
}

// FUNCTION: 0x4b6220
int GetDisplay(void)
{
    return (int)g_display;
}

// Shuts the application down: marks it as quitting, calls SetFullScreen(0)
// when flag 1 is set, shows an optional message box and posts WM_DESTROY
// to the main window.
// FUNCTION: 0x4b6230
void __stdcall QuitApp(char* message)
{
    g_display->flags.bits.quitting = 1;
    if (g_display->flags.bits.bit1) {
        SetFullScreen(0);
    }
    if (message) {
        MessageBoxA(g_display->hwnd, message, g_display->title, 0);
    }
    PostMessageA(g_display->hwnd, WM_DESTROY, 0, 0);
}

// Fatal error: shows an optional message box and exits the process.
// FUNCTION: 0x4b6290
void __stdcall FatalError(char* message)
{
    if (message) {
        MessageBoxA(g_display->hwnd, message, g_display->title, MB_ICONHAND | MB_SYSTEMMODAL);
    }
    exit(1);
}

// FUNCTION: 0x4b62c0
int __stdcall EmptyPostArchiveMountHook(int)
{
    return 0;
}

// FUNCTION: 0x4b62d0
void __stdcall InitTimers(int param_1)
{
    g_display->tickScale = param_1;
    g_timerCount = 0;
    for (int i = 0; i < 10; i++) {
        g_timerSlots[i].interval = -1;
    }
    g_lastTimerTick = (GetTickCount() * g_display->tickScale) / 1000;
}

// FUNCTION: 0x4b6330
int GetTickRate()
{
    return g_display->tickScale;
}

// Milliseconds since boot scaled by the rate at +0xe8 of the object at
// g_display, divided by 1000 (unsigned, so `mul` by the magic number).
// FUNCTION: 0x4b6340
unsigned int GetTicks()
{
    return GetTickCount() * g_display->tickScale / 1000;
}

// FUNCTION: 0x4b6370
void UpdateTimers()
{
    unsigned int q1 = (GetTickCount() * g_display->tickScale) / 1000;
    int diff = (int)q1 - g_lastTimerTick;
    g_lastTimerTick = (GetTickCount() * g_display->tickScale) / 1000;

    int* p = &g_timerSlot0Interval;
    do {
        if (p[0] >= 0) {
            if ((p[1] -= diff) <= 0) {
                int arg = p[-1];
                TimerCb_4b63f0 fn = (TimerCb_4b63f0)p[-2];
                fn(arg);
                p[1] = p[0];
            }
        }
        p += 4;
    } while ((int)p < (int)&g_timerCount);
}

// FUNCTION: 0x4b63f0
int __stdcall AddTimer(int interval, int id, TimerCb_4b63f0 callback)
{
    unsigned int now = (GetTickCount() * g_display->tickScale) / 1000;
    int diff = (int)now - g_lastTimerTick;
    g_lastTimerTick = (GetTickCount() * g_display->tickScale) / 1000;

    int* p = &g_timerSlot0Interval;
    do {
        if (*p >= 0) {
            if ((p[1] -= diff) <= 0) {
                TimerCb_4b63f0 fn = (TimerCb_4b63f0)p[-2];
                fn(p[-1]);
                p[1] = *p;             // reload the whole interval
            }
        }
        p += 4;
    } while ((int)p < (int)&g_timerCount);

    int i = 0;
    int* q = &g_timerSlot0Interval;
    // The bound is the end of the ten slots. Written relative to the first one
    // so the entry test folds away, the way the original's code has none.
    for (; (int)q < (int)&g_timerSlot0Interval + 160; q += 4, i++) {
        if (*q < 0) {
            g_timerSlots[i].callback = callback;
            g_timerSlots[i].id = id;
            g_timerSlots[i].interval = interval;
            g_timerSlots[i].countdown = interval;
            g_timerCount++;
            return i;
        }
    }
    return -1;
}

// Disables one entry of the timer table (period = -1, see 0x4b6510);
// returns 0 when the index is out of range.
// FUNCTION: 0x4b64d0
int __stdcall RemoveTimer(int i)
{
    if (i >= g_timerCount)
        return 0;
    if (i < 0)
        return 0;
    g_timerSlots[i].interval = -1;
    return 1;
}

// Resets the timer table (period = -1 disables an entry) and restarts the
// clock used by UpdateTimers.
// FUNCTION: 0x4b6510
void ResetTimers()
{
    g_timerCount = 0;
    for (int i = 0; i < 10; i++) {
        g_timerSlots[i].interval = -1;
    }
    g_lastTimerTick = GetTickCount() * g_display->tickScale / 1000;
}

// FUNCTION: 0x4b6560
unsigned long GetMilliseconds(void)
{
    return GetTickCount();
}

// Frame rate counter: adds the time since the last call to the counter, and once
// a second stores the number of frames in that second as the frame rate. When
// there is no offscreen GDI device context it draws "FRATE <rate>" over the
// frame through the surface's device context.
// FUNCTION: 0x4b6570
int __stdcall DrawFrameRate()
{
    FrameCounter_4b66a0* c = &g_display->counter;
    unsigned int now = GetTickCount();
    c->accum += now - c->lastTick;
    c->lastTick = now;
    c->frames++;
    if (c->accum > 2000) {
        c->accum = 1000;
    }
    if (c->accum > 1000) {
        c->rate = c->frames;
        c->accum -= 1000;
        c->frames = 0;
    }

    if (!g_display->dib) {
        HDC dc;
        char buf[128];
        if (g_display->draw.back->GetDC(&dc) == 0) {
            SetBkMode(dc, TRANSPARENT);
            HGDIOBJ oldFont = SelectObject(dc, g_display->font);
            int len = wsprintf(buf, "FRATE %d", g_display->counter.rate);
            SetTextColor(dc, RGB(255, 255, 0));
            TextOut(dc, 0, 0, buf, len);
            SelectObject(dc, oldFont);
            g_display->draw.back->ReleaseDC(dc);
        }
    }

    return g_display->counter.rate;
}

// FUNCTION: 0x4b66a0
int GetFrameRate()
{
    g_display->counter.Tick();
    return g_display->counter.rate;
}

// FUNCTION: 0x4b6700
int GetScreenWidth()
{
    return g_display->width;
}

// FUNCTION: 0x4b6710
int GetScreenHeight()
{
    return g_display->height;
}

// FUNCTION: 0x4b6720
int __stdcall PointInRect(Rect_004b6720* r, int x, int y)
{
    if (x < r->left || x > r->right)
        return 0;
    if (y < r->top || y > r->bottom)
        return 0;
    return 1;
}
