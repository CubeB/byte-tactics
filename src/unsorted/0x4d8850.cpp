// Decompiled by Haiku. Names are provisional.
#include <windows.h>

struct Class_004d8850 {
    char unknown_0[0xc];
    char field_c[0x20];

    void FUN_004d8850(char* param_1);
};

// FUNCTION: 0x4d8850
void Class_004d8850::FUN_004d8850(char* param_1)
{
    if (param_1 != 0) {
        lstrcpynA(field_c, param_1, 0x20);
    } else {
        field_c[0] = 0;
    }
}
