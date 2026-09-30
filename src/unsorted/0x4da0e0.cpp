// Decompiled by Opus. Names are provisional.
// Returns whether arg starts (case-insensitively) with one of the 19 option
// names in the table at 0x50c908 ("memfussy" ... "saveresources").
#include <string.h>

extern char* DAT_0050c908[19];

// FUNCTION: 0x4da0e0
bool __cdecl FUN_004da0e0(const char* arg)
{
    for (char** p = DAT_0050c908; p < DAT_0050c908 + 19; p++) {
        if (_strnicmp(*p, arg, strlen(*p)) == 0)
            return true;
    }
    return false;
}
