// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14281];
    unsigned short flags_14281;      // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned char flags_142f1;       // +0x142f1
    char unknown_142f2;
    int value_142f3;                 // +0x142f3
    int value_142f7;                 // +0x142f7
    int xs[4];                       // +0x142fb
    int ys[4];                       // +0x1430b
    unsigned char valid[4];          // +0x1431b
    int x;                           // +0x1431f
    int y;                           // +0x14323
    int x2;                          // +0x14327
    int y2;                          // +0x1432b
    char unknown_1432f[0x1434b - 0x1432f];
    short value_1434b;               // +0x1434b
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

void ClampCameraPosition(void);

static inline void SetPos(int x, int y)
{
    g_game->x = x;
    g_game->y = y;
}

// FUNCTION: 0x41d3f0
void __stdcall RestoreCameraPosition(int index)
{
    if (g_game->valid != 0) {
        g_game->value_1434b = 0;
        g_game->value_142f3 = 0;
        g_game->value_142f7 = 0;
        SetPos(g_game->xs[index], g_game->ys[index]);
        g_game->flags_142f1 |= 2;
        ClampCameraPosition();
        g_game->x2 = g_game->x;
        g_game->y2 = g_game->y;
        g_game->flags_14281 &= 0xfff7;
    }
}
