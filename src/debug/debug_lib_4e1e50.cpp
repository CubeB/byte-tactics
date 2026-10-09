// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <string.h>

unsigned char __cdecl HasPerfCounters(void);
double __cdecl GetTimeSeconds();
void InitPerformanceEvents(void);
bool __cdecl ProgramPerfEvent(unsigned int effect, unsigned int level, char f1, char f2);

// One of the two performance counters the profiler reads.
struct Counter_004e1e50 {
    unsigned int event;                 // +0x0, the event it counts
    const char* name;                   // +0x4
    int unknown_8;
    unsigned int base;                  // +0xc, the event it is shown as a share of
};

extern Counter_004e1e50 g_pmcEvent0;
extern Counter_004e1e50 g_pmcEvent1;
extern unsigned char g_perfEnabled;     // profiling is on
extern unsigned char g_perfDisplayInDebugger;  // reports go to the debugger too
extern unsigned char g_perfDisplayInWindow;  // reports go to the log
extern char g_reportIndentText[];       // the indent of the current report
extern char DAT_005119b8[];             // ""

// A report for the log: its name and its text.
class NameKey {
public:
    const char* name;                   // +0x0
    char text[500];                     // +0x4

    NameKey(const char* name_)
    {
        name = name_;
        if (!name_)
            name = DAT_005119b8;
        text[0] = 0;
    }
};

class NameTable {
public:
    void Upsert(const NameKey& key);
};

NameTable* GetNameTable();

// A named timer that also reads the two performance counters.
class Timer {
public:
    double time;                        // +0x0, elapsed when stopped, else the start time
    __int64 start0;                     // +0x8, counter 0 at the start
    __int64 start1;                     // +0x10, counter 1 at the start
    char unknown_18[0x28];
    int flags;                          // +0x40, bit 1: report to the debugger
    const char* name;                   // +0x44
    char stopped;                       // +0x48

    double GetElapsedSeconds();
    void ReportElapsedTime(const char* label);
    double RestartTimer();
};

// Reports the elapsed time and the counters since the start, to the
// debugger and to the log.
// FUNCTION: 0x4e1e50
void Timer::ReportElapsedTime(const char* label)
{
    __int64 count1;
    __int64 count0;
    char text[500];
    if (!g_perfEnabled || !HasPerfCounters())
        return;
    if (HasPerfCounters()) {
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
        p = text + sprintf(text, "%sElapsed time for '%s' is %g sec", g_reportIndentText, label, GetElapsedSeconds());
    else
        p = text + sprintf(text, "%sElapsed time is %g sec", g_reportIndentText, GetElapsedSeconds());
    if (HasPerfCounters()) {
        p += sprintf(p, "\r\n\t%s%s: %u", g_reportIndentText, g_pmcEvent0.name, (unsigned int)count0);
        if (g_pmcEvent0.base == g_pmcEvent1.event)
            p += sprintf(p, " (%1.2f%%)", (double)count0 * 100.0 / (double)count1);
        p += sprintf(p, "\r\n\t%s%s: %u", g_reportIndentText, g_pmcEvent1.name, (unsigned int)count1);
        if (g_pmcEvent1.base == g_pmcEvent0.event)
            p += sprintf(p, " (%1.2f%%)", (double)count1 * 100.0 / (double)count0);
    }
    strcat(text, "\r\n");
    if (g_perfDisplayInDebugger && (flags & 2))
        OutputDebugStringA(text);
    if (g_perfDisplayInWindow) {
        NameKey report(label);
        strcpy(report.text, text);
        GetNameTable()->Upsert(report);
    }
}

// Starts the timer (again) and the counters; the time it had run.
// FUNCTION: 0x4e20a0
double Timer::RestartTimer()
{
    double elapsed;
    __int64 count0;
    __int64 count1;
    if (HasPerfCounters()) {
        InitPerformanceEvents();
        ProgramPerfEvent(g_pmcEvent0.event, 0, 1, 0);
        ProgramPerfEvent(g_pmcEvent1.event, 1, 1, 0);
    }
    double now = GetTimeSeconds();
    if (stopped)
        elapsed = time;
    else
        elapsed = now - time;
    time = now;
    stopped = 0;
    if (HasPerfCounters()) {
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
