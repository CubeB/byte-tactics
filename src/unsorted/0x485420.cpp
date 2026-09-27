// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#pragma pack(push, 1)
struct Game_00485420 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;     // +0x14273
};
#pragma pack(pop)

extern Game_00485420* g_game;

// Copies one player's visibility bit into another player's bit on every cell.
// FUNCTION: 0x485420
void __stdcall FUN_00485420(unsigned char from, unsigned char to)
{
    unsigned short fromBit = 1 << from;
    unsigned short toBit = 1 << to;
    int n = g_game->width * g_game->height / 4;
    for (int i = 0; i < n; i++) {
        unsigned short* p = &g_game->visibilityMask[i];
        if (*p & fromBit)
            *p |= toBit;
    }
}
