// Decompiled by Haiku. Names are provisional.
#include <stdlib.h>

struct CMemoryCache
{
    void ClearPointers();
};

extern CMemoryCache DAT_00511f80;
extern void __cdecl FUN_00420610();

// FUNCTION: 0x4205f0
void FUN_004205f0(void)
{
    DAT_00511f80.ClearPointers();
    atexit(FUN_00420610);
}
