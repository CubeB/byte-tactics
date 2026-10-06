// Decompiled by Opus. Names are provisional.
// Both toggle protection on the same game field, one read-only and one read-write.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1439b];
    void* field_1439b;                 // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl ProtectBlockReadOnly(void* param_1);
void __cdecl ProtectBlockReadWrite(void* param_1);

// FUNCTION: 0x428fc0
void FUN_00428fc0()
{
    ProtectBlockReadOnly(g_game->field_1439b);
}

// FUNCTION: 0x428fe0
void FUN_00428fe0()
{
    ProtectBlockReadWrite(g_game->field_1439b);
}
