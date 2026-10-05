// Decompiled by Opus. Names are provisional.
// A byte-identical copy of 0x49ff90: finds a GUI entry by name (entries
// 1..count, where count is entry 0's field +0xb6) and reports a missing one.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0010 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

void __stdcall FatalError(char* path);

static inline int FindEntry(Entry_004a0010* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a0010
Entry_004a0010* __stdcall FUN_004a0010(Entry_004a0010* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FatalError("Error in GUI layout");
    return 0;
}
