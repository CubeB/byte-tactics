// Decompiled by Opus. Names are provisional.
// Timer: while stopped (flag at +0x48) the double at +0 holds the elapsed
// time; while running it holds the start time and the elapsed time is the
// current time (FUN_004e1730) minus it. Callers set ecx to the timer
// (0x4e1eda, 0x4e2150), so this is a method.

extern double FUN_004e1730();

class Class_004e1e30 {
public:
    double time;                       // +0x0
    char unknown_8[0x40];
    char stopped;                      // +0x48

    double FUN_004e1e30();
};

// FUNCTION: 0x4e1e30
double Class_004e1e30::FUN_004e1e30()
{
    if (stopped) {
        return time;
    }
    return FUN_004e1730() - time;
}
