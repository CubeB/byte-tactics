// Decompiled by Opus. Names are provisional.
// Resets the timer table (period = -1 disables an entry) and restarts the
// clock used by UpdateTimers.
#include <windows.h>

struct GameCtx_4b6510 {
    char unknown_0[0xe8];
    unsigned int rate;                 // +0xe8
};

struct Timer_4b6510 {
    int period;                        // +0x0 (-1 = unused)
    int counter;                       // +0x4
    int unknown_8;
    int unknown_c;
};

extern GameCtx_4b6510* g_display;
extern Timer_4b6510 DAT_0051fbe0[10];
extern int g_timerCount;
extern unsigned int DAT_0051fc84;

// FUNCTION: 0x4b6510
void ResetTimers()
{
    g_timerCount = 0;
    for (int i = 0; i < 10; i++) {
        DAT_0051fbe0[i].period = -1;
    }
    DAT_0051fc84 = GetTickCount() * g_display->rate / 1000;
}
