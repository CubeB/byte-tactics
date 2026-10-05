// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// Full match. The only thing that ever differed was the base/index order of the
// inner load: the original has [edx+esi+0x108] (the loop counter in the base
// slot) and an earlier attempt produced [esi+edx+0x108]. In `p[a+b]` MSVC makes
// the variable declared first the addressing-mode index, so declaring the two
// Player pointers (mine, other) before the loop counters (i, j) puts `other` in
// the index slot of the inner load and leaves the two outer loads (which want
// the counter indexed) untouched. <windows.h> is still needed to keep those two
// the right way round.
#include <windows.h>                    // unused, but it picks the operand order

#pragma pack(push, 1)
struct PlayerInfo_00490080 {            // flags at +0x9b and +0x9d
    char unknown_0[0x9b];
    unsigned char flags_9b;              // +0x9b, bit 0x40 is tested
    unsigned char unknown_9c;
    unsigned char flags_9d;              // +0x9d, bit 2 is tested
};

struct Player_00490080 {                // 0x14b bytes, ten of them in the game
    int unknown_0;                       // +0x00, zero when the slot is unused
    char unknown_4[0x23];
    PlayerInfo_00490080* info;            // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;                 // +0x73, only 1, 2 and 3 are looked at
    char unknown_74[0x108 - 0x74];
    unsigned char unknown_108[11];       // +0x108, one entry per other team
    unsigned char unknown_113[10];       // +0x113, one entry per other team
    char unknown_11d[0x140 - 0x11d];
    int unknown_140;                     // +0x140
    short unknown_144;                   // +0x144
    unsigned char unknown_146;           // +0x146, 0xa skips the team
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00490080 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;                // +0x2a42
    char unknown_2a43[0x37ef6 - 0x2a43];
    int value_37ef6;                     // +0x37ef6
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x490080
int FUN_00490080()
{
    // declared before the loop counters on purpose: that is what puts `other`
    // in the index slot of the inner load (see the note at the top)
    Player_00490080* mine;
    Player_00490080* other;
    unsigned char i;
    int j;
    unsigned char state;

    if (g_game->value_37ef6 == 2) {
        return 0;
    }
    mine = &g_game->players[g_game->player];
    for (i = 0; i < 10; i++) {
        other = &g_game->players[i];
        if (i == g_game->player) {
            continue;
        }
        if (other->unknown_0 == 0) {
            continue;
        }
        // state is a local because the original tests it twice, the second
        // time still in al, without reloading it.
        state = other->state;
        if (state == 1 || state == 2 || state == 3) {
            if (other->unknown_146 == 0xa) {
                continue;
            }
            if (other->info->flags_9b & 0x40) {
                continue;
            }
            if (other->unknown_140 == 0) {
                return 0;
            }
            if (state == 1 || state == 2 || state == 3) {
                if (other->unknown_144 == 0) {
                    continue;
                }
                if (!(other->info->flags_9d & 2)) {
                    return 0;
                }
                if (!(mine->info->flags_9d & 2)) {
                    return 0;
                }
                if (mine->unknown_108[i] == 0) {
                    return 0;
                }
                if (mine->unknown_113[i] == 0) {
                    return 0;
                }
                for (j = 0; j < 10; j++) {
                    Player_00490080* o = &g_game->players[j];
                    if (o->unknown_0 != 0
                        && (o->state == 1 || o->state == 2 || o->state == 3)
                        && o->unknown_146 != 0xa
                        && (o->unknown_144 != 0 || o->unknown_140 == 0)) {
                        if (other->unknown_108[j] == 0) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}
