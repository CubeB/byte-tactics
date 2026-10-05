// Decompiled by Opus. Names are provisional.
// Counts the active players of type 2 (compare 0x457b40). The pointer local
// per iteration keeps the walk at the start of each entry; indexing twice
// makes MSVC walk the type field instead.

#pragma pack(push, 1)
struct Player_00457b90 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00457b90 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x457b90
int FUN_00457b90()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_00457b90* p = &g_game->players[i];
        if (p->active != 0 && p->type == 2)
            count++;
    }
    return count;
}
