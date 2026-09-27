// Decompiled by Opus. Names are provisional.
// Milliseconds since boot scaled by the rate at +0xe8 of the object at
// DAT_0051fbd0, divided by 1000 (unsigned, so `mul` by the magic number).
#include <windows.h>

struct GlobalObj_004b6340 {
    char unknown_0[0xe8];
    int rate;                          // +0xe8
};

extern GlobalObj_004b6340* DAT_0051fbd0;

// FUNCTION: 0x4b6340
unsigned int FUN_004b6340()
{
    return GetTickCount() * DAT_0051fbd0->rate / 1000;
}
