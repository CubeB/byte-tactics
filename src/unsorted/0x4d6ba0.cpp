// Decompiled by Sonnet. Names are provisional.
#include <string.h>

struct Class_004d6ba0 {
    char unknown_0[0x24];
    void* buffer;       // +0x24
    char unknown_28[4];
    void* pos;          // +0x2c
    void* end;          // +0x30
};

// FUNCTION: 0x4d6ba0
void __stdcall FUN_004d6ba0(Class_004d6ba0* param_1, const void* param_2, unsigned int param_3)
{
    memcpy(param_1->buffer, param_2, param_3);
    param_1->pos = param_1->end = (char*)param_1->buffer + param_3;
}
