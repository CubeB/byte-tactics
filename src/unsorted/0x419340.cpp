// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Chat command: sets the local player's metal-sharing threshold to the
// minimum of field_a8 and the argument, then prints a confirmation.
//
// Not matched (85.7%). Every instruction matches except the scheduling of the
// one x87 store. Ours emits, at the ternary's join:
//     fild [esp+8] / fstp [esi+0xe4] / push 0 / push 1 / mov ecx,edi / call
// The original emits the store one slot later, between the two pushes:
//     fild [esp+8] / push 0 / push 1 / fstp [esi+0xe4] / mov ecx,edi / call
// Tried and unchanged: if/else with a store per arm (turns the field arm into
// an integer move), a float/int temporary for the ternary result, a reference
// or inline getter/setter for the field, comma expressions around the store
// and inside sprintf's argument, an explicit (float) cast, a double cap, an
// extra local, both declaration orders, polarity variants of the comparison,
// a manual sprintf declaration, ~120 header sets (tools/headers.py), and a
// preceding function in the same file. The sibling share-energy command
// (0x419400, same shape) is stuck at the same 85.7% for the same reason, and
// no other function in TotalA.exe contains this push/fstp pattern.
#include <stdio.h>

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
        float cap;
        p->share_metal = (cap = p->field_a8) < args->FUN_004b73e0(1, 0)
            ? p->field_a8 : args->FUN_004b73e0(1, 0);
        sprintf(buf, "OK.  Will share metal if above %d", args->FUN_004b73e0(1, 0));
        FUN_00463ca0(buf, 2, 0, 10);
    }
}
