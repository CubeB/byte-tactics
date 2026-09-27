// Decompiled by Haiku. Names are provisional.
#include <windows.h>

extern void* DAT_0051fbd0;

// FUNCTION: 0x4b6340
unsigned int __cdecl FUN_004b6340()
{
    unsigned int result = GetTickCount();
    void* ptr = DAT_0051fbd0;
    unsigned int val = *(int*)((unsigned char*)ptr + 0xe8);
    result = result * val;
    result = result / 1000;
    return result;
}
