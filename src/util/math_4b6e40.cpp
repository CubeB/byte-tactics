// Decompiled by Opus. Names are provisional.
// Approximate length of (dx, dy): the larger magnitude plus a quarter of the
// smaller one.
#include <stdlib.h>

// FUNCTION: 0x4b6e40
int __stdcall ApproxDistance(int dx, int dy)
{
    int a = abs(dx);
    int b = abs(dy);
    if (a > b)
        return (b >> 2) + a;
    return (a >> 2) + b;
}
