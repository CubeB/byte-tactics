// Decompiled by space-bunny-free. Names are provisional.
// Mission-script event dispatcher: the argument selects one of six cases out
// of 0x3d..0x70. The 0x3d walk keeps a byte counter in ebx next to a separate
// byte-offset induction variable in eax, and the redundant `i < 10` guard
// inside the do-while is what leaves the preheader test in the binary.

#include <windows.h>
#pragma pack(push, 1)

struct PlayerData_004956c0;

struct Player_004956c0 {
    PlayerData_004956c0* data;         // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char kind;               // +0x73
    char unknown_74[0x8c - 0x74];
    float energy;                     // +0x8c
    char unknown_90[0x98 - 0x90];
    float metal;                      // +0x98
    char unknown_9c[0xa4 - 0x9c];
    float energyCapacity;             // +0xa4
    float metalCapacity;              // +0xa8
    char unknown_ac[0x146 - 0xac];
    unsigned char field_146;          // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Unit_004956c0 {
    char unknown_0[0xa6];
    unsigned short field_a6;          // +0xa6
    char unknown_a8[0xf0 - 0xa8];
    int field_f0;                     // +0xf0
    unsigned char field_f4;           // +0xf4
    char unknown_f5[0x110 - 0xf5];
    unsigned int bits_110 : 14;       // +0x110
    unsigned int flag_110 : 1;
    char unknown_114[0x118 - 0x114];
};

struct Game_004956c0 {
    char unknown_0[0x1b63];
    Player_004956c0 players[10];       // +0x1b63
    char unknown_2851[0x2cba - 0x2851];
    unsigned short field_2cba;         // +0x2cba
    char unknown_2cbc[0x14280 - 0x2cbc];
    unsigned char counter_14280;      // +0x14280
    char unknown_14281[0x14357 - 0x14281];
    Unit_004956c0* units;             // +0x14357
    char unknown_1435b[0x3923b - 0x1435b];
    unsigned short bit0_3923b : 1;    // +0x3923b
    unsigned short rest_3923b : 15;
};
#pragma pack(pop)

extern Game_004956c0* g_game;

int __stdcall FUN_004c61b0(int enable);

// FUNCTION: 0x4956c0
void __stdcall FUN_004956c0(int eventType)
{
    switch (eventType) {
    case 0x69:
        g_game->bit0_3923b = !g_game->bit0_3923b;
        break;
    case 0x6d:
        g_game->counter_14280++;
        if (g_game->counter_14280 == 5)
            g_game->counter_14280 = 0;
        break;
    case 0x50:
        FUN_004c61b0(1);
        break;
    case 0x70:
        FUN_004c61b0(0);
        break;
    case 0x5d: {
        Unit_004956c0* u = &g_game->units[g_game->field_2cba];
        if (u->field_a6 != 0) {
            u->field_f4 = 10;
            u->flag_110 = 1;
            u->field_f0 = 0;
        }
        break;
    }
    case 0x3d: {
        int k = 0;
        unsigned char i = 0;
        do {
            if (i < 10) {
                if (g_game->players[k].data != 0) {
                    unsigned char kind = g_game->players[k].kind;
                    if (kind == 1 || kind == 2 || kind == 3) {
                        if (g_game->players[k].field_146 != 10) {
                            g_game->players[k].energy = g_game->players[k].energyCapacity;
                            g_game->players[k].metal = g_game->players[k].metalCapacity;
                        }
                    }
                }
            }
            i++;
            k++;
        } while (i < 10);
        break;
    }
    default:
        break;
    }
}
