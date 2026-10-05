// Decompiled by Haiku. Names are provisional.
#include <windows.h>

extern LPCSTR g_loopingWav;

// FUNCTION: 0x49f620
void ResumeLoopingWav(void)
{
    if (g_loopingWav != 0) {
        PlaySoundA(g_loopingWav, 0, 0x15);
    }
}
