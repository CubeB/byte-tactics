// Decompiled by Opus. Names are provisional.
// Chat command: sets the local player's energy-sharing threshold to the
// argument, capped at field_a4 (a min() macro, so the argument is read twice),
// and prints a confirmation.
#include <stdio.h>

#pragma pack(push, 1)
struct Player_00419400 {
    char unknown_0[0xa4];
    float field_a4;                    // +0xa4
    char unknown_a8[0xe8 - 0xa8];
    float share_energy;                // +0xe8
    char unknown_ec[0x14b - 0xec];
};

struct Game_00419400 {
    char unknown_0[0x1b63];
    Player_00419400 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    unsigned char flags;               // +0x2a44
};
#pragma pack(pop)

extern Game_00419400* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

void __stdcall FUN_00463ca0(char* param_1, int param_2, int param_3, int param_4);

// Not matched (85.7%). The `cap =` assignment reproduces the original's x87
// spill of field_a4 around the first call (a plain field is compared after
// the call instead). The one remaining difference: the original stores
// share_energy after the next call's `push 0; push 1`, ours before them.
// If/else, double, inline-helper and header variants did not move it; an
// if/else that duplicates the tail schedules the fld arm exactly like the
// original, so the original tail may have come from a cross-jumped copy.
// FUNCTION: 0x419400
void __stdcall FUN_00419400(Class_004b73e0* args)
{
    char buf[256];
    if (g_game->flags & 1) {
        Player_00419400* p = &g_game->players[g_game->local_player];
        float cap;
        p->share_energy = (cap = p->field_a4) < args->FUN_004b73e0(1, 0)
            ? p->field_a4 : args->FUN_004b73e0(1, 0);
        sprintf(buf, "OK.  Will share energy if above %d", args->FUN_004b73e0(1, 0));
        FUN_00463ca0(buf, 2, 0, 10);
    }
}
