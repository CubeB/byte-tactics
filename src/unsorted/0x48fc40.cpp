// Decompiled by Sonnet. Names are provisional.
#include <stdlib.h>

struct Other_0048fc40 {
    char unknown_0[0x78];
    short value;   // +0x78
};

struct Class_0048fc40 {
    int unknown_0;  // +0x0
    int unknown_4;  // +0x4

    bool FUN_0048fc40(Other_0048fc40* param_1);
};

// FUNCTION: 0x48fc40
bool Class_0048fc40::FUN_0048fc40(Other_0048fc40* param_1)
{
    int diff = abs((int)param_1->value - unknown_4);

    int* flag = (int*)((char*)this - 8);
    if (diff <= 2) {
        *flag = 1;
    }
    return *flag == 0;
}
