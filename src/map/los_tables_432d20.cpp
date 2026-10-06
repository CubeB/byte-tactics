// Decompiled by Haiku and Sonnet. Names are provisional.
#include <stdlib.h>

// A reference-counted string handle, with the count in the dword before the
// characters, and an int after it.
struct Class_004c93b0 {
    char* ptr;                         // +0x0
    int field_4;                       // +0x4

    void* FUN_00432d20(Class_004c93b0* param);
    Class_004c93b0* Assign(Class_004c93b0* param_1);
};

// FUNCTION: 0x432d20
void* Class_004c93b0::FUN_00432d20(Class_004c93b0* param)
{
    Assign(param);
    field_4 = param->field_4;
    return this;
}

// The original calls this from 0x432d20 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4c93b0
Class_004c93b0* Class_004c93b0::Assign(Class_004c93b0* param_1)
{
    *(int*)(param_1->ptr - 4) += 1;
    *(int*)(ptr - 4) -= 1;
    if (*(int*)(ptr - 4) == 0) {
        free(ptr - 4);
    }
    ptr = param_1->ptr;
    return this;
}
#pragma auto_inline(on)
