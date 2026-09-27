// Decompiled by Opus. Names are provisional.
// As in 0x401220 and 0x4012a0, the header include decides which operand
// MSVC loads first (<stdlib.h> here).
#include <stdlib.h>

struct Obj_00401320 {
    float x0;                          // +0x00
    float x1;                          // +0x04
    float x2;                          // +0x08
    float x3;                          // +0x0c
    float prev0;                       // +0x10
    float prev1;                       // +0x14
};

// FUNCTION: 0x401320
void __stdcall FUN_00401320(Obj_00401320* p, float a, float b)
{
    p->prev1 = p->x1;
    p->prev0 = p->x0;
    p->x3 -= a * p->x3;
    p->x3 += p->x2 - b * p->x2;
    p->x1 = 0;
    p->x0 = 0;
    p->x2 = 0;
}
