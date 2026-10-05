// Decompiled by Opus. Names are provisional.
// Returns the index of the player whose id is the given one, or 10 when the
// id is -1 or no active player has it. The id lookup is an inlined helper
// that treats index 10 as "no player".

#pragma pack(push, 1)
struct Player_0044fe40 {
    char unknown_0[4];
    int id;                            // +0x4
    char unknown_8[0x73 - 8];
    char active;                       // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0044fe40 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

static inline int PlayerId(unsigned char i)
{
    if (i == 10 || !g_game->players[i].active)
        return -1;
    return g_game->players[i].id;
}

// FUNCTION: 0x44fe40
unsigned char __stdcall FUN_0044fe40(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (PlayerId(i) == id)
                return i;
        }
    }
    return 10;
}
