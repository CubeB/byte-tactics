// Decompiled by Opus. Names are provisional.
// Reads the system palette into entries[], marking the 20 static colours
// (10 at each end) as plain and the 236 in between as PC_NOCOLLAPSE.
#include <windows.h>

class Class_0047c230 {
public:
    char unknown_0[0xc];
    HWND hwnd;                          // +0x0c
    PALETTEENTRY entries[256];          // +0x10

    void ReadSystemPalette(int unused);
};

// FUNCTION: 0x47c230
void Class_0047c230::ReadSystemPalette(int unused)
{
    HDC hdc = GetDC(hwnd);
    GetSystemPaletteEntries(hdc, 0, 256, entries);
    int i;
    for (i = 0; i < 10; i++)
        entries[i].peFlags = 0;
    for (i = 10; i < 246; i++)
        entries[i].peFlags = PC_NOCOLLAPSE;
    for (i = 246; i < 256; i++)
        entries[i].peFlags = 0;
    ReleaseDC(hwnd, hdc);
}
