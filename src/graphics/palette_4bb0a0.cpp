// Decompiled by Opus. Names are provisional.
// Returns 1 if the string contains a '.', else 0.
#include <string.h>

// FUNCTION: 0x4bb0a0
int __stdcall FUN_004bb0a0(char* name)
{
    for (unsigned int i = 0; i < strlen(name); i++) {
        if (name[i] == '.')
            return 1;
    }
    return 0;
}
