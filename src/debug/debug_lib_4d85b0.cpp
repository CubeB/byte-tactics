// Decompiled by Opus. Names are provisional.
// The game's free(): under the allocator lock, either hands the block to the
// "memfussy" debug heap or updates the allocation counters and frees it.
#include <windows.h>
#include <stdlib.h>

CRITICAL_SECTION* FUN_004da780();
char IsMemFussy();
void __cdecl FreeDebugBlock(void* p, int flags);
size_t __cdecl GetBlockSize(void* p);
void __cdecl CountFree(int param_1);

// FUNCTION: 0x4d85b0
void __cdecl GameFree(void* p)
{
    if (p) {
        CRITICAL_SECTION* cs = FUN_004da780();
        EnterCriticalSection(cs);
        if (IsMemFussy()) {
            FreeDebugBlock(p, 0);
        } else {
            CountFree(GetBlockSize(p));
            free(p);
        }
        LeaveCriticalSection(cs);
    }
}
