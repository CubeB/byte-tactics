// Decompiled by Haiku. Names are provisional.
#include <string.h>

// FUNCTION: 0x43c020
int __stdcall FUN_0043c020(void* param_1, void* param_2)
{
    const char* str1 = *(const char**)((char*)param_1 + 0x15);
    const char* str2 = *(const char**)((char*)param_2 + 0x15);
    return _strcmpi(str1, str2) < 0 ? 1 : 0;
}
