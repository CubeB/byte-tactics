// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Entry_00428850 {
    int value;                         // +0x0
    int unknown_4;                     // +0x4
    char name[0x20];                   // +0x8
};

extern Entry_00428850 DAT_005120b8[10];

// FUNCTION: 0x428850
int __stdcall FUN_00428850(char* name)
{
    if (name == 0)
        return 0;
    for (int i = 0; i < 10; i++) {
        if (strcmp(DAT_005120b8[i].name, name) == 0)
            return DAT_005120b8[i].value;
    }
    return 0;
}
