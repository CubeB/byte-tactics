// Decompiled by Opus. Names are provisional.
// Returns 1 when every active player (other than type 5) has the same type
// and at least one such player exists, else 0.

struct Player_00479760 {
    int active;                        // +0x0
    char unknown_4[4];
    int type;                          // +0x8
    char unknown_c[0xc];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x29a0];
    Player_00479760* players;          // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int playerCount;                   // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x479760
int FUN_00479760(void)
{
    int type;
    int i;
    for (i = 0; i < g_game->playerCount; i++) {
        if (g_game->players[i].active != 0 && (type = g_game->players[i].type) != 5)
            break;
    }
    if (i == g_game->playerCount)
        return 0;
    for (i = 0; i < g_game->playerCount; i++) {
        if (g_game->players[i].type != type && g_game->players[i].active != 0)
            return 0;
    }
    return 1;
}
