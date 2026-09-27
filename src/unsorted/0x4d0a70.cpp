// Decompiled by Haiku. Names are provisional.
#include <stdlib.h>

extern void* DAT_00526ff4;

// FUNCTION: 0x4d0a70
int __cdecl FUN_004d0a70(void)
{
    void* eax = calloc(1, 0x1011);
    DAT_00526ff4 = eax;
    if (eax != 0) return 0;
    return -1;
}
