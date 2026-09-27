// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <string.h>

// Shows the contents of stats.txt in a message box.
// FUNCTION: 0x47c4d0
void FUN_0047c4d0()
{
    char text[0x400];
    memset(text, 0, sizeof(text));
    FILE* file = fopen("stats.txt", "r");
    if (file) {
        fread(text, sizeof(text), 1, file);
        MessageBoxA(NULL, text, "Summary", MB_TOPMOST);
    }
}
