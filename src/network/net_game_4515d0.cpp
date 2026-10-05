// Decompiled by Opus. Names are provisional.
// Looks for the player slot holding `id` (the search result is unused) and
// then hands the id to FUN_00450a10. The four other arguments are unused.
//
// The helper's own "index 10 means no player" test survives as the
// "cmp bl, 0xa; je" before the loop; a plain i < 10 loop folds it away.

#pragma pack(push, 1)
struct Player_004515d0 {
    int field_0;                        // +0x0
    int field_4;                        // +0x4
    char unknown_8[0x73 - 8];
    char flag_73;                       // +0x73
    char unknown_74[0x14b - 0x73 - 1];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004515d0 players[10];        // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00450a10(int id);

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].flag_73)
        return g_game->players[i].field_4;
    return -1;
}

// FUNCTION: 0x4515d0
void __stdcall FUN_004515d0(int id, int unused1, int unused2, int unused3, int unused4)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (PlayerId(i) == id)
                break;
        }
    }
    FUN_00450a10(id);
}
