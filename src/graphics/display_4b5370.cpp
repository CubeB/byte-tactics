// Decompiled by space-bunny-free. Names are provisional.
#include <windows.h>
#include <ddraw.h>

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

struct Display_004b5370 {
    char unknown_0[0x44];
    HGDIOBJ old_palette;               // +0x44, set in GDI mode
    char unknown_48[0x84 - 0x48];
    LPDIRECTDRAW2 ddraw;               // +0x84, set in DirectDraw mode
};

extern Display_004b5370* g_display;

HRESULT __stdcall EnumModesCallback(DDSURFACEDESC* desc, void* context);

// FUNCTION: 0x4b5370
// Fills the list with the display modes to offer. In GDI mode the modes come
// from a fixed table plus the two big desktop sizes when the screen is big
// enough; in DirectDraw mode they come from EnumDisplayModes. Returns 1.
int __stdcall GetDisplayModes(ModeList_004b5330* list)
{
    if (g_display->old_palette) {
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
    hr = g_display->ddraw->QueryInterface(IID_IDirectDraw2, (LPVOID*)&ddraw2);
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
