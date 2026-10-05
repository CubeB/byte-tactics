// Decompiled by Opus. Names are provisional.

// A timer: while stopped, `time` holds the elapsed time; while running, it
// holds the start time (see the neighbouring methods at 0x4e2160..0x4e21f0).
struct Timer {
    double time;           // +0x00
    char unknown_8[0x40];
    char stopped;          // +0x48

    void FUN_004e21a0(double delta);
};

// FUNCTION: 0x4e21a0
void Timer::FUN_004e21a0(double delta)
{
    if (stopped) {
        time -= delta;
    } else {
        // Without /Op the float conversion emits nothing, but it makes MSVC
        // load delta first (fld delta; fadd time) instead of hoisting a shared
        // `fld time` above the branch.
        time += (float)delta;
    }
}
