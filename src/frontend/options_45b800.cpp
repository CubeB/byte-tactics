// Decompiled by Haiku. Names are provisional.
#include <string.h>

extern char DAT_00512d90[];

// FUNCTION: 0x45b800
void __stdcall FUN_0045b800(char* param_1)
{
    DAT_00512d90[0] = 0;
    strncat(DAT_00512d90, param_1, 0x3f);
}
