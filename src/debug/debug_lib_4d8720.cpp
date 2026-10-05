// Decompiled by Opus. Names are provisional.
// Changes the protection of a heap block, widened to whole pages when the
// "memfussy" switch (IsMemFussy) is on.
#include <stddef.h>

char IsMemFussy();
size_t __cdecl GetBlockSize(void* p);
void __cdecl ProtectPages(int addr, int size, int protect);
void __cdecl RoundRangeToPages(unsigned int* param_1, unsigned int* param_2);

// FUNCTION: 0x4d8720
void __cdecl ProtectBlock(void* p, int protect)
{
    unsigned int start = (unsigned int)p;
    size_t size = GetBlockSize(p);
    unsigned int end = start + size;
    if (IsMemFussy())
        RoundRangeToPages(&start, &end);
    ProtectPages(start, end - start, protect);
}
