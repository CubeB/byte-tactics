// Decompiled by DeepSeek V4.1 Flash, finished by Claude Sonnet 5.5. Names are provisional.
// Marks, for each active player (active 1 or 2), every player slot that shares
// its team type (or is the player itself), in the entry's `marks` array.
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

struct Game {
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

extern Game* g_game;

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
                int k;
                int ii;
                // Out-of-line found path via gotos: lays the found block after the ret.
                if (j != n) {
                    for (ii = j; ii < n; ii++) {
                        if (players[ii].type == players[i].type && players[ii].active != 0 && players[ii].type != 5)
                            goto found;
                        if (ii == i)
                            goto found;
                    }
                }
                k = -1;
                goto done;
            found:
                k = ii;
            done:
                if (k == -1)
                    break;
                g_game->entries[i].marks[k] = 1;
                j = k + 1;
            }
        }
    }
}
