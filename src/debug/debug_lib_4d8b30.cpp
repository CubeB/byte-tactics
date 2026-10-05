// Decompiled by Haiku. Names are provisional.

#include <windows.h>

struct RunningStats {
    char unknown_0[0x98];
    char field_98[0x20];

    void FUN_004d8b30(const char* param_1);
};

// FUNCTION: 0x4d8b30
void RunningStats::FUN_004d8b30(const char* param_1)
{
    if (param_1 != 0) {
        lstrcpynA(field_98, param_1, 0x20);
    } else {
        field_98[0] = 0;
    }
}
