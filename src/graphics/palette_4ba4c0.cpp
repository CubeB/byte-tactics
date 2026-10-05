// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Fetches the current 256-entry palette (from the DirectDraw palette object
// when there is no GDI palette, otherwise through GetPaletteEntries) and
// converts entries [first, first+count) into 4-byte RGB0 pixels written to
// the caller's buffer. Returns 0 if the palette could not be read, 1 otherwise.
#include <windows.h>
#include <ddraw.h>

#pragma pack(push, 1)
struct Display_004ba4c0 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x4c - 0x48];
    HPALETTE hpalette;                 // +0x4c
    char unknown_50[0x94 - 0x50];
    IDirectDrawPalette* palette;       // +0x94
    char unknown_98[0xf0 - 0x98];
    unsigned short bit0_1 : 2;         // +0xf0
    unsigned short flag2 : 1;
    unsigned short bit3_15 : 13;
};

struct RGBX_004ba4c0 {
    unsigned char r, g, b, x;
};
#pragma pack(pop)

Display_004ba4c0* GetDisplay(void);

// FUNCTION: 0x4ba4c0
int __stdcall GetPaletteColors(unsigned char* dest, int first, int count)
{
    PALETTEENTRY pal[256];
    Display_004ba4c0* d = GetDisplay();
    if (d->flag2) {
        if (d->field_44 != 0) {
            if (GetPaletteEntries(d->hpalette, 0, 0x100, pal) == 0)
                return 0;
        } else {
            HRESULT hr = d->palette->GetEntries(0, 0, 0x100, pal);
            if (hr != 0)
                return 0;
        }
    }
    RGBX_004ba4c0* out = (RGBX_004ba4c0*)dest;
    count += first;
    for (int i = first; i < count; i++) {
        out[i].r = pal[i].peRed;
        out[i].g = pal[i].peGreen;
        out[i].b = pal[i].peBlue;
        out[i].x = 0;
    }
    return 1;
}
