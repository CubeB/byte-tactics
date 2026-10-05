// Decompiled by Haiku. Names are provisional.

#include <string.h>

void* GetDisplay();

// FUNCTION: 0x4c2360
void __stdcall FUN_004c2360(int* param_1)
{
    int eax = (int)GetDisplay();
    int edi = eax + 0x196;
    memcpy((void*)edi, param_1, 24);
}
