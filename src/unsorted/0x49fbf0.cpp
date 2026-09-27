// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Struct_0049fbf0 {
    char unknown_0[0xab6];
    char path[0x100];                  // +0xab6
};

// FUNCTION: 0x49fbf0
void __stdcall FUN_0049fbf0(Struct_0049fbf0* obj, const char* dir)
{
    strncpy(obj->path, dir, 0x100);
    strcat(obj->path, "\\");
}
