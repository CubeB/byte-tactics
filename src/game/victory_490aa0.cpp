// Decompiled by Haiku. Names are provisional.
#include <windows.h>

// FUNCTION: 0x490aa0
void FUN_00490aa0(void)
{
    MEMORYSTATUS mem;
    mem.dwLength = 0x20;
    GlobalMemoryStatus(&mem);
}
