// Decompiled by Haiku. Names are provisional.
#include <windows.h>

extern char* DAT_0051fb9c;

// FUNCTION: 0x49f6a0
void __stdcall FUN_0049f6a0(char* param_1)
{
    DAT_0051fb9c = param_1;
    PlaySoundA(param_1, 0, 0xd);
}
