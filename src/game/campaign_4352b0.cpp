// Decompiled by Haiku. Names are provisional.

#include <string.h>

class Mission {
public:
    char unknown_0[0x4];
    char name[1];       // +0x4

    char* FUN_004352b0();
};

// FUNCTION: 0x4352b0
char* Mission::FUN_004352b0()
{
    char* ptr = name;
    if (strlen(ptr) > 0) {
        return ptr;
    }
    return 0;
}
