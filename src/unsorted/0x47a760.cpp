// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// PARTIAL 83.3%, 372 vs 368 bytes. Everything matches except the tail of the
// mark search. The original keeps the search result in eax (cmp eax,esi / je /
// cmp eax,esi / mov ecx,eax / jge at the top, then or eax,0xffffffff on the
// normal loop exit and mov eax,ecx on the found path) and lays the found path
// out of line at the function end. Ours coalesces the loop counter and result
// into ecx and adds a cmpres before the -1 store. Tried: separate k, k hoisted
// outside the restart loop, pointer loop, while/do-while shapes, writing to j
// directly. None beat this. See the pull request for the full list.
#include <windows.h>

#pragma pack(push, 1)
struct Player_0047a760 {
    int active;
    unsigned char shade;
    char unknown_5[3];
    int type;
    char unknown_c[0x14 - 0xc];
    unsigned char color;
    char unknown_15[3];
};

struct Unit_0047a760 {
    char unknown_0[0x95];
    unsigned char field_95;
    unsigned char slot;
};

struct Entry_0047a760 {
    char unknown_0[0x27];
    Unit_0047a760* unit;
    char unknown_2b[0x108 - 0x2b];
    unsigned char marks[0x14b - 0x108];
};

struct Game_0047a760 {
    char unknown_0[0x1b63];
    Entry_0047a760 entries[10];
    char unknown_2851[0x29a0 - 0x2851];
    Player_0047a760* players;
    char unknown_29a4[0x2a42 - 0x29a4];
    unsigned char localPlayer;
    unsigned char playerIndex;
    char unknown_2a44[0x38d81 - 0x2a44];
    int playerCount;
};
#pragma pack(pop)

extern Game_0047a760* g_game;

void __stdcall FUN_00464290(unsigned char player, unsigned char kind);

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
                Player_0047a760* players = g_game->players;
                int n = g_game->playerCount;
                int ii = j;
                if (j != n) {
                    for (; ii < n; ii++) {
                        if (players[ii].type == players[i].type && players[ii].active != 0 && players[ii].type != 5)
                            break;
                        if (ii == i)
                            break;
                    }
                }
                int k = (ii < n) ? ii : -1;
                if (k == -1)
                    break;
                g_game->entries[i].marks[k] = 1;
                j = k + 1;
            }
        }
    }
}
