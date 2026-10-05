// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>

extern char* g_game;

int FUN_00428bc0(void);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x425860
void __stdcall FUN_00425860(char state, int line, char* file)
{
    char buf[256];
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", line, file);
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
    g_game[0x2bbe] = state;
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 155, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
    g_game[0x2bbf] = 0;
    g_game[0x2bc0] = 0;
}
