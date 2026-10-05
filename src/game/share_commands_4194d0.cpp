// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Chat command: toggles outgoing packet compression and prints the new state.
#include <stdio.h>

#pragma pack(push, 1)
struct Game_004194d0 {
    char unknown_0[0x4ed];
    int field_4ed;                     // +0x4ed
    char unknown_4f1[0x2a44 - 0x4f1];
    unsigned short flags_2a44;         // +0x2a44
};
#pragma pack(pop)

extern Game_004194d0* g_game;

void __stdcall FUN_00463ca0(char* param_1, int param_2, int param_3, int param_4);

// FUNCTION: 0x4194d0
void __stdcall FUN_004194d0(int unused)
{
    char buf[256];
    if (g_game->flags_2a44 & 1) {
        g_game->field_4ed = (g_game->field_4ed == 0);
        sprintf(buf, "Ok.  Outgoing packet compression turned %s",
                g_game->field_4ed ? "OFF" : "ON");
        FUN_00463ca0(buf, 2, 0, 10);
    }
}
