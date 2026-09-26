// Decompiled by Opus. Names are provisional.
#include <windows.h>

struct FrameCounter_4b66a0 {
    int accum;                       // +0x0  ms accumulated since the last sample
    unsigned int lastTick;           // +0x4
    int frames;                      // +0x8  frames since the last sample
    int rate;                        // +0xc  frames counted in the last second

    void Tick()
    {
        unsigned int now = GetTickCount();
        accum += now - lastTick;
        lastTick = now;
        frames++;
        if (accum > 2000) {
            accum = 1000;
        }
        if (accum > 1000) {
            rate = frames;
            accum -= 1000;
            frames = 0;
        }
    }
};

#pragma pack(push, 2)
struct GameCtx_4b66a0 {
    char unknown_0[0x1da];
    FrameCounter_4b66a0 counter;     // +0x1da
};
#pragma pack(pop)

extern GameCtx_4b66a0* DAT_0051fbd0;

// FUNCTION: 0x4b66a0
int FUN_004b66a0()
{
    DAT_0051fbd0->counter.Tick();
    return DAT_0051fbd0->counter.rate;
}
