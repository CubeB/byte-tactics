// Decompiled by Opus. Names are provisional.
// Releases the offscreen object created by 0x490ac0.

void __cdecl FUN_004d85a0(int* param_1);
void __stdcall SetRestoreSurface(int param_1);
void RestoreScreen();

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1b];
    int* field_37e1b;                  // +0x37e1b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x490b00
void FUN_00490b00()
{
    FUN_004d85a0(g_game->field_37e1b);
    g_game->field_37e1b = 0;
    SetRestoreSurface(0);
    RestoreScreen();
}
