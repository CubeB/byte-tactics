// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Struct_0049fb50 {
    char unknown_0[0xbb6];
    char path[0x100];                  // +0xbb6
};

// FUNCTION: 0x49fb50
void __stdcall FUN_0049fb50(Struct_0049fb50* obj, const char* dir)
{
    strncpy(obj->path, dir, 0x100);
    strcat(obj->path, "\\");
}
