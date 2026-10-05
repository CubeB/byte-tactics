// Decompiled by Opus. Names are provisional.
// Searches the player table from index `start` for the first active player
// on the same side as `self` (type 5 excluded), stopping at `self` itself.
// Returns that index, or -1 when the table ends first.

struct Player_0047a700 {
    int active;                        // +0x0
    char unknown_4[4];
    int type;                          // +0x8
    char unknown_c[0xc];
};

#pragma pack(push, 1)
struct Game_0047a700 {
    char unknown_0[0x29a0];
    Player_0047a700* players;          // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int playerCount;                   // +0x38d81
};
#pragma pack(pop)

extern Game_0047a700* g_game;

// FUNCTION: 0x47a700
int __stdcall FUN_0047a700(int self, int start)
{
    Player_0047a700* players = g_game->players;
    int n = g_game->playerCount;
    if (start != n) {
        for (int i = start; i < n; i++) {
            if (players[i].type == players[self].type && players[i].active != 0 && players[i].type != 5)
                return i;
            if (i == self)
                return i;
        }
    }
    return -1;
}
