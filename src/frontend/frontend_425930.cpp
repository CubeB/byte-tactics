// Decompiled by Opus. Names are provisional.
// Applies a pending front-end state change (+0x2bc0) to the current state
// (+0x2bbf). A debug check (FUN_00428bc0, compiled to return 0) would report
// a code segment checksum error, with the line and file of the change.
#include <stdio.h>

extern char* g_game;

int FUN_00428bc0(void);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x425930
void FUN_00425930()
{
    char buf[256];
    char next = g_game[0x2bc0];
    if (next != g_game[0x2bbf]) {
        if (FUN_00428bc0()) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                    163, "c:\\cavedog\\wargame\\frontend.cpp");
            OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
        }
        g_game[0x2bbf] = next;
        g_game[0x2bc0] = next;
    }
}
