// Decompiled by Sonnet. Names are provisional.
#include <string.h>

extern int __cdecl _strcmpi(const char*, const char*);

// FUNCTION: 0x43a940
int __stdcall OrderTypeNameLess(int param_1, char* param_2)
{
    int result = _strcmpi(*(char**)(param_1 + 0x15), param_2);
    return result < 0;
}
