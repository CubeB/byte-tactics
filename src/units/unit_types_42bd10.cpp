// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_0042bd10 {
    char unknown_0[0x1438f];
    int field_1438f;                   // +0x1438f
    char unknown_14393[4];
    int field_14397;                   // +0x14397
};
#pragma pack(pop)

extern Game_0042bd10* g_game;

void FUN_0042a8d0();

// FUNCTION: 0x42bd10
void FUN_0042bd10()
{
    if (g_game->field_1438f == 0 || g_game->field_14397 != 0) {
        FUN_0042a8d0();
        g_game->field_14397 = 0;
    }
}
