// Decompiled by Opus. Names are provisional.
// (windows.h only for its effect on operand order; see tools/headers.py)
#include <windows.h>

#pragma pack(push, 1)
struct Game_0041c640 {
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
extern Game_0041c640* g_game;

// FUNCTION: 0x41c640
void __stdcall FUN_0041c640(int dx, int dy, int value)
{
    if ((g_game->flags_37f2f & 0x10) == 0) {
        if ((g_game->flags_1434e & 1) == 0) {
            g_game->sum_x = 0;
            g_game->sum_y = 0;
        }
        g_game->value_1432f = (g_game->value_1432f + value) / 2;
        g_game->value_14333 = g_game->value_1432f;
        g_game->sum_x += dx;
        g_game->sum_y += dy;
        if (g_game->value_1432f > 0) {
            g_game->flags_1434e |= 1;
        }
    }
}
