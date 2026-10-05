// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1b];
    int field_37e1b;                   // +0x37e1b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SetOffscreenSurface(int param_1);
void FUN_004c2470();
void FUN_004c2870();
void FlipScreen();

// FUNCTION: 0x425b60
void FUN_00425b60()
{
    SetOffscreenSurface(g_game->field_37e1b);
    FUN_004c2470();
    FUN_004c2870();
    FlipScreen();
}
