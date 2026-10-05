// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_4a04f0 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0x29 - 0x12];
    char f_29;                       // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                     // +0xb6
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

struct Holder_4a04f0 {
    char unknown_0[4];
    Entry_4a04f0* entries;           // +0x04
};

struct Dialog {
    char unknown_0[0x18];
    Holder_4a04f0* holder;           // +0x18
};

static inline int FindEntry(Entry_4a04f0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a04f0
char __stdcall FUN_004a04f0(Dialog* obj, char* name)
{
    Entry_4a04f0* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i == -1) {
        return -1;
    }
    return entries[i].f_29;
}
