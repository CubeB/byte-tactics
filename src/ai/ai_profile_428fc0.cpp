// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1439b];
    void* field_1439b;                 // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl ProtectBlockReadOnly(void* param_1);

// FUNCTION: 0x428fc0
void FUN_00428fc0()
{
    ProtectBlockReadOnly(g_game->field_1439b);
}
