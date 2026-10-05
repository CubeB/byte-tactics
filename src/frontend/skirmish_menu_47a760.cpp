// Decompiled by DeepSeek V4.1 Flash, finished by Claude Sonnet 5.5. Names are provisional.
// MATCH (372 of 372 bytes). Marks, for each active player (active 1 or 2), every
// player slot that shares its team type (or is the player itself), in the entry's
// `marks` array.
//
// What decided it (#743, was 83.3% and 368 bytes): the search for the next mark
// is written with an out-of-line found path,
//     for (ii = j; ii < n; ii++) { if (match) goto found; if (ii == i) goto found; }
//     k = -1; goto done;
//   found: k = ii;
//   done:
// which is what the original has: the normal loop exit falls into `or eax, -1`,
// and the found path (`mov ebp, [esp+0x14]; mov eax, ecx; jmp`) is laid out after
// the function's `ret`. The compiler knows `ii >= n` on the normal exit, so it
// drops the `cmp ecx, esi; jge` the old `k = ii < n ? ii : -1` form needed. The same
// control flow through an inline helper with early `return ii` gives the right
// search but the two per-player offsets (players at 0x18, entries at 0x14b) swap
// registers (71.6%), and `k = ii; break;` on the found paths is 408 bytes. The
// declaration-count sweep (0 to 400) and all 128 header sets are flat at the old
// 83.3%, so compiler state is not involved. The goto is a reproduction device: the
// original was probably an inlined find helper, but no helper form matched.
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
                int k;
                int ii;
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
