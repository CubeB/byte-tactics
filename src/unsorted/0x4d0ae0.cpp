// Decompiled by Sonnet. Names are provisional.
#include <stdio.h>
#include <stdlib.h>

struct Struct_00526ff0;
extern Struct_00526ff0* DAT_00526ff0;

// FUNCTION: 0x4d0ae0
int FUN_004d0ae0()
{
    if (DAT_00526ff0 == 0) {
        printf("Hey!  The tree ptr is not pointing to anything!\n");
        return -1;
    }
    free(DAT_00526ff0);
    DAT_00526ff0 = 0;
    return 0;
}
