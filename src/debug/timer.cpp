// Decompiled by Opus and Haiku. Names are provisional.

#include <windows.h>

class Class_004e1d60 {
public:
    void FUN_004e1d60(int a, int b);
};

class Class_004e20a0 {
public:
    double RestartTimer();
};

class Class_004e1e50 {
public:
    void ReportElapsedTime(char* text);
};

extern int DAT_00529dd0;
extern char DAT_00529e20[];

extern double GetTimeSeconds();

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

    ~Timer();
    Timer(int param_1);
    Timer(int a, int b);
    double GetElapsedSeconds();
    void ResumeTimer();
    void ResetTimer();
    void FUN_004e21a0(double delta);
    void FUN_004e21c0(double elapsed);
    double FUN_004e21f0();
};

// A timer object: starts timing (0x4e1d60, which saves and raises the thread
// priority) and discards a first elapsed-time reading (0x4e20a0).
// FUNCTION: 0x4e1d20 ??0Timer@@QAE@H@Z
Timer::Timer(int param_1)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(0, param_1);
    ((Class_004e20a0*)this)->RestartTimer();
}

// A second constructor of the timer object of 0x4e1d20: starts timing
// (0x4e1d60) with both values given, without the first reading.
// FUNCTION: 0x4e1d40 ??0Timer@@QAE@HH@Z
Timer::Timer(int a, int b)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(a, b);
}

// The timer's counterpart to 0x4e1d60: pops the profiling nesting level,
// reports the elapsed time when the timer has a name (the hand-written
// routine at 0x4e1e50), and restores the process and thread priorities that
// 0x4e1d60 saved before raising them.
// FUNCTION: 0x4e1de0
Timer::~Timer()
{
    DAT_00529e20[--DAT_00529dd0] = 0;
    if (name) {
        ((Class_004e1e50*)this)->ReportElapsedTime(0);
    }
    if (boosted) {
        HANDLE process = GetCurrentProcess();
        SetPriorityClass(process, oldPriorityClass);
        HANDLE thread = GetCurrentThread();
        SetThreadPriority(thread, oldThreadPriority);
    }
}

// Timer: while stopped (flag at +0x48) the double at +0 holds the elapsed
// time; while running it holds the start time and the elapsed time is the
// current time (GetTimeSeconds) minus it. Callers set ecx to the timer
// (0x4e1eda, 0x4e2150), so this is a method.
// FUNCTION: 0x4e1e30
double Timer::GetElapsedSeconds()
{
    if (stopped) {
        return time;
    }
    return GetTimeSeconds() - time;
}

// FUNCTION: 0x4e2160
void Timer::ResumeTimer()
{
    if (stopped != 0) {
        double now = GetTimeSeconds();
        time = now - time;
        stopped = 0;
    }
}

// FUNCTION: 0x4e2180
void Timer::ResetTimer()
{
    time = 0;
    stopped = 1;
}

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

// FUNCTION: 0x4e21c0
void Timer::FUN_004e21c0(double elapsed)
{
    if (stopped) {
        time = elapsed;
    } else {
        time = GetTimeSeconds() - elapsed;
    }
}

// FUNCTION: 0x4e21f0
double Timer::FUN_004e21f0()
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
