// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1422b];
    int mapWidth;                    // +0x1422b
    int mapHeight;                   // +0x1422f
    char unknown_14233[0x14281 - 0x14233];
    unsigned short flags_14281;      // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned char flags_142f1;       // +0x142f1
    char unknown_142f2[0x1431f - 0x142f2];
    int x;                           // +0x1431f
    int y;                           // +0x14323
    int x2;                          // +0x14327
    int y2;                          // +0x1432b
    char unknown_1432f[0x37e37 - 0x1432f];
    int viewWidth;                   // +0x37e37
    int viewHeight;                  // +0x37e3b
};
#pragma pack(pop)

struct Pos_41c8e0 {
    int x;                           // +0
    int y;                           // +4
    int z;                           // +8
};

// GLOBAL: 0x511de8
extern Game* g_game;

void ClampCameraPosition(void);

// FUNCTION: 0x41c8e0
void __stdcall CenterCameraOnMapPosition(Pos_41c8e0* p, int param_2)
{
    int cy = (short)((p->z - (p->y >> 1)) >> 16) - g_game->viewHeight / 2;
    int cx = (short)(p->x >> 16) - g_game->viewWidth / 2;
    if (param_2 != 0) {
        g_game->x2 = cx;
        g_game->y2 = cy;
        int maxX = g_game->mapWidth - g_game->viewWidth;
        int maxY = g_game->mapHeight - g_game->viewHeight;
        if (g_game->x2 < 0) {
            g_game->x2 = 0;
        } else if (g_game->x2 > maxX) {
            g_game->x2 = maxX;
        }
        if (g_game->y2 < 0) {
            g_game->y2 = 0;
        } else if (g_game->y2 > maxY) {
            g_game->y2 = maxY;
        }
    } else {
        g_game->x = cx;
        g_game->y = cy;
        g_game->flags_142f1 |= 2;
        ClampCameraPosition();
        g_game->x2 = g_game->x;
        g_game->y2 = g_game->y;
    }
    g_game->flags_14281 &= 0xfff7;
}
