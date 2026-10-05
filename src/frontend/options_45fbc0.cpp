// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0xdcb];
    unsigned char field_dcb;           // +0xdcb
    char unknown_dcc[0x37e1b - 0xdcc];
    int field_37e1b;                   // +0x37e1b
};
#pragma pack(pop)

struct Surface_0045fbc0 {
    int data[12];
};

extern Game* g_game;

void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall SetOffscreenSurface(int param_1);
int __stdcall LockScreen(Surface_0045fbc0* out);
void __stdcall FillSurface(Surface_0045fbc0* surface, int color);
int __stdcall UnlockScreen(Surface_0045fbc0* s);
void FlipScreen();

// FUNCTION: 0x45fbc0
void FUN_0045fbc0()
{
    Surface_0045fbc0 screen;
    unsigned char palette[0x400];

    memset(palette, 0, sizeof(palette));
    SetPaletteColors(palette, 0, 0x100);
    SetOffscreenSurface(g_game->field_37e1b);
    if (LockScreen(&screen)) {
        FillSurface(&screen, g_game->field_dcb);
        UnlockScreen(&screen);
        FlipScreen();
    }
}
