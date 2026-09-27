// Decompiled by Opus. Names are provisional.
// Returns the index of the palette entry closest to `color` (squared RGB
// distance). The colour fields are read in the loop in the source; MSVC
// hoists their widened values out of it, which is why the loop counter,
// not the colour, gets a callee-saved register.
#include <windows.h>

// FUNCTION: 0x4ba880
unsigned char __stdcall FUN_004ba880(PALETTEENTRY* palette, PALETTEENTRY color)
{
    int best = 1000000000;
    unsigned char bestIndex;
    for (int i = 0; i < 256; i++) {
        int dr = palette[i].peRed - color.peRed;
        int dg = palette[i].peGreen - color.peGreen;
        int db = palette[i].peBlue - color.peBlue;
        int d = dr * dr + dg * dg + db * db;
        if (d < best) {
            bestIndex = i;
            best = d;
        }
    }
    return bestIndex;
}
