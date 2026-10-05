// Decompiled by Opus. Names are provisional.
// Returns 1 when the local player has nothing left (the short at player
// +0x144 is zero; compare 0x48ffd0).

#pragma pack(push, 1)
struct Player_00490050 {                // 0x14b bytes
    char unknown_0[0x144];
    short count;                        // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00490050 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;               // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x490050
int FUN_00490050()
{
    return g_game->players[g_game->player].count == 0 ? 1 : 0;
}
