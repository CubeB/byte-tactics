// Decompiled by Sonnet. Names are provisional.
#include <stdlib.h>

extern void* DAT_00526ff0;

// FUNCTION: 0x4d0a90
int FUN_004d0a90()
{
    void* p = calloc(0x1001, 6);
    DAT_00526ff0 = p;
    return p ? 0 : -1;
}
