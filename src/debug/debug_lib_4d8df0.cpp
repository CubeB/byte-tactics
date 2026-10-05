// Decompiled by Sonnet. Names are provisional.

void __cdecl FUN_004d8d70(int, int);

// This thread's stack pointer at the last check, which FUN_004d8d70 (whose
// object, src/debug/debug_lib_4d8d70.cpp, defines the thread's variables) has just
// stored.
extern __declspec(thread) char* g_stackLow;

// FUNCTION: 0x4d8df0
int __cdecl FUN_004d8df0(void)
{
    FUN_004d8d70(0, 0);
    return (int)g_stackLow;
}
