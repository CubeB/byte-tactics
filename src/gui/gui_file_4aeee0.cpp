// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Object_004aeee0 {
    int unknown_0;
    void* gaf;                         // +0x4
    char unknown_8[0xab6 - 0x8];
    char dir[0x100];                   // +0xab6
};

void __stdcall ChangeExtension(char* out, char* in, const char* ext);
int __stdcall FUN_004bbc40(char* path);
void* __stdcall LoadGaf(char* path);

// FUNCTION: 0x4aeee0
void __stdcall LoadGafFile(Object_004aeee0* obj, char* name)
{
    char path[256];
    strncpy(path, obj->dir, 0x100);
    strcat(path, name);
    ChangeExtension(path, path, "GAF");
    if (FUN_004bbc40(path)) {
        obj->gaf = LoadGaf(path);
    }
}
