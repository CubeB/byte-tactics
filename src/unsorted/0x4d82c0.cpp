// Decompiled by Opus. Names are provisional.
#include <string.h>

// Fills n bytes at p with a repeating 4-byte pattern (a debug-heap fill; its
// checking counterpart follows at 0x4d8310).
// FUNCTION: 0x4d82c0
void FUN_004d82c0(void* p, unsigned int pattern, unsigned int n)
{
    unsigned int* d = (unsigned int*)p;
    while (n >= 4) {
        *d++ = pattern;
        n -= 4;
    }
    if (n > 0)
        memcpy(d, &pattern, n);
}
