// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial (71.6%): the whole function is reproduced, including the inlined
// player search (an inlined copy of FUN_0047a700, as in 0x450380), but MSVC
// assigns the two outer induction variables the other way round: it keeps
// i*0x18 in ebp and i*0x14b in esi, while the original keeps i*0x18 in esi and
// i*0x14b in ebp. Every other byte, including the inlined search's register
// use, matches. <windows.h> is needed for the [edi+esi] addressing order (see
// 0x465e30 and 0x497080 for the same compiler-state trick).
#include <windows.h>

#pragma pack(push, 1)
struct Unit_0047a760 {
    char unknown_0[0x95];
    unsigned char field_95;            // +0x95
    unsigned char slot;                // +0x96
};

struct Player_0047a760 {               // 0x18 bytes
    int active;                        // +0x00
    unsigned char shade;               // +0x04
    char unknown_5[3];
    int type;                          // +0x08
    char unknown_c[0x14 - 0xc];
    unsigned char color;               // +0x14
    char unknown_15[3];
};

struct Entry_0047a760 {                // 0x14b bytes
    char unknown_0[0x27];
    Unit_0047a760* unit;               // +0x27
    char unknown_2b[0x108 - 0x2b];
    unsigned char marks[0x14b - 0x108];// +0x108
};

struct Game_0047a760 {
    char unknown_0[0x1b63];
    Entry_0047a760 entries[10];        // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Player_0047a760* players;          // +0x29a0
    char unknown_29a4[0x2a42 - 0x29a4];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x38d81 - 0x2a44];
    int playerCount;                   // +0x38d81
};
#pragma pack(pop)

extern Game_0047a760* g_game;

void __stdcall FUN_00464290(unsigned char player, unsigned char kind);

// Inlined copy of FUN_0047a700: first slot from `start` on the same side as
// `self` that is active and not type 5, or `self` itself, or -1.
static __inline int FindNext_0047a760(int self, int start)
{
    Player_0047a760* players = g_game->players;
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

// FUNCTION: 0x47a760
void FUN_0047a760()
{
    for (int i = 0; i < g_game->playerCount; i++) {
        if (g_game->players[i].active == 1) {
            g_game->entries[i].unit->slot = g_game->players[i].color;
            g_game->entries[i].unit->field_95 = g_game->players[i].shade;
            FUN_00464290(i, 1);
            g_game->playerIndex = i;
            g_game->localPlayer = i;
        } else if (g_game->players[i].active == 2) {
            g_game->entries[i].unit->slot = g_game->players[i].color;
            g_game->entries[i].unit->field_95 = g_game->players[i].shade;
            FUN_00464290(i, 2);
        } else {
            FUN_00464290(i, 0);
        }
        if (g_game->players[i].active == 1 || g_game->players[i].active == 2) {
            int j = 0;
            for (;;) {
                int k = FindNext_0047a760(i, j);
                if (k == -1)
                    break;
                g_game->entries[i].marks[k] = 1;
                j = k + 1;
            }
        }
    }
}
