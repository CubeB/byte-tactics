// Decompiled by Opus. Names are provisional.
// Returns 1 when the object's entry has a name (after a two-character
// prefix) equal to `name`, ignoring case, over at most 16 characters.
#include <string.h>

struct Entry_004ab060 {
    char unknown_0[4];
    char* name;                        // +0x4
};

struct Object_004ab060 {
    char unknown_0[0x18];
    Entry_004ab060* entry;             // +0x18
};

// FUNCTION: 0x4ab060
int __stdcall FUN_004ab060(Object_004ab060* obj, const char* name)
{
    if (obj->entry && _strnicmp(obj->entry->name + 2, name, 0x10) == 0)
        return 1;
    return 0;
}
