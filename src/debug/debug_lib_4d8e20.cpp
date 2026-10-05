// Decompiled by Sonnet. Names are provisional.
// fs:[0x2c] is the thread-local storage array pointer; the value read back
// after the IsOutsideStack(0, 0) call is the end of this thread's stack, which
// IsOutsideStack looks up the first time (src/debug/debug_lib_4d8d70.cpp defines the
// thread's variables).

extern "C" int __cdecl IsOutsideStack(int, int);

extern __declspec(thread) char* g_stackHigh;

// FUNCTION: 0x4d8e20
void* GetStackHigh()
{
    IsOutsideStack(0, 0);
    return g_stackHigh;
}
