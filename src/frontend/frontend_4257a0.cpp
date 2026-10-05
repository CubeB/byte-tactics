// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1b];
    int field_37e1b;                   // +0x37e1b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004c69a0(int param_1);
void __stdcall FUN_004c6890(int param_1, int param_2);
void FUN_004c63a0();

// FUNCTION: 0x4257a0
void FUN_004257a0()
{
    FUN_004c69a0(g_game->field_37e1b);
    FUN_004c6890(0, 0);
    FUN_004c63a0();
}
