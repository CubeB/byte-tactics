// Decompiled by Opus. Names are provisional.
// The game's realloc() for the "memfussy" debug heap: under the allocator
// lock, allocates the new block, copies the smaller of the two sizes and
// frees the old block. Returns 0 (keeping the old block) on failure.
#include <windows.h>
#include <string.h>

CRITICAL_SECTION* FUN_004da780();
size_t __cdecl FUN_004d8360(void* p);
void* __cdecl FUN_004dacf0(unsigned int size, int flags);
void __cdecl FUN_004db7d0(void* p, int flags);

// FUNCTION: 0x4dba40
void* __cdecl FUN_004dba40(void* p, unsigned int size, int flags)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* q = 0;
    size_t old = 0;
    if (p)
        old = FUN_004d8360(p);
    if (size > 0) {
        q = FUN_004dacf0(size, flags);
        if (!q) {
            LeaveCriticalSection(cs);
            return 0;
        }
        unsigned int n = old >= size ? size : old;
        if (n > 0)
            memcpy(q, p, n);
    }
    if (p)
        FUN_004db7d0(p, flags);
    LeaveCriticalSection(cs);
    return q;
}
