// Decompiled by Opus. Names are provisional.
// Counts the active players of type 3 (compare 0x457b90, type 2).

#pragma pack(push, 1)
struct Player_00457c80 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00457c80 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x457c80
int CountRemotePlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_00457c80* p = &g_game->players[i];
        if (p->active != 0 && p->type == 3)
            count++;
    }
    return count;
}
