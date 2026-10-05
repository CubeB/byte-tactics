// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_0042a3b0 {
    char unknown_0[0x391f9];
    int* field_391f9;                  // +0x391f9
    int* field_391fd;                  // +0x391fd
};
#pragma pack(pop)

extern Game_0042a3b0* g_game;

void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x42a3b0
void FUN_0042a3b0()
{
    FUN_004d85a0(g_game->field_391fd);
    g_game->field_391fd = 0;
    FUN_004d85a0(g_game->field_391f9);
    g_game->field_391f9 = 0;
}
