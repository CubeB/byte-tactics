// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Finds the player whose field_4 equals the given id and returns a pointer to
// it, or null when there is none. The index search was an inlined helper and
// appears twice in the original; the getter it uses treats the not-found
// sentinel 10 as an out-of-range index, which is why the loop keeps a
// redundant "i != 10" test.

#pragma pack(push, 1)
struct Player_0044fed0 {
    int field_0;                        // +0x0
    int field_4;                        // +0x4
    char unknown_8[0x73 - 8];
    char flag_73;                       // +0x73
    char unknown_74[0x14b - 0x73 - 1];
};

struct Game_0044fed0 {
    char unknown_0[0x1b63];
    Player_0044fed0 players[10];        // +0x1b63
};
#pragma pack(pop)

extern Game_0044fed0* g_game;

static inline int GetPlayerField_0044fed0(unsigned char i)
{
    if (i != 10 && g_game->players[i].flag_73)
        return g_game->players[i].field_4;
    return -1;
}

static inline unsigned char FindPlayerIndex(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerField_0044fed0(i) == id)
                return i;
        }
    }
    return 10;
}

// FUNCTION: 0x44fed0
Player_0044fed0* __stdcall FUN_0044fed0(int id)
{
    if (FindPlayerIndex(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex(id)];
}
