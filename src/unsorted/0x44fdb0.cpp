// Decompiled by Opus. Names are provisional.
// Returns field_4 of the first active player whose flag_73 is 1 or 2, or -1.

#pragma pack(push, 1)
struct PlayerStruct_44fdb0 {
    int field_0;                        // +0x0
    int field_4;                        // +0x4
    char unknown_8[0x73 - 8];
    char flag_73;                       // +0x73
    char unknown_74[0x14b - 0x73 - 1];
};

struct GameState_44fdb0 {
    char unknown_0[0x1b63];
    PlayerStruct_44fdb0 players[10];
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern GameState_44fdb0* g_game;

// FUNCTION: 0x44fdb0
int FUN_0044fdb0()
{
    int i = 0;
    while (!(g_game->players[i].field_0 != 0
             && (g_game->players[i].flag_73 == 1 || g_game->players[i].flag_73 == 2))) {
        if (++i >= 10)
            return -1;
    }
    return g_game->players[i].field_4;
}
