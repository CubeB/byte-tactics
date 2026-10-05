// Decompiled by Opus. Names are provisional.
// Builds a 256-entry remap table: for each colour of the source palette,
// the index of the closest colour (sum of absolute RGB differences) in the
// destination palette.

#include <stdlib.h>

struct PalEntry_004ac710 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char flags;
};

// FUNCTION: 0x4ac710
void __stdcall FUN_004ac710(PalEntry_004ac710* src, PalEntry_004ac710* dest, unsigned char* table)
{
    for (int n = 256; n != 0; n--) {
        int i = 0;
        int best = 9999999;
        PalEntry_004ac710* p = dest;
        int b = src->b;
        int g = src->g;
        int r = src->r;
        int bestIndex;
        for (; i < 256; i++, p++) {
            int d = abs(b - p->b) + abs(g - p->g) + abs(r - p->r);
            if (d < best) {
                best = d;
                bestIndex = i;
            }
        }
        src++;
        *table++ = (unsigned char)bestIndex;
    }
}
