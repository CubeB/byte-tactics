// Decompiled by Haiku. Names are provisional.
#include <malloc.h>

extern void* operator new(size_t size);

// FUNCTION: 0x46ff70
void __stdcall FUN_0046ff70(int param_1, int param_2)
{
    void* eax = operator new(0x24);
    *(int*)((unsigned char*)eax + 4) = param_1;
    *(int*)((unsigned char*)eax + 0x20) = param_2;
}
