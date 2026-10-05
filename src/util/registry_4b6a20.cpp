// Decompiled by Opus. Names are provisional.
// Stores a string value (type 1) through AccessRegistryValue, passing its length
// including the terminator.
#include <string.h>

extern int __stdcall AccessRegistryValue(void*, void*, void*, void*, int, int);

// FUNCTION: 0x4b6a20
void __stdcall WriteRegistryString(void* param_1, void* param_2, char* value)
{
    unsigned int size = strlen(value) + 1;
    AccessRegistryValue(param_1, param_2, value, &size, 1, 0);
}
