// Decompiled by Opus. Names are provisional.
#include <string.h>

// Looks an entry up by name in a list; returns the entry or 0.
void* __stdcall FindGafEntry(void* list, char* name);

// FUNCTION: 0x4222b0
void* __stdcall FUN_004222b0(void* list, int unused, char* name)
{
    // Returning a value on both paths keeps them apart: with a void return the
    // early exit is folded into a jump to the shared epilogue.
    if (strlen(name) == 0) {
        return 0;
    }
    return FindGafEntry(list, name);
}
