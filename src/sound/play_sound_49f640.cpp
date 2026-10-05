// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

void __cdecl FUN_004d85a0(int* param_1);

extern int* g_diskWav;
extern int g_loopingWav;

// FUNCTION: 0x49f640
void StopWindowsSound()
{
    PlaySoundA(0, 0, 0x2000);
    if (g_diskWav != 0) {
        FUN_004d85a0(g_diskWav);
        g_diskWav = 0;
    }
    g_loopingWav = 0;
}
