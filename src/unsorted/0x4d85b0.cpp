// Decompiled by Opus. Names are provisional.
// The game's free(): under the allocator lock, either hands the block to the
// "memfussy" debug heap or updates the allocation counters and frees it.
#include <windows.h>
#include <stdlib.h>

CRITICAL_SECTION* FUN_004da780();
char FUN_004d80d0();
void __cdecl FUN_004db7d0(void* p, int flags);
size_t __cdecl FUN_004d8360(void* p);
void __cdecl FUN_004da840(int param_1);

// FUNCTION: 0x4d85b0
void __cdecl FUN_004d85b0(void* p)
{
    if (p) {
        CRITICAL_SECTION* cs = FUN_004da780();
        EnterCriticalSection(cs);
        if (FUN_004d80d0()) {
            FUN_004db7d0(p, 0);
        } else {
            FUN_004da840(FUN_004d8360(p));
            free(p);
        }
        LeaveCriticalSection(cs);
    }
}
