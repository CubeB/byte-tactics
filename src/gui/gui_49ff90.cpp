// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_49ff90 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

void __stdcall FUN_004b6290(char* path);

static inline int FindEntry(Entry_49ff90* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// Like 0x49ff10, but reports a missing entry.
// FUNCTION: 0x49ff90
Entry_49ff90* __stdcall FUN_0049ff90(Entry_49ff90* entries, char* name)
{
    int i = FindEntry(entries, name);
    if (i != -1) {
        return &entries[i];
    }
    FUN_004b6290("Error in GUI layout");
    return 0;
}
