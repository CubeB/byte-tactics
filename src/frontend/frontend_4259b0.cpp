// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>

extern char* g_game;
extern char DAT_00511fb8[];

int FUN_00428bc0(void);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x4259b0
void FUN_004259b0()
{
    char buf[256];
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0xa9, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
    g_game[0x2bbe] = 0;
    if (FUN_00428bc0()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x9b, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
    g_game[0x2bbf] = 0;
    g_game[0x2bc0] = 0;
    g_game[0x2bf0] = 0;
    *(unsigned short*)(g_game + 0x2aaf) &= 0xfffe;
    DAT_00511fb8[0] = 0;
}
