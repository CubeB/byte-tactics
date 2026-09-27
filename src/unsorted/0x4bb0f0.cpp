// Decompiled by Opus. Names are provisional.
// Cuts a file name at its last '.' ("a\\b.txt" -> "a\\b").
#include <string.h>

// FUNCTION: 0x4bb0f0
char* __stdcall FUN_004bb0f0(char* name)
{
    for (int i = strlen(name); i >= 0; i--) {
        if (name[i] == '.') {
            name[i] = '\0';
            break;
        }
    }
    return name;
}
