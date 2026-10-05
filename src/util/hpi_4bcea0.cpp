// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Struct_004bcea0 {
    char unknown_0[0x728];
    char name[0x100];                  // +0x728
};

Struct_004bcea0* FUN_004b6220(void);

// FUNCTION: 0x4bcea0
void __stdcall FUN_004bcea0(char* dst)
{
    Struct_004bcea0* s = FUN_004b6220();
    strncpy(dst, s->name, 0x100);
}
