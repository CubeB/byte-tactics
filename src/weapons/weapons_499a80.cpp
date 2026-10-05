// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_00499a80 {
    char unknown_0[0x141f7];
    void* field_141f7;                 // +0x141f7
};
#pragma pack(pop)

extern Game_00499a80* g_game;

void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x499a80
void FUN_00499a80(void)
{
    FUN_004d85a0(g_game->field_141f7);
    g_game->field_141f7 = 0;
}
