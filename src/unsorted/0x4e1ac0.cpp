// Decompiled by Opus. Names are provisional.
// Returns a function-local static critical section, initialised on first
// use (same shape as 0x4da780). The empty inline destructor makes MSVC
// register the empty atexit thunk FUN_004e1b00.
#include <windows.h>

class CritSec_004e1ac0 {
public:
    CRITICAL_SECTION cs;

    CritSec_004e1ac0() { InitializeCriticalSection(&cs); }
    ~CritSec_004e1ac0() {}
};

// FUNCTION: 0x4e1ac0
CritSec_004e1ac0* FUN_004e1ac0()
{
    static CritSec_004e1ac0 lock;
    return &lock;
}
