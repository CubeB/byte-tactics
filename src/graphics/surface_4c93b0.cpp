// Decompiled by Sonnet. Names are provisional.
#include <stdlib.h>

struct Class_004c93b0 {
    char* ptr;

    Class_004c93b0* FUN_004c93b0(Class_004c93b0* param_1);
};

// FUNCTION: 0x4c93b0
Class_004c93b0* Class_004c93b0::FUN_004c93b0(Class_004c93b0* param_1)
{
    *(int*)(param_1->ptr - 4) += 1;
    *(int*)(ptr - 4) -= 1;
    if (*(int*)(ptr - 4) == 0) {
        free(ptr - 4);
    }
    ptr = param_1->ptr;
    return this;
}
