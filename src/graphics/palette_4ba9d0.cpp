// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Finds the palette entry closest to `color` whose brightness band `band[k]`
// is within 40 of the colour's brightness; returns `order[bestIndex]`. The
// band pointer stays in the `band` argument's own stack slot (MSVC turns
// `band[i]` into an induction variable in place) and the squared distance is
// written as one expression, not as three locals, which is what puts the
// loop counter in edx.
#include <windows.h>

// FUNCTION: 0x4ba9d0
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* band, unsigned char* order, PALETTEENTRY color)
{
    int best = 1000000000;
    unsigned char bestIndex;
    int sum = color.peRed + (color.peGreen + color.peBlue);
    int i;
    for (i = 0; i < 256; i++) {
        if (band[i] >= sum - 40) {
            if (band[i] > sum + 40)
                break;
            int k = order[i];
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
