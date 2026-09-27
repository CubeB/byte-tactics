// Decompiled by Opus. Names are provisional.
// Changes the protection of a heap block, widened to whole pages when the
// "memfussy" switch (FUN_004d80d0) is on.
#include <stddef.h>

char FUN_004d80d0();
size_t __cdecl FUN_004d8360(void* p);
void __cdecl FUN_004d86b0(int addr, int size, int protect);
void __cdecl FUN_004dbb90(unsigned int* param_1, unsigned int* param_2);

// FUNCTION: 0x4d8720
void __cdecl FUN_004d8720(void* p, int protect)
{
    unsigned int start = (unsigned int)p;
    size_t size = FUN_004d8360(p);
    unsigned int end = start + size;
    if (FUN_004d80d0())
        FUN_004dbb90(&start, &end);
    FUN_004d86b0(start, end - start, protect);
}
