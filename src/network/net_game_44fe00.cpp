// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct PlayerStruct_44fe00 {
    char unknown_0[4];
    int field_4;                        // +0x4
    char unknown_8[0x73 - 8];
    char flag_73;                       // +0x73
    char unknown_74[0x14b - 0x73 - 1];
};

struct Game {
    char unknown_0[0x1b63];
    PlayerStruct_44fe00 players[10];
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// FUNCTION: 0x44fe00
int GetLocalHumanDpid()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].flag_73 == 1) {
            return g_game->players[i].field_4;
        }
    }
    return -1;
}
