// Decompiled by Opus. Names are provisional.
#include <stdio.h>

void __stdcall WriteRegistryDword(char* key, char* name, int value);

// FUNCTION: 0x42f910
void __stdcall SaveTrackSettings(unsigned char* tracks)
{
    char name[12];
    int i;

    for (i = 0; i < 10; i++) {
        sprintf(name, "track%d", i);
        WriteRegistryDword("Total Annihilation", name, tracks[i]);
    }
}
