// Decompiled by Opus. Names are provisional.
// Returns the lowest of ten slot numbers (0..9) not taken by any active
// player of type 1-3 (side 10 excluded), or 0 when all are taken.
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_00440c10 {
    char unknown_0[0x96];
    unsigned char slot;                // +0x96
};

struct Player_00440c10 {               // 0x14b bytes
    int active;                        // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00440c10* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char side;                // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00440c10 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x440c10
int FindUnusedLogo()
{
    int used[10];
    memset(used, 0, sizeof(used));
    for (int i = 0; i < 10; i++) {
        Player_00440c10* p = &g_game->players[i];
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->side != 10)
            used[p->info->slot < 9 ? p->info->slot : 9] = 1;
    }
    int result = 0;
    for (int j = 0; j < 10; j++) {
        if (!used[j]) {
            result = j;
            break;
        }
    }
    return result;
}
