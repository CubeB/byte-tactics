// Decompiled by Opus. Names are provisional.
// The game's realloc() for the "memfussy" debug heap: under the allocator
// lock, allocates the new block, copies the smaller of the two sizes and
// frees the old block. Returns 0 (keeping the old block) on failure.
#include <windows.h>
#include <string.h>

CRITICAL_SECTION* FUN_004da780();
size_t __cdecl GetBlockSize(void* p);
void* __cdecl AllocDebugBlock(unsigned int size, int flags);
void __cdecl FreeDebugBlock(void* p, int flags);

// FUNCTION: 0x4dba40
void* __cdecl ReallocDebugBlock(void* p, unsigned int size, int flags)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* q = 0;
    size_t old = 0;
    if (p)
        old = GetBlockSize(p);
    if (size > 0) {
        q = AllocDebugBlock(size, flags);
        if (!q) {
            LeaveCriticalSection(cs);
            return 0;
        }
        unsigned int n = old >= size ? size : old;
        if (n > 0)
            memcpy(q, p, n);
    }
    if (p)
        FreeDebugBlock(p, flags);
    LeaveCriticalSection(cs);
    return q;
}
