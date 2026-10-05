// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1432f];
    int value_1432f;                 // +0x1432f
    int value_14333;                 // +0x14333
    int sum_x;                       // +0x14337
    int sum_y;                       // +0x1433b
    char unknown_1433f[0x1434e - 0x1433f];
    unsigned char flags_1434e;       // +0x1434e
    char unknown_1434f[0x37f2f - 0x1434f];
    unsigned char flags_37f2f;       // +0x37f2f
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// FUNCTION: 0x41c5e0
void __stdcall StartScreenShake(int dx, int dy, int value)
{
    if ((g_game->flags_37f2f & 0x10) == 0) {
        g_game->value_1432f = value;
        g_game->value_14333 = value;
        if (dx != 0) {
            g_game->sum_x = dx;
        }
        if (dy != 0) {
            g_game->sum_y = dy;
        }
        if (value > 0) {
            g_game->flags_1434e |= 1;
        }
    }
}
