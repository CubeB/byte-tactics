// Decompiled by Opus. Names are provisional.
#include <stdio.h>

void __stdcall FUN_004b6a50(char* key, char* name, int value);

// FUNCTION: 0x42f910
void __stdcall FUN_0042f910(unsigned char* tracks)
{
    char name[12];
    int i;

    for (i = 0; i < 10; i++) {
        sprintf(name, "track%d", i);
        FUN_004b6a50("Total Annihilation", name, tracks[i]);
    }
}
