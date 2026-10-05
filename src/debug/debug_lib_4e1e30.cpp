// Decompiled by Opus. Names are provisional.
// Timer: while stopped (flag at +0x48) the double at +0 holds the elapsed
// time; while running it holds the start time and the elapsed time is the
// current time (GetTimeSeconds) minus it. Callers set ecx to the timer
// (0x4e1eda, 0x4e2150), so this is a method.

extern double GetTimeSeconds();

class Timer {
public:
    double time;                       // +0x0
    char unknown_8[0x40];
    char stopped;                      // +0x48

    double GetElapsedSeconds();
};

// FUNCTION: 0x4e1e30
double Timer::GetElapsedSeconds()
{
    if (stopped) {
        return time;
    }
    return GetTimeSeconds() - time;
}
