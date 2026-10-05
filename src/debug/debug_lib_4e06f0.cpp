// Decompiled by Opus. Names are provisional.
// Returns a critical section that is initialised on first use (a
// function-local static; its atexit destructor 0x4e0730 is empty).
#include <windows.h>

class CritSec_004e06f0 {
public:
    CRITICAL_SECTION cs;

    CritSec_004e06f0() { InitializeCriticalSection(&cs); }
    ~CritSec_004e06f0() {}
};

// FUNCTION: 0x4e06f0
LPCRITICAL_SECTION FUN_004e06f0(void)
{
    static CritSec_004e06f0 lock;
    return &lock.cs;
}
