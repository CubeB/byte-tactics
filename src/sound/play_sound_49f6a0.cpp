// Decompiled by Haiku. Names are provisional.
#include <windows.h>

extern char* g_loopingWav;

// FUNCTION: 0x49f6a0
void __stdcall PlayLoopingWavMemory(char* param_1)
{
    g_loopingWav = param_1;
    PlaySoundA(param_1, 0, 0xd);
}
