// Decompiled by Opus. Names are provisional.
// Finds the first player (of 10) with a type set and bit 0 of its info flags
// (index 10 if none), then returns 1 if that player slot is active with type
// 1 or 2. Types and layout as in 0x457af0.

#pragma pack(push, 1)
struct PlayerInfo_00457a50 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

struct Player_00457a50 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00457a50* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00457a50 {
    char unknown_0[0x1b63];
    Player_00457a50 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00457a50* g_game;

static inline unsigned char FindPlayer()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && (g_game->players[i].info->flags & 1))
            return i;
    }
    return 10;
}

static inline int IsHuman(Player_00457a50* p)
{
    if (p->active != 0 && (p->type == 1 || p->type == 2))
        return 1;
    return 0;
}

// FUNCTION: 0x457a50
int FUN_00457a50()
{
    return IsHuman(&g_game->players[FindPlayer()]);
}
