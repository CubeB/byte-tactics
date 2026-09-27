// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_00457c10 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x140 - 0x74];
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00457c10 {
    char unknown_0[0x1b63];
    Player_00457c10 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00457c10* g_game;

// FUNCTION: 0x457c10
int FUN_00457c10()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0))
            count++;
    }
    return count;
}
