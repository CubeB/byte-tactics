// Decompiled by Sonnet. Names are provisional.
// fs:[0x2c] is the thread-local storage array pointer; the value read back
// after the FUN_004d8d70(0, 0) call is the end of this thread's stack, which
// FUN_004d8d70 looks up the first time (src/gap/0x4d8d70.cpp defines the
// thread's variables).

extern "C" int __cdecl FUN_004d8d70(int, int);

extern __declspec(thread) char* g_stackHigh;

// FUNCTION: 0x4d8e20
void* FUN_004d8e20()
{
    FUN_004d8d70(0, 0);
    return g_stackHigh;
}
