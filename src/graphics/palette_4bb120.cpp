// Decompiled by Opus. Names are provisional.
#include <string.h>

// Cuts a path after its last backslash ("a\\b\\c.txt" -> "a\\b\\").
// The scan starts at strlen, on the terminator, not at strlen - 1: starting
// one lower adds a `dec ecx` that MSVC cannot fold into the strlen sequence.
// FUNCTION: 0x4bb120
char* __stdcall StripFileName(char* path)
{
    for (int i = strlen(path); i >= 0; i--) {
        if (path[i] == '\\') {
            path[i + 1] = '\0';
            break;
        }
    }
    return path;
}
