// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1438f];
    int field_1438f;                   // +0x1438f
    char unknown_14393[4];
    int field_14397;                   // +0x14397
};
#pragma pack(pop)

extern Game* g_game;

void LoadUnitInfo();

// FUNCTION: 0x42bd10
void RefreshUnitInfo()
{
    if (g_game->field_1438f == 0 || g_game->field_14397 != 0) {
        LoadUnitInfo();
        g_game->field_14397 = 0;
    }
}
