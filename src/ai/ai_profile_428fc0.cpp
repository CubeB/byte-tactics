// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_00428fc0 {
    char unknown_0[0x1439b];
    void* field_1439b;                 // +0x1439b
};
#pragma pack(pop)

extern Game_00428fc0* g_game;

void __cdecl FUN_004d8710(void* param_1);

// FUNCTION: 0x428fc0
void FUN_00428fc0()
{
    FUN_004d8710(g_game->field_1439b);
}
