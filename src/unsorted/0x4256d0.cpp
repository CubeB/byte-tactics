// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <stdio.h>

extern char* g_game;

int FUN_00428bc0(void);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x4256d0
void __stdcall FUN_004256d0(int line, char* file)
{
    char buf[256];
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", line, file);
        FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
    }
}
