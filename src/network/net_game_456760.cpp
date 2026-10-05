// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Returns 0 when no player is in state 3. Otherwise every active player of
// type 1 or 2, or in state 3, must have bit 0x20 set in its data flags
// (+0x9b), else it returns 0. Returns 1 when some slot is inactive or lacks
// bit 0x40, unless field_2a3c is 1 (then 0).

#pragma pack(push, 1)
struct PlayerInfo_00456760 {
    char unknown_0[0x9b];
    unsigned char flags;               // +0x9b
};

struct Player_00456760 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00456760* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00456760 {
    char unknown_0[0x1b63];
    Player_00456760 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    short field_2a3c;                  // +0x2a3c
};
#pragma pack(pop)

extern Game_00456760* g_game;

static inline int IsPlaying(unsigned char i)
{
    return g_game->players[i].active != 0 && g_game->players[i].type == 3;
}

// The mix of `p->` and `g_game->players[j].` in the second loop is needed:
// it keeps the later reads of active and data from reusing the first loads,
// which frees dl for j and bl for the 0x40 mask as in the original.
// FUNCTION: 0x456760
int FUN_00456760()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_00456760* p = &g_game->players[i];
        if (p->active != 0 && p->type == 3)
            count++;
    }
    if (count == 0)
        return 0;

    int all = 1;
    for (unsigned char j = 0; j < 10; j++) {
        Player_00456760* p = &g_game->players[j];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || IsPlaying(j))) {
            if (!(g_game->players[j].data->flags & 0x20))
                return 0;
        }
        if (g_game->players[j].active == 0 || !(g_game->players[j].data->flags & 0x40))
            all = 0;
    }
    if (g_game->field_2a3c == 1)
        return 0;
    return all == 0;
}
