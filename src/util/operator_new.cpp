// Decompiled by Opus and Haiku. Names are provisional, except the operators.

#include <stddef.h>

void* __cdecl FUN_004d8660(size_t size);
void __cdecl FUN_004d8670(void* p);

// Cavedog replaced the global operator new and delete with wrappers around
// the game's own allocator.
// FUNCTION: 0x4b4f10
void* __cdecl operator new(size_t size)
{
    return FUN_004d8660(size);
}

// FUNCTION: 0x4b4f20
void __cdecl operator delete(void* p)
{
    FUN_004d8670(p);
}

// FUNCTION: 0x4b4f30
void __cdecl FUN_004b4f30(unsigned int param_1)
{
    FUN_004d8660(param_1);
}

// FUNCTION: 0x4b4f40
void __cdecl FUN_004b4f40(void* param_1)
{
    FUN_004d8670(param_1);
}
