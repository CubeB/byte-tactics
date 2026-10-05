// Decompiled by Sonnet. Names are provisional.
#include <malloc.h>

char IsMemFussy();
unsigned int __cdecl LookupBlockSize(void* p);

// FUNCTION: 0x4d8360
size_t __cdecl GetBlockSize(void* p)
{
    if (p == 0)
        return 0;

    if (IsMemFussy())
        return LookupBlockSize(p);

    return _msize(p);
}
