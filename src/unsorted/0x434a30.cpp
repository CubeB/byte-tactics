// Decompiled by Haiku. Names are provisional.
#include <stdlib.h>

extern char DAT_005122c0;
extern int DAT_005122c4;
extern int DAT_005122c8;
extern int DAT_005122cc;

extern void FUN_00434a60(void);

// FUNCTION: 0x434a30
void __fastcall FUN_00434a30(int param_1)
{
    int temp;
    unsigned char al;

    temp = param_1;
    al = *((unsigned char*)(&temp) + 3);
    DAT_005122c0 = al;
    DAT_005122c4 = 0;
    DAT_005122c8 = 0;
    DAT_005122cc = 0;
    atexit(FUN_00434a60);
}
