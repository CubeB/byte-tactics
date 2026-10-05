// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391cb];
    int* field_391cb;                  // +0x391cb
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x42e120
void FreeDownloadMenus()
{
    FUN_004d85a0(g_game->field_391cb);
}
