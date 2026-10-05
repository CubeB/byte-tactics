// Decompiled by Opus. Names are provisional.
// Resets the timer table (period = -1 disables an entry) and restarts the
// clock used by FUN_004b6370.
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

extern GameCtx_4b6510* DAT_0051fbd0;
extern Timer_4b6510 DAT_0051fbe0[10];
extern int DAT_0051fc80;
extern unsigned int DAT_0051fc84;

// FUNCTION: 0x4b6510
void FUN_004b6510()
{
    DAT_0051fc80 = 0;
    for (int i = 0; i < 10; i++) {
        DAT_0051fbe0[i].period = -1;
    }
    DAT_0051fc84 = GetTickCount() * DAT_0051fbd0->rate / 1000;
}
