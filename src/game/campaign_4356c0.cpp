// Decompiled by Sonnet. Names are provisional.
#include <string.h>

class Mission {
public:
    int FUN_004356c0(int param_1);
};

// FUNCTION: 0x4356c0
int Mission::FUN_004356c0(int param_1)
{
    char* ptr = (char*)this + param_1 * 0x100 + 0x104;
    return 0 < strlen(ptr) ? (int)ptr : 0;
}
