// Decompiled by Opus. Names are provisional.
// Returns the first player slot that is inactive and not of type 4, or 10
// when there is none.

#pragma pack(push, 1)
struct Player_004504f0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004504f0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4504f0
int FUN_004504f0()
{
    int found = 0;
    int i;
    for (i = 0; i < 10; i++) {
        Player_004504f0* p = &g_game->players[i];
        if (p->active == 0 && p->type != 4) {
            found = 1;
            break;
        }
    }
    if (!found)
        i = 10;
    return i;
}
