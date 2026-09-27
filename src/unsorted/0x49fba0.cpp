// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Struct_0049fba0 {
    char unknown_0[0x9b6];
    char path[0x100];                  // +0x9b6
};

// FUNCTION: 0x49fba0
void __stdcall FUN_0049fba0(Struct_0049fba0* obj, const char* dir)
{
    strncpy(obj->path, dir, 0x100);
    strcat(obj->path, "\\");
}
