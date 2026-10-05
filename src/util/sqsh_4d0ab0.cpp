// Decompiled by Opus. Names are provisional.
// Frees the window buffer allocated by 0x4d0a70.
#include <stdio.h>
#include <stdlib.h>

extern void* DAT_00526ff4;

// FUNCTION: 0x4d0ab0
int __cdecl LzssFreeWindow(void)
{
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
        return -1;
    }
    free(DAT_00526ff4);
    DAT_00526ff4 = 0;
    return 0;
}
