// Decompiled by Opus. Names are provisional.

extern double GetTimeSeconds();

// A timer: while stopped, `time` holds the elapsed time; while running, it
// holds the start time (see the neighbouring methods at 0x4e2160..0x4e21f0).
// This sets the elapsed time.
struct Class_004e21c0 {
    double time;           // +0x00
    char unknown_8[0x40];
    char stopped;          // +0x48

    void FUN_004e21c0(double elapsed);
};

// FUNCTION: 0x4e21c0
void Class_004e21c0::FUN_004e21c0(double elapsed)
{
    if (stopped) {
        time = elapsed;
    } else {
        time = GetTimeSeconds() - elapsed;
    }
}
