// Decompiled by Sonnet. Names are provisional.
#include <stdlib.h>

struct Other_0048fb30 {
    char unknown_0[0x76];
    short value;   // +0x76
};

struct Class_0048fb30 {
    int unknown_0;  // +0x0
    int unknown_4;  // +0x4

    bool FUN_0048fb30(Other_0048fb30* param_1);
};

// FUNCTION: 0x48fb30
bool Class_0048fb30::FUN_0048fb30(Other_0048fb30* param_1)
{
    int diff = abs((int)param_1->value - unknown_4);

    int* flag = (int*)((char*)this - 8);
    if (diff <= 2) {
        *flag = 1;
    }
    return *flag == 0;
}
