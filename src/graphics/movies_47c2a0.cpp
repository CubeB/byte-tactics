// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <ddraw.h>

struct PaletteSource {
    char unknown_0[0x6c];
    unsigned char rgb[256][3];          // +0x6c
};

struct DisplaySurface {
    char unknown_0[0x10];
    IDirectDrawPalette* palette;        // +0x10
};

class Class_0047c2a0 {
public:
    PaletteSource* source;              // +0x00
    char unknown_4[0xc];
    PALETTEENTRY entries[256];          // +0x10
    char unknown_410[0x134];
    DisplaySurface* display;            // +0x544
    void UpdatePalette(void);
};

// FUNCTION: 0x47c2a0
void Class_0047c2a0::UpdatePalette(void)
{
    unsigned char* src = source->rgb[0];
    for (int i = 0; i < 256; i++) {
        entries[i].peRed = *src++;
        entries[i].peGreen = *src++;
        entries[i].peBlue = *src++;
    }
    display->palette->SetEntries(0, 0, 256, entries);
}
