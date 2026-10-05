// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <stdio.h>

extern char* g_game;

int CodeChecksumFailed(void);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x4256d0
void __stdcall CheckFrontendStateChange(int line, char* file)
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", line, file);
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
}
