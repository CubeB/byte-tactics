// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_004469c0 {
    char unknown_0[0x73];
    unsigned char type;                // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char field_13f;           // +0x13f
    char unknown_140[0x14b - 0x140];
};

struct Game_004469c0 {
    char unknown_0[0x1b63];
    Player_004469c0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_004469c0* g_game;

// FUNCTION: 0x4469c0
int __stdcall FUN_004469c0(int player, int start)
{
    Player_004469c0* players = g_game->players;
    if (start == 10)
        return -1;
    for (int i = start; i < 10; i++) {
        if ((players[i].field_13f == players[player].field_13f && players[i].type != 0
             && players[i].field_13f != 5) || i == player)
            return i;
    }
    return -1;
}
