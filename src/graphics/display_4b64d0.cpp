// Decompiled by Opus. Names are provisional.
// Disables one entry of the timer table (period = -1, see 0x4b6510);
// returns 0 when the index is out of range.

struct Timer_4b64d0 {
    int period;                        // +0x0 (-1 = unused)
    int counter;                       // +0x4
    int unknown_8;
    int unknown_c;
};

extern Timer_4b64d0 DAT_0051fbe0[10];
extern int g_timerCount;

// FUNCTION: 0x4b64d0
int __stdcall RemoveTimer(int i)
{
    if (i >= g_timerCount)
        return 0;
    if (i < 0)
        return 0;
    DAT_0051fbe0[i].period = -1;
    return 1;
}
