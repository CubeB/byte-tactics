// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Best attempt: 39.8%. The original's lengths and structure are close, but
// MSVC keeps the loop counter in ebx and reloads the hoisted colour bytes into
// caller-saved registers, while the original keeps the counter in edx, the
// palette pointer in edi and the colour bytes (peRed/peGreen/peBlue) in the
// stack slots E-0x10/E-8/E-4. Changing the sum order, the declaration order,
// the distance expression and the loop form (for/while/do-while, pointer or
// indexed) did not move the counter out of ebx. The remaining difference is
// register allocation, not control flow.
#include <windows.h>

// FUNCTION: 0x4ba9d0
unsigned char __stdcall FUN_004ba9d0(PALETTEENTRY* palette, int* band, unsigned char* order, PALETTEENTRY color)
{
    int sum = color.peBlue + color.peGreen + color.peRed;
    int best = 1000000000;
    unsigned char bestIndex;
    int i;
    for (i = 0; i < 256; i++) {
        if (band[i] >= sum - 40) {
            if (band[i] > sum + 40)
                break;
            int k = order[i];
            int r = palette[k].peRed - color.peRed;
            int g = palette[k].peGreen - color.peGreen;
            int b = palette[k].peBlue - color.peBlue;
            int d = r * r;
            d += g * g;
            d += b * b;
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
