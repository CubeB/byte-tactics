// Decompiled by Opus. Names are provisional.
// Returns 1 when the object's entry is named "MSGBOX.GUI" (after a
// two-character prefix); 0 when it has no entry.
#include <string.h>

struct Entry_004abd20 {
    char unknown_0[4];
    char* name;                        // +0x4
};

struct Object_004abd20 {
    char unknown_0[0x18];
    Entry_004abd20* entry;             // +0x18
};

// FUNCTION: 0x4abd20
int __stdcall IsMessageBoxScreen(Object_004abd20* obj)
{
    if (!obj->entry)
        return 0;
    return strcmp(obj->entry->name + 2, "MSGBOX.GUI") == 0;
}
