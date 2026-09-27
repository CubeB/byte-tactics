// Decompiled by Opus. Names are provisional.
#include <string.h>

extern char* g_game;

int* __stdcall FUN_00429330(const char* name);
void FUN_004d85a0(int* param_1);

// FUNCTION: 0x42a400
int FUN_0042a400()
{
    int* palette = FUN_00429330("PALETTE");
    memcpy(g_game + 0x143a7, palette, 0x400);
    FUN_004d85a0(palette);
    return 1;
}
