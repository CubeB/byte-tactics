// Decompiled by Opus. Names are provisional.
// calloc-style allocation: count * size bytes from GameAlloc, zeroed.
#include <string.h>

void* __cdecl GameAlloc(unsigned int size);

// FUNCTION: 0x4d8460
void* __cdecl GameCalloc(unsigned int count, unsigned int size)
{
    unsigned int total = size * count;
    void* p = GameAlloc(total);
    if (p != 0) {
        memset(p, 0, total);
    }
    return p;
}
