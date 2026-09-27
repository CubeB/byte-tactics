// Decompiled by Opus. Names are provisional.
// Saves a bitmap as an image file with the current palette: the game's
// PALETTEENTRY table at +0x214 is converted to packed RGB triples first.
#include <windows.h>

struct Bitmap_004caec0 {
    int width;                         // +0x0
    int height;                        // +0x4
    char unknown_8[4];
    unsigned char* data;               // +0xc
};

#pragma pack(push, 1)
struct Game_004caec0 {
    char unknown_0[0x214];
    PALETTEENTRY palette[256];         // +0x214
};
#pragma pack(pop)

Game_004caec0* FUN_004b6220();
int __stdcall FUN_004cac40(char* name, unsigned char* data, int width, int height, unsigned char* palette);

// FUNCTION: 0x4caec0
void __stdcall FUN_004caec0(char* name, Bitmap_004caec0* bitmap)
{
    unsigned char pal[256 * 3];
    Game_004caec0* game = FUN_004b6220();
    for (int i = 0; i < 256; i++) {
        pal[i * 3] = game->palette[i].peRed;
        pal[i * 3 + 1] = game->palette[i].peGreen;
        pal[i * 3 + 2] = game->palette[i].peBlue;
    }
    FUN_004cac40(name, bitmap->data, bitmap->width, bitmap->height, pal);
}
