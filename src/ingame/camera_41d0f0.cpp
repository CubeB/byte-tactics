// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Centres the camera on the screen position stored at +0x2c76, then clamps it.

struct Rect_0041d0f0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int unknown_8[4];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2c76];
    Rect_0041d0f0 view;                // +0x2c76
    char unknown_2c8e[0x1422b - 0x2c8e];
    int world_w;                       // +0x1422b
    int world_h;                       // +0x1422f
    char unknown_14233[0x14281 - 0x14233];
    unsigned short flags_14281;        // +0x14281
    char unknown_14283[0x142e7 - 0x14283];
    short origin_x;                    // +0x142e7
    short origin_y;                    // +0x142e9
    short screen_w;                    // +0x142eb
    short screen_h;                    // +0x142ed
    char unknown_142ef[0x142f1 - 0x142ef];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2;
    int value_142f3;                   // +0x142f3
    int value_142f7;                   // +0x142f7
    char unknown_142fb[0x1431f - 0x142fb];
    int x;                             // +0x1431f
    int y;                             // +0x14323
    int x2;                            // +0x14327
    int y2;                            // +0x1432b
    char unknown_1432f[0x1434b - 0x1432f];
    short value_1434b;                 // +0x1434b
    char unknown_1434d[0x37e37 - 0x1434d];
    int viewWidth;                     // +0x37e37
    int viewHeight;                    // +0x37e3b
};
#pragma pack(pop)

extern Game* g_game;

void FUN_0041c3c0(void);

// FUNCTION: 0x41d0f0
void FUN_0041d0f0()
{
    Rect_0041d0f0 r = g_game->view;
    int y = g_game->world_h * (r.y - g_game->origin_y) / g_game->screen_h - g_game->viewHeight / 2;
    int x = g_game->world_w * (r.x - g_game->origin_x) / g_game->screen_w - g_game->viewWidth / 2;
    g_game->x = x;
    g_game->y = y;
    g_game->flags_142f1 |= 2;
    FUN_0041c3c0();
    g_game->x2 = g_game->x;
    g_game->y2 = g_game->y;
    g_game->flags_14281 &= 0xfff7;
    g_game->value_1434b = 0;
    g_game->value_142f3 = 0;
    g_game->value_142f7 = 0;
}
