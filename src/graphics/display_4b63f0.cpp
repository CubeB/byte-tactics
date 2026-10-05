// Decompiled by Space Bunny Free. Names are provisional.
#include <windows.h>

struct GameCtx_4b63f0 {
    char unknown_0[0xe8];
    unsigned int rate;                 // +0xe8, ticks per second
};

struct Timer_4b63f0 {
    void* callback;                    // +0
    int id;                            // +4, the handle the callback removes
    int interval;                      // +8, -1 when the slot is free
    int countdown;                     // +0xc
};

extern GameCtx_4b63f0* g_display;
extern Timer_4b63f0 DAT_0051fbd8[];    // 10 slots, then g_timerCount counts them
extern int DAT_0051fbe0;               // the interval field of the first slot
extern int g_timerCount;
extern unsigned int DAT_0051fc84;      // last tick count, in game ticks

typedef void (__stdcall *TimerCb_4b63f0)(int);

// FUNCTION: 0x4b63f0
int __stdcall AddTimer(int interval, int id, TimerCb_4b63f0 callback)
{
    unsigned int now = (GetTickCount() * g_display->rate) / 1000;
    int diff = (int)now - DAT_0051fc84;
    DAT_0051fc84 = (GetTickCount() * g_display->rate) / 1000;

    int* p = &DAT_0051fbe0;
    do {
        if (*p >= 0) {
            if ((p[1] -= diff) <= 0) {
                TimerCb_4b63f0 fn = (TimerCb_4b63f0)p[-2];
                fn(p[-1]);
                p[1] = *p;             // reload the whole interval
            }
        }
        p += 4;
    } while ((int)p < (int)&g_timerCount);

    int i = 0;
    int* q = &DAT_0051fbe0;
    // The bound is the end of the ten slots. Written relative to the first one
    // so the entry test folds away, the way the original's code has none.
    for (; (int)q < (int)&DAT_0051fbe0 + 160; q += 4, i++) {
        if (*q < 0) {
            DAT_0051fbd8[i].callback = callback;
            DAT_0051fbd8[i].id = id;
            DAT_0051fbd8[i].interval = interval;
            DAT_0051fbd8[i].countdown = interval;
            g_timerCount++;
            return i;
        }
    }
    return -1;
}
