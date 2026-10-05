// Decompiled by Opus, finished with the fix Claude Opus 5.5 found for 0x419340. Names are provisional.
// Chat command: sets the local player's energy-sharing threshold to the
// argument, capped at field_a4 (a min() macro, so the argument is read twice),
// and prints a confirmation.
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Player_00419400 {
    char unknown_0[0xa4];
    float field_a4;                    // +0xa4
    char unknown_a8[0xe8 - 0xa8];
    float share_energy;                // +0xe8
    char unknown_ec[0x14b - 0xec];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00419400 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    unsigned char flags;               // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);

// The min() macro's parentheses load field_a4 before the call and spill it,
// and the explicit (float) cast on the argument makes the store come after
// the next call's pushes, as in the original (see 0x419340, #106).
// FUNCTION: 0x419400
void __stdcall FUN_00419400(Class_004b73e0* args)
{
    char buf[256];
    if (g_game->flags & 1) {
        Player_00419400* p = &g_game->players[g_game->local_player];
        p->share_energy = __min(p->field_a4, (float)args->FUN_004b73e0(1, 0));
        sprintf(buf, "OK.  Will share energy if above %d", args->FUN_004b73e0(1, 0));
        AddMessage(buf, 2, 0, 10);
    }
}
