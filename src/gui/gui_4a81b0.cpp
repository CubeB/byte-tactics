// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Struct_004a81b0 {
    char unknown_0[0xab6];
    char name[0x100];                  // +0xab6
};

// FUNCTION: 0x4a81b0
void __stdcall FUN_004a81b0(Struct_004a81b0* obj, char* out)
{
    *out = 0;
    if (obj->name[0] != 0) {
        strncpy(out, obj->name, 0x100);
    }
}
