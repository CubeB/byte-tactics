// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_41c3c0 {
    char unknown_0[0x1422b];
    int mapWidth;                    // +0x1422b
    int mapHeight;                   // +0x1422f
    char unknown_14233[0x142cb - 0x14233];
    char field_142cb[0x1431f - 0x142cb]; // +0x142cb
    int x;                           // +0x1431f
    int y;                           // +0x14323
    char unknown_14327[0x37e37 - 0x14327];
    int viewWidth;                   // +0x37e37
    int viewHeight;                  // +0x37e3b
};
#pragma pack(pop)

extern Game_41c3c0* g_game;

void __stdcall FUN_00466b70(void* param_1);

// FUNCTION: 0x41c3c0
void FUN_0041c3c0()
{
    int maxX = g_game->mapWidth - g_game->viewWidth;
    int maxY = g_game->mapHeight - g_game->viewHeight;
    if (g_game->x < 0) {
        g_game->x = 0;
    } else if (g_game->x > maxX) {
        g_game->x = maxX;
    }
    if (g_game->y < 0) {
        g_game->y = 0;
    } else if (g_game->y > maxY) {
        g_game->y = maxY;
    }
    FUN_00466b70(g_game->field_142cb);
}
