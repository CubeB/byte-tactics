// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Copies a 256-entry palette into the object and then builds a 256-byte remap
// table: for each colour of the copied palette, the index of the closest
// colour (sum of absolute RGB differences) in the source palette.
//
// Partial (94.7%). Everything matches except the spill slots of the three
// hoisted palette bytes at the top of the outer loop. The original puts the
// p->b value in [esp+0x10], p->r in [esp+0x14] and p->g in [esp+0x18] and
// evaluates the terms in that order (b, r, g). MSVC 5 puts p->r in [esp+0x10],
// p->b in [esp+0x14], p->g in [esp+0x18] and evaluates r, b, g instead. Naming
// the values as locals only moves the problem: with `int b` and `int r` locals
// the slots come out right but the two loads/stores of g and r are emitted in
// the opposite order. Expression order, parenthesisation, casts, byte pointers,
// inlined helpers and index loops were all tried and do not change it.

#include <string.h>
#include <stdlib.h>

struct PalEntry_004ac7d0 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char flags;
};

struct Palette_004ac7d0 {
    unsigned char unknown_0[0xb2];
    PalEntry_004ac7d0 dest[256];       // +0xb2
    unsigned char unknown_4b2[0x400];
    unsigned char table[256];          // +0x8b2
};

// FUNCTION: 0x4ac7d0
void __stdcall FUN_004ac7d0(Palette_004ac7d0* pal, PalEntry_004ac7d0* src, PalEntry_004ac7d0* copy)
{
    memcpy(pal->dest, copy, 0x400);

    unsigned char* table = pal->table;
    PalEntry_004ac7d0* p = pal->dest;
    for (int n = 256; n != 0; n--) {
        int i = 0;
        int best = 9999999;
        PalEntry_004ac7d0* q = src;
        int bestIndex;
        for (; i < 256; i++, q++) {
            int d = abs(p->b - q->b) + abs(p->g - q->g) + abs(p->r - q->r);
            if (d < best) {
                best = d;
                bestIndex = i;
            }
        }
        p++;
        *table++ = (unsigned char)bestIndex;
    }
}
