// Decompiled by Haiku. Names are provisional.
#include <windows.h>

extern LPCSTR DAT_0051fb9c;

// FUNCTION: 0x49f620
void FUN_0049f620(void)
{
    if (DAT_0051fb9c != 0) {
        PlaySoundA(DAT_0051fb9c, 0, 0x15);
    }
}
