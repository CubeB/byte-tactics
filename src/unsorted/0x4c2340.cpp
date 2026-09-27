// Decompiled by Haiku. Names are provisional.

#include <string.h>

extern int FUN_004b6220();

// FUNCTION: 0x4c2340
void __stdcall FUN_004c2340(int* param_1)
{
    int ptr = FUN_004b6220();
    memcpy(param_1, (void*)(ptr + 0x196), 6 * 4);
}
