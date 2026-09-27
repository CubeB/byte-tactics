// Decompiled by Opus. Names are provisional.
// Returns the (translated) description of the last DirectPlay error: the
// text after " - " in HAPINET_GetDPErrorString's message, falling back to
// DPERR_GENERIC (0x80004005).
#include <string.h>

extern int DAT_0051ff0c;

char* __stdcall FUN_004c9530(int error);
char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x4c9750
char* FUN_004c9750()
{
    char* s = FUN_004c9530(DAT_0051ff0c);
    if (s == 0)
        s = FUN_004c9530(0x80004005);
    s = strstr(s, " - ");
    if (s == 0)
        return FUN_004c9530(0x80004005);
    return FUN_004c5740(s + 3);
}
