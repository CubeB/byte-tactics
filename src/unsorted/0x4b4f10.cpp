// Decompiled by Opus. Names are provisional, except the operators.
// Cavedog replaced the global operator new and delete with wrappers around
// the game's own allocator.
#include <stddef.h>

void* FUN_004d8660(size_t size);
void FUN_004d8670(void* p);

// FUNCTION: 0x4b4f10
void* operator new(size_t size)
{
    return FUN_004d8660(size);
}

// FUNCTION: 0x4b4f20
void operator delete(void* p)
{
    FUN_004d8670(p);
}
