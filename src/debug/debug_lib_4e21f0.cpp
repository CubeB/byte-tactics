// Decompiled by Opus and Haiku. Names are provisional.
// 0x4e21f0 stays out of the gathered file: its 0.0 and 5.0 constants sit in a
// different constant pool from the memory status dialog's 0.0, so the original
// had it in another translation unit (tools/place.py sees one shared copy
// otherwise).
#include <windows.h>

double __cdecl GetTimeSeconds();

// While stopped, `time` holds the elapsed time; while running, it holds the
// start time.
class Timer {
public:
    double time;                       // +0x00
    char unknown_8[0x18 - 0x8];
    double history[5];                 // +0x18, the last five sample times
    char unknown_40[0x44 - 0x40];
    char* name;                        // +0x44
    char stopped;                      // +0x48
    char boosted;                      // +0x49
    char unknown_4a[2];
    DWORD oldPriorityClass;            // +0x4c
    int oldThreadPriority;             // +0x50

    double ComputeSampleRate();
};

// FUNCTION: 0x4e21f0
double Timer::ComputeSampleRate()
{
    double now = GetTimeSeconds();
    double rate;
    if (now - history[0] > 0.0)
        rate = 5.0 / (now - history[0]);
    else
        rate = 0.0;
    for (int i = 0; i < 4; i++)
        history[i] = history[i + 1];
    history[4] = now;
    return rate;
}
