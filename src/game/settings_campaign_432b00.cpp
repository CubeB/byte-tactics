// Decompiled by Opus. Names are provisional.
#include <string.h>

extern void __stdcall FUN_004b6a20(const char* app, const char* key, const char* value);

// FUNCTION: 0x432b00
void __stdcall SaveCampaignRegistryName(int core, char* name)
{
    char key[64];
    if (core)
        strcpy(key, "CoreCamp");
    else
        strcpy(key, "ArmCamp");
    name[0x19] = 0;
    FUN_004b6a20("Total Annihilation", key, name);
}
