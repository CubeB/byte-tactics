// Decompiled by Opus. Names are provisional.
// Returns a function-local static critical section, initialised on first
// use. The empty inline destructor makes MSVC register the empty atexit
// thunk FUN_004da7c0.
#include <windows.h>

class CritSec_004da780 {
public:
    CRITICAL_SECTION cs;

    CritSec_004da780() { InitializeCriticalSection(&cs); }
    ~CritSec_004da780() {}
};

// FUNCTION: 0x4da780
CritSec_004da780* FUN_004da780()
{
    static CritSec_004da780 lock;
    return &lock;
}
