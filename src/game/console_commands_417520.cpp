// Decompiled by Opus. Names are provisional.
// Chat command handler (table at 0x5020b8): toggles a game flag.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38dd5];
    int field_38dd5;                   // +0x38dd5
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x417520
void __stdcall FUN_00417520(void* args)
{
    g_game->field_38dd5 = g_game->field_38dd5 == 0 ? 1 : 0;
}
