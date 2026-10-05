// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

extern int __stdcall ReadRegistryData(const char* app, const char* key, char* buf, int* size);

// FUNCTION: 0x432a80
void __stdcall LoadCampaignRegistryName(int core, char* name)
{
    char key[64];
    if (core)
        strcpy(key, "CoreCamp");
    else
        strcpy(key, "ArmCamp");
    core = 26; // the original reuses the dead first argument slot for the size
    if (ReadRegistryData("Total Annihilation", key, name, &core) == 0) {
        memset(name, 0x55, 0x19);
        name[0x19] = 0;
    }
}
