// Decompiled by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by gpt-6-luna, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
// Finds the highest field_4 among the active players of type 1 or 3, looks
// that player up by field_4 and sets bit 0 of its info flags. Nothing in the
// exe calls it. It returns the constant 10, the "no player" index.

#pragma pack(push, 1)
struct Info_00450240 {
    char unknown_0[0x97];
    unsigned char flags;                // +0x97
};

struct Player_00450240 {
    int active;                         // +0x00
    unsigned int field_4;               // +0x04
    char unknown_8[0x27 - 0x8];
    Info_00450240* info;                // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                 // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00450240 players[10];        // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

static inline int GetPlayerField(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].field_4;
    return -1;
}

static inline unsigned char FindPlayerIndex(int id)
{
    // Early return, not a loop wrapped in if (id != -1).
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerField(i) == id)
            return i;
    }
    return 10;
}

static inline Player_00450240* FindPlayer(int id)
{
    if (FindPlayerIndex(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex(id)];
}

// FUNCTION: 0x450240
unsigned char PickNewHost()
{
    unsigned int max = 0;
    Player_00450240* p = g_game->players;
    int n = 10;
    do {
        if ((p->active != 0 && p->type == 3)
            || (p->active != 0 && p->type == 1)) {
            if (p->field_4 > max)
                max = p->field_4;
        }
        p++;
    } while (--n);

    Player_00450240* q = FindPlayer(max);
    if (q)
        q->info->flags |= 1;
    // Must return 10, not void.
    return 10;
}
