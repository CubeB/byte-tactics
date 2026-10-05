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

void __stdcall FUN_004ba200(unsigned char* palette, int first, int count);
void __stdcall FUN_004c69a0(int param_1);
int __stdcall FUN_004c5e70(Surface_0045fbc0* out);
void __stdcall FUN_004c6890(Surface_0045fbc0* surface, int color);
int __stdcall FUN_004c5fa0(Surface_0045fbc0* s);
void FUN_004c63a0();

// FUNCTION: 0x45fbc0
void FUN_0045fbc0()
{
    Surface_0045fbc0 screen;
    unsigned char palette[0x400];

    memset(palette, 0, sizeof(palette));
    FUN_004ba200(palette, 0, 0x100);
    FUN_004c69a0(g_game->field_37e1b);
    if (FUN_004c5e70(&screen)) {
        FUN_004c6890(&screen, g_game->field_dcb);
        FUN_004c5fa0(&screen);
        FUN_004c63a0();
    }
}
