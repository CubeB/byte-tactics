// Decompiled by Opus and Haiku. Names are provisional, except the operators.

#include <stddef.h>

void* __cdecl GameAllocThunk(size_t size);
void __cdecl GameFreeIndirectThunk(void* p);

// Cavedog replaced the global operator new and delete, scalar and array,
// with wrappers around the game's own allocator.
// FUNCTION: 0x4b4f10
void* __cdecl operator new(size_t size)
{
    return GameAllocThunk(size);
}

// FUNCTION: 0x4b4f20
void __cdecl operator delete(void* p)
{
    GameFreeIndirectThunk(p);
}

// FUNCTION: 0x4b4f30 ??_U@YAPAXI@Z
void* __cdecl operator new[](size_t size)
{
    return GameAllocThunk(size);
}

// FUNCTION: 0x4b4f40 ??_V@YAXPAX@Z
void __cdecl operator delete[](void* p)
{
    GameFreeIndirectThunk(p);
}
