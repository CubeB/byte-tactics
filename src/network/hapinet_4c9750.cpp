// Decompiled by Opus. Names are provisional.
// Returns the (translated) description of the last DirectPlay error: the
// text after " - " in HAPINET_GetDPErrorString's message, falling back to
// DPERR_GENERIC (0x80004005).
#include <string.h>

extern int g_enumSessionsResult;

char* __stdcall HAPINET_GetDPErrorString(int error);
char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x4c9750
char* GetEnumSessionsErrorText()
{
    char* s = HAPINET_GetDPErrorString(g_enumSessionsResult);
    if (s == 0)
        s = HAPINET_GetDPErrorString(0x80004005);
    s = strstr(s, " - ");
    if (s == 0)
        return HAPINET_GetDPErrorString(0x80004005);
    return FUN_004c5740(s + 3);
}
