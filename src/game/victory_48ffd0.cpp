// Decompiled by Opus. Names are provisional.
// Returns 1 when every other player is either allied with the local player
// or has nothing left (the short at player +0x144 is zero).

#pragma pack(push, 1)
struct Player_0048ffd0 {                // 0x14b bytes
    char unknown_0[0x108];
    unsigned char allied[10];           // +0x108
    char unknown_112[0x144 - 0x112];
    short count;                        // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game_0048ffd0 {
    char unknown_0[0x1b63];
    Player_0048ffd0 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;               // +0x2a42
};
#pragma pack(pop)

extern Game_0048ffd0* g_game;
// FUNCTION: 0x48ffd0
int FUN_0048ffd0()
{
    Player_0048ffd0* p = &g_game->players[g_game->player];
    for (unsigned char i = 0; i < 10; i++) {
        if (i != g_game->player && !p->allied[i] && g_game->players[i].count != 0)
            return 0;
    }
    return 1;
}
