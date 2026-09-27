// Decompiled by Opus. Names are provisional.
// IDirectDraw::EnumDisplayModes callback: records every 8-bit display mode
// (width, height, refresh rate) in the caller's list.
#include <ddraw.h>

struct Mode_004b5330 {
    DWORD width;                       // +0x0
    DWORD height;                      // +0x4
    DWORD refreshRate;                 // +0x8
};

struct ModeList_004b5330 {
    int count;                         // +0x0
    Mode_004b5330* modes;              // +0x4
};

// FUNCTION: 0x4b5330
HRESULT __stdcall FUN_004b5330(DDSURFACEDESC* desc, void* context)
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
