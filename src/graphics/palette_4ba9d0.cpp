// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Finds the palette entry closest to `color` whose brightness band `band[k]`
// is within 40 of the colour's brightness; returns `order[bestIndex]`.
#include <windows.h>

// FUNCTION: 0x4ba9d0
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* band, unsigned char* order, PALETTEENTRY color)
{
    int best = 1000000000;
    unsigned char bestIndex;
    int sum = color.peRed + (color.peGreen + color.peBlue);
    int i;
    // Index `band` directly: no local pointer copy.
    for (i = 0; i < 256; i++) {
        if (band[i] >= sum - 40) {
            if (band[i] > sum + 40)
                break;
            int k = order[i];
            // One expression, not three locals: puts the loop counter in edx.
            int d = (palette[k].peRed - color.peRed) * (palette[k].peRed - color.peRed)
                  + (palette[k].peGreen - color.peGreen) * (palette[k].peGreen - color.peGreen)
                  + (palette[k].peBlue - color.peBlue) * (palette[k].peBlue - color.peBlue);
            if (d < best) {
                bestIndex = i;
                best = d;
            }
        }
    }
    if (best == 1000000000)
        bestIndex = i;
    return order[bestIndex];
}
