// Decompiled by Opus. Names are provisional.
#include <string.h>

extern void __stdcall WriteRegistryString(const char* app, const char* key, const char* value);

// FUNCTION: 0x432b00
void __stdcall SaveCampaignRegistryName(int core, char* name)
{
    char key[64];
    if (core)
        strcpy(key, "CoreCamp");
    else
        strcpy(key, "ArmCamp");
    name[0x19] = 0;
    WriteRegistryString("Total Annihilation", key, name);
}
