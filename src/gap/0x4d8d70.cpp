// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>

// This thread's stack, as the last check saw it (the image's .tls section
// holds them at +4, +8 and +0xc; 0x4d8df0.cpp and 0x4d8e20.cpp read the first
// two through a struct view). They are separate variables: as fields of one
// struct, the compiler would not read the flag before storing stackLow.
__declspec(thread) char* g_stackLow;       // the stack pointer at the last check
__declspec(thread) char* g_stackHigh;      // the end of the stack's memory region
__declspec(thread) char g_stackKnown;      // g_stackHigh has been looked up

// Whether [p, p + size) lies outside this thread's live stack: 0 when it is
// between the current stack pointer and the end of the stack's region.
// FUNCTION: 0x4d8d70
char __cdecl FUN_004d8d70(char* p, int size)
{
    MEMORY_BASIC_INFORMATION info;
    char* top;
    __asm mov top, esp
    g_stackLow = top;
    if (!g_stackKnown) {
        g_stackKnown = 1;
        if (VirtualQuery(top, &info, sizeof info) != sizeof info)
            return 1;
        g_stackHigh = (char*)info.BaseAddress + info.RegionSize;
    }
    if (p < g_stackLow || p + size > g_stackHigh)
        return 1;
    return 0;
}
