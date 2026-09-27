// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

void FUN_004d85a0(int* param_1);

extern int* DAT_0051fba0;
extern int DAT_0051fb9c;

// FUNCTION: 0x49f640
void FUN_0049f640()
{
    PlaySoundA(0, 0, 0x2000);
    if (DAT_0051fba0 != 0) {
        FUN_004d85a0(DAT_0051fba0);
        DAT_0051fba0 = 0;
    }
    DAT_0051fb9c = 0;
}
