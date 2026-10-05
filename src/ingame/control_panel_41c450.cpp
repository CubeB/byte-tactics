// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1422b];
    int mapWidth;                    // +0x1422b
    int mapHeight;                   // +0x1422f
    char unknown_14233[0x14327 - 0x14233];
    int x;                           // +0x14327
    int y;                           // +0x1432b
    char unknown_1432f[0x37e37 - 0x1432f];
    int viewWidth;                   // +0x37e37
    int viewHeight;                  // +0x37e3b
};
#pragma pack(pop)

extern Game* g_game;

// Same clamp as ClampCameraPosition, on the second position pair.
// FUNCTION: 0x41c450
void ClampCameraTarget()
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
}
