// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <string.h>

unsigned char __cdecl FUN_004e1680(void);
double __cdecl FUN_004e1730();
void FUN_004e1be0(void);
bool __cdecl FUN_004e3750(unsigned int effect, unsigned int level, char f1, char f2);

// One of the two performance counters the profiler reads.
struct Counter_004e1e50 {
    unsigned int event;                 // +0x0, the event it counts
    const char* name;                   // +0x4
    int unknown_8;
    unsigned int base;                  // +0xc, the event it is shown as a share of
};

extern Counter_004e1e50 DAT_00529e00;
extern Counter_004e1e50 DAT_00529e10;
extern unsigned char DAT_00529dd8;      // profiling is on
extern unsigned char DAT_00529ddc;      // reports go to the debugger too
extern unsigned char DAT_00529e64;      // reports go to the log
extern char DAT_00529e20[];             // the indent of the current report
extern char DAT_005119b8[];             // ""

// A report for the log: its name and its text.
class Class_004e1a30 {
public:
    const char* name;                   // +0x0
    char text[500];                     // +0x4

    Class_004e1a30(const char* name_)
    {
        name = name_;
        if (!name_)
            name = DAT_005119b8;
        text[0] = 0;
    }
};

class Class_004e17c0;
Class_004e17c0* FUN_004e1a90();

class Class_004e1990 {
public:
    void FUN_004e1990(const Class_004e1a30& key);
};

// A named timer that also reads the two performance counters.
class Class_004e1e30 {
public:
    double time;                        // +0x0, elapsed when stopped, else the start time
    __int64 start0;                     // +0x8, counter 0 at the start
    __int64 start1;                     // +0x10, counter 1 at the start
    char unknown_18[0x28];
    int flags;                          // +0x40, bit 1: report to the debugger
    const char* name;                   // +0x44
    char stopped;                       // +0x48

    double FUN_004e1e30();
    void FUN_004e1e50(const char* label);
    double FUN_004e20a0();
};

// Reports the elapsed time and the counters since the start, to the
// debugger and to the log.
// FUNCTION: 0x4e1e50
void Class_004e1e30::FUN_004e1e50(const char* label)
{
    __int64 count1;
    __int64 count0;
    char text[500];
    if (!DAT_00529dd8 || !FUN_004e1680())
        return;
    if (FUN_004e1680()) {
        __asm {
            mov ecx, 0
            _emit 0x0f      // rdpmc, which this compiler's assembler does not know
            _emit 0x33
            mov dword ptr count0, eax
            mov dword ptr count0[4], edx
            mov ecx, 1
            _emit 0x0f      // rdpmc
            _emit 0x33
            mov dword ptr count1, eax
            mov dword ptr count1[4], edx
        }
        count0 -= start0;
        count1 -= start1;
    }
    if (!label)
        label = name;
    char* p;
    if (label)
        p = text + sprintf(text, "%sElapsed time for '%s' is %g sec", DAT_00529e20, label, FUN_004e1e30());
    else
        p = text + sprintf(text, "%sElapsed time is %g sec", DAT_00529e20, FUN_004e1e30());
    if (FUN_004e1680()) {
        p += sprintf(p, "\r\n\t%s%s: %u", DAT_00529e20, DAT_00529e00.name, (unsigned int)count0);
        if (DAT_00529e00.base == DAT_00529e10.event)
            p += sprintf(p, " (%1.2f%%)", (double)count0 * 100.0 / (double)count1);
        p += sprintf(p, "\r\n\t%s%s: %u", DAT_00529e20, DAT_00529e10.name, (unsigned int)count1);
        if (DAT_00529e10.base == DAT_00529e00.event)
            p += sprintf(p, " (%1.2f%%)", (double)count1 * 100.0 / (double)count0);
    }
    strcat(text, "\r\n");
    if (DAT_00529ddc && (flags & 2))
        OutputDebugStringA(text);
    if (DAT_00529e64) {
        Class_004e1a30 report(label);
        strcpy(report.text, text);
        ((Class_004e1990*)FUN_004e1a90())->FUN_004e1990(report);
    }
}

// Starts the timer (again) and the counters; the time it had run.
// FUNCTION: 0x4e20a0
double Class_004e1e30::FUN_004e20a0()
{
    double elapsed;
    __int64 count0;
    __int64 count1;
    if (FUN_004e1680()) {
        FUN_004e1be0();
        FUN_004e3750(DAT_00529e00.event, 0, 1, 0);
        FUN_004e3750(DAT_00529e10.event, 1, 1, 0);
    }
    double now = FUN_004e1730();
    if (stopped)
        elapsed = time;
    else
        elapsed = now - time;
    time = now;
    stopped = 0;
    if (FUN_004e1680()) {
        __asm {
            mov ecx, 0
            _emit 0x0f      // rdpmc
            _emit 0x33
            mov dword ptr count0, eax
            mov dword ptr count0[4], edx
            mov ecx, 1
            _emit 0x0f      // rdpmc
            _emit 0x33
            mov dword ptr count1, eax
            mov dword ptr count1[4], edx
        }
        start0 = count0;
        start1 = count1;
    }
    return elapsed;
}
