// Decompiled by Haiku. Names are provisional.
#include <string.h>

extern void* FUN_004b6220();

// FUNCTION: 0x4bcec0
void __stdcall FUN_004bcec0(char* param)
{
    void* p = FUN_004b6220();
    char* src = (char*)p + 0x628;
    strncpy(param, src, 0x100);
}
