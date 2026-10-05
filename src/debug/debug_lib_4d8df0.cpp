// Decompiled by Sonnet. Names are provisional.

void __cdecl IsOutsideStack(int, int);

// This thread's stack pointer at the last check, which IsOutsideStack (whose
// object, src/debug/debug_lib_4d8d70.cpp, defines the thread's variables) has just
// stored.
extern __declspec(thread) char* g_stackLow;

// FUNCTION: 0x4d8df0
int __cdecl GetStackLow(void)
{
    IsOutsideStack(0, 0);
    return (int)g_stackLow;
}
