// Decompiled by Opus. Names are provisional.
// Sets the game's flag at +0x299c when two active players (of type 1, 2 or
// 3, and not in state 10) share an id from 1 to 10 (see 0x450910, which
// finds the first unused id).
#include <string.h>

#pragma pack(push, 1)
struct Player_450980 {
    int active;                      // +0x00
    char unknown_4[8];
    int id;                          // +0x0c
    char unknown_10[0x73 - 0x10];
    char type;                       // +0x73
    char unknown_74[0x146 - 0x74];
    char f_146;                      // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_450980 {
    char unknown_0[0x1b63];
    Player_450980 players[10];       // +0x1b63
    char unknown_2851[0x299c - 0x2851];
    int duplicateIds;                // +0x299c
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game_450980* g_game;

// Indexing g_game->players[i] in every test (rather than a player pointer
// local) is what makes MSVC walk the array from the type field.
// FUNCTION: 0x450980
void FUN_00450980(void)
{
    int counts[11];
    memset(counts, 0, sizeof(counts));
    int dup = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].f_146 != 10
            && g_game->players[i].id > 0 && g_game->players[i].id <= 10) {
            if (++counts[g_game->players[i].id] > 1) {
                dup = 1;
                break;
            }
        }
    }
    g_game->duplicateIds = dup;
}
