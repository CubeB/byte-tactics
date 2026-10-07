// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// For every map cell whose feature index is below 0xfffb and whose feature
// definition has a non-zero value at +0xf0 and bit 9 set in its flags word,
// stamp the feature's value into the byte at +7 of every cell covered by the
// feature footprint.
//
// The flags are a 16-bit word at +0xfe (0x423c50 and 0x422170 test other
// bits of it).

// Needed: without it the cells walk loses its absolute pointer.
#include <windows.h>

#pragma pack(push, 1)
struct Feature {
    char unknown_0[0x94];
    short footprintX;                  // +0x94
    short footprintY;                  // +0x96
    char unknown_98[0xf0 - 0x98];
    float value;                       // +0xf0
    char unknown_f4[0xfe - 0xf4];
    // A 16-bit word, not an unsigned char at +0xff: that swaps two registers.
    unsigned short flags;              // +0xfe
};

struct Cell {
    char unknown_0[7];
    char field7;                       // +0x7
    unsigned short feature;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell* cells;                       // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

Cell* __stdcall GetMapCell(int x, int y);

// FUNCTION: 0x422040
void StampFeatureMetal(void)
{
    Cell* c = g_game->cells;
    for (int i = 0; i < g_game->width * g_game->height; i++, c++) {
        if (c->feature < 0xfffb) {
            Feature* f = &g_game->features[c->feature];
            if (f->value != 0.0f && (f->flags & 0x200)) {
                int x0 = i % g_game->width;
                int y0 = i / g_game->width;
                for (int y = y0; y < y0 + f->footprintY; y++) {
                    for (int x = x0; x < x0 + f->footprintX; x++) {
                        Cell* cc = GetMapCell(x, y);
                        if (cc != 0)
                            cc->field7 = (char)f->value;
                    }
                }
            }
        }
    }
}
