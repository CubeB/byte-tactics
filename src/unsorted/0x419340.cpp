// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Chat command: sets the local player's metal-sharing threshold to the
// argument, capped at field_a8, and prints a confirmation.
//
// The cap is a min() macro (stdlib.h __min here; any macro with the usual
// parentheses does the same) with an explicit (float) cast on the argument.
// The macro's parentheses make MSVC load and spill field_a8 before the
// first call, and the cast is what moves the store of the result after the
// next call's `push 0; push 1`. Without the cast the store comes before the
// pushes (85.7%).
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Player_00419340 {
    char unknown_0[0xa8];
    float field_a8;                    // +0xa8
    char unknown_ac[0xe4 - 0xac];
    float share_metal;                 // +0xe4
    char unknown_e8[0x14b - 0xe8];
};

struct Game_00419340 {
    char unknown_0[0x1b63];
    Player_00419340 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    unsigned char flags;               // +0x2a44
};
#pragma pack(pop)

extern Game_00419340* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

void __stdcall FUN_00463ca0(char* param_1, int param_2, int param_3, int param_4);

// FUNCTION: 0x419340
void __stdcall FUN_00419340(Class_004b73e0* args)
{
    char buf[256];
    if (g_game->flags & 1) {
        Player_00419340* p = &g_game->players[g_game->local_player];
        p->share_metal = __min(p->field_a8, (float)args->FUN_004b73e0(1, 0));
        sprintf(buf, "OK.  Will share metal if above %d", args->FUN_004b73e0(1, 0));
        FUN_00463ca0(buf, 2, 0, 10);
    }
}
