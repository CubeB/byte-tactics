// Decompiled by Opus. Names are provisional.
#include <windows.h>

class Class_004e20a0 {
public:
    double RestartTimer();
};

class Class_004e1d60 {
public:
    char unknown_0[0x40];
    int field_40;                      // +0x40
    int field_44;                      // +0x44
    char field_48;                     // +0x48
    char boosted;                      // +0x49
    char unknown_4a[2];
    DWORD oldPriorityClass;            // +0x4c
    int oldThreadPriority;             // +0x50

    void FUN_004e1d60(int a, int b);
};

extern int DAT_00529dd0;
extern char DAT_00529dd4;
extern char DAT_00529e20[];

// FUNCTION: 0x4e1d60
void Class_004e1d60::FUN_004e1d60(int a, int b)
{
    field_40 = b;
    field_44 = a;
    DAT_00529e20[DAT_00529dd0++] = 9;
    boosted = DAT_00529dd4;
    if (boosted) {
        HANDLE thread = GetCurrentThread();
        oldThreadPriority = GetThreadPriority(thread);
        SetThreadPriority(thread, THREAD_PRIORITY_ABOVE_NORMAL);
        HANDLE process = GetCurrentProcess();
        oldPriorityClass = GetPriorityClass(process);
        SetPriorityClass(process, HIGH_PRIORITY_CLASS);
    }
    ((Class_004e20a0*)this)->RestartTimer();
}
