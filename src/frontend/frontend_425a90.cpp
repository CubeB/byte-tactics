// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Resets front-end state (state 2) and clears the pending state-change markers
// (+0x2bbf, +0x2bc0). Two debug checks (CodeChecksumFailed, compiled to return 0)
// would report a code segment checksum error with the line and file.
#include <stdio.h>

extern char* g_game;

int CodeChecksumFailed(void);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall LoadPictureCached(int param_1, int param_2, int param_3, int param_4);

// FUNCTION: 0x425a90
void EnterMainMenuState()
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                178, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
    g_game[0x2bbe] = 2;
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]",
                155, "c:\\cavedog\\wargame\\frontend.cpp");
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
    g_game[0x2bbf] = 0;
    g_game[0x2bc0] = 0;
    LoadPictureCached(0, 0, 0, 0);
}
