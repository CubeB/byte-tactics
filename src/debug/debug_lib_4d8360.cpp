// Decompiled by Sonnet. Names are provisional.
#include <malloc.h>

char FUN_004d80d0();
unsigned int __cdecl FUN_004dbae0(void* p);

// FUNCTION: 0x4d8360
size_t __cdecl FUN_004d8360(void* p)
{
    if (p == 0)
        return 0;

    if (FUN_004d80d0())
        return FUN_004dbae0(p);

    return _msize(p);
}
