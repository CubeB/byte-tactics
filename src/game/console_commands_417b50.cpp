// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Splits the command line param_1 into tokens in a local tokenizer object and
// dispatches it with the flags param_2. A null command line copies the global
// command buffer DAT_00511bd0 instead. Compare 0x416780.

#include <string.h>

extern char DAT_00511bd0[];

class Class_004b73b0 {
public:
    char unknown_0[0xd0];
    int field_d0;                      // +0xd0

    Class_004b73b0* FUN_004b73b0();
};

class Class_004b7440 {
public:
    char unknown_0[0xd0];
    int field_d0;                      // +0xd0

    int FUN_004b7440(char* param_1, char* param_2);
};

void __stdcall FUN_004b7900(void* param_1, int param_2);

// FUNCTION: 0x417b50
void __stdcall FUN_00417b50(char* param_1, int param_2)
{
    char buf[0xd4];

    if (param_1 == 0)
        param_1 = DAT_00511bd0;
    else
        strncpy(DAT_00511bd0, param_1, 0x4f);

    ((Class_004b73b0*)buf)->FUN_004b73b0();
    ((Class_004b7440*)buf)->FUN_004b7440(param_1, 0);
    FUN_004b7900(buf, param_2);
}
