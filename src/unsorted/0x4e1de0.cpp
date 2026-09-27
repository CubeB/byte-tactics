// Decompiled by Opus. Names are provisional.
// The timer's counterpart to 0x4e1d60: pops the profiling nesting level,
// reports the elapsed time when the timer has a name (the hand-written
// routine at 0x4e1e50), and restores the process and thread priorities that
// 0x4e1d60 saved before raising them.
#include <windows.h>

class Class_004e1e50 {
public:
    void FUN_004e1e50(char* text);
};

class Class_004e1d20 {
public:
    char unknown_0[0x44];
    char* name;                        // +0x44
    char field_48;                     // +0x48
    char boosted;                      // +0x49
    char unknown_4a[2];
    DWORD oldPriorityClass;            // +0x4c
    int oldThreadPriority;             // +0x50

    ~Class_004e1d20();
};

extern int DAT_00529dd0;
extern char DAT_00529e20[];

// FUNCTION: 0x4e1de0
Class_004e1d20::~Class_004e1d20()
{
    DAT_00529e20[--DAT_00529dd0] = 0;
    if (name) {
        ((Class_004e1e50*)this)->FUN_004e1e50(0);
    }
    if (boosted) {
        HANDLE process = GetCurrentProcess();
        SetPriorityClass(process, oldPriorityClass);
        HANDLE thread = GetCurrentThread();
        SetThreadPriority(thread, oldThreadPriority);
    }
}
