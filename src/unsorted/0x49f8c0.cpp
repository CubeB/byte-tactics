// Decompiled by Opus. Names are provisional.
// Whether the GUI entry list has an entry with the given name.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0049f8c0 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

struct Holder_0049f8c0 {
    char unknown_0[4];
    Entry_0049f8c0* entries;         // +0x04
};

struct Class_0049f8c0 {
    char unknown_0[0x18];
    Holder_0049f8c0* holder;         // +0x18
};

// FUNCTION: 0x49f8c0
int __stdcall FUN_0049f8c0(Class_0049f8c0* obj, char* name, int unused)
{
    Entry_0049f8c0* entries = obj->holder->entries;
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return 1;
        }
    }
    return 0;
}
