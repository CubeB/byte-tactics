// Decompiled by Opus. Names are provisional.
// Returns the current time in seconds: from the performance counter when it
// is available, otherwise from the system time (100 ns file-time units).
#include <windows.h>

// FUNCTION: 0x4e1730
double __cdecl GetTimeSeconds()
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    BOOL haveFrequency = QueryPerformanceFrequency(&frequency);
    BOOL haveCounter = QueryPerformanceCounter(&counter);
    if (haveFrequency == TRUE && haveCounter == TRUE)
        return (double)counter.QuadPart / (double)frequency.QuadPart;

    SYSTEMTIME systemTime;
    FILETIME fileTime;
    GetSystemTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &fileTime);
    return ((double)fileTime.dwLowDateTime + (double)fileTime.dwHighDateTime * 4294967296.0) * 1e-7;
}
