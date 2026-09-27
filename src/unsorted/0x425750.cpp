// Decompiled by Opus. Names are provisional.
#include <string.h>

extern char DAT_00511fb8[];
extern char* g_game;

int __stdcall FUN_004a5030(char* text);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x425750
void FUN_00425750()
{
    if (strlen(DAT_00511fb8) != 0) {
        FUN_004abd90(g_game + 0x519, DAT_00511fb8, FUN_004a5030(DAT_00511fb8) + 0x14, 1, 1);
        DAT_00511fb8[0] = 0;
    }
}
