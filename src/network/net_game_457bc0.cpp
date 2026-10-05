// Decompiled by Opus. Names are provisional.
// Counts active players of type 2 whose field_146 is not 10 (compare
// 0x457c10).

#pragma pack(push, 1)
struct Player_00457bc0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x140 - 0x74];
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00457bc0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x457bc0
int FUN_00457bc0()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && g_game->players[i].type == 2
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0))
            count++;
    }
    return count;
}
