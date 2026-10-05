// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1b];
    int field_37e1b;                   // +0x37e1b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SetOffscreenSurface(int param_1);
void __stdcall FillSurface(int param_1, int param_2);
void FlipScreen();

// FUNCTION: 0x4257a0
void FUN_004257a0()
{
    SetOffscreenSurface(g_game->field_37e1b);
    FillSurface(0, 0);
    FlipScreen();
}
