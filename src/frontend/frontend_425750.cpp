// Decompiled by Opus. Names are provisional.
#include <string.h>

extern char DAT_00511fb8[];
extern char* g_game;

int __stdcall GetTextPixelWidth(char* text);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x425750
void FUN_00425750()
{
    if (strlen(DAT_00511fb8) != 0) {
        OpenMessageBox(g_game + 0x519, DAT_00511fb8, GetTextPixelWidth(DAT_00511fb8) + 0x14, 1, 1);
        DAT_00511fb8[0] = 0;
    }
}
