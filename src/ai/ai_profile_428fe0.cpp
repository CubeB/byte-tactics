// Decompiled by Opus. Names are provisional.
// Same shape as FUN_00428fc0, calling FUN_004d8780 on the same game field.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1439b];
    void* field_1439b;                 // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d8780(void* param_1);

// FUNCTION: 0x428fe0
void FUN_00428fe0()
{
    FUN_004d8780(g_game->field_1439b);
}
