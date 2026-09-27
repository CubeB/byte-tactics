// Decompiled by Opus. Names are provisional.
// calloc-style allocation: count * size bytes from FUN_004d83c0, zeroed.
#include <string.h>

void* FUN_004d83c0(unsigned int size);

// FUNCTION: 0x4d8460
void* FUN_004d8460(unsigned int count, unsigned int size)
{
    unsigned int total = size * count;
    void* p = FUN_004d83c0(total);
    if (p != 0) {
        memset(p, 0, total);
    }
    return p;
}
