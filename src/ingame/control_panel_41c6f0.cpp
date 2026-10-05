// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdlib.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    int scroll_x;                    // +0x1431f
    int scroll_y;                    // +0x14323
    char unknown_14327[0x1432f - 0x14327];
    int value_1432f;                 // +0x1432f
    int value_14333;                 // +0x14333
    int sum_x;                       // +0x14337
    int sum_y;                       // +0x1433b
    char unknown_1433f[0x1434e - 0x1433f];
    unsigned char flags_1434e;       // +0x1434e
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// FUNCTION: 0x41c6f0
void FUN_0041c6f0()
{
    if ((g_game->flags_1434e & 1) != 0) {
        int count = g_game->value_14333;
        if (count > 0) {
            int dx = g_game->sum_x * count / g_game->value_1432f;
            int dy = g_game->sum_y * count / g_game->value_1432f;
            int rx = (int)(((__int64)rand() * dx) / 32768) - dx / 2;
            int ry = (int)(((__int64)rand() * dy) / 32768) - dy / 2;
            g_game->scroll_x += rx;
            g_game->scroll_y += ry;
            g_game->value_14333--;
        } else {
            g_game->flags_1434e &= 0xfe;
        }
    }
}
