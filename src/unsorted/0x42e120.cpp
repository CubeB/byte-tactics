// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_0042e120 {
    char unknown_0[0x391cb];
    int* field_391cb;                  // +0x391cb
};
#pragma pack(pop)

extern Game_0042e120* g_game;

void FUN_004d85a0(int* param_1);

// FUNCTION: 0x42e120
void FUN_0042e120()
{
    FUN_004d85a0(g_game->field_391cb);
}
