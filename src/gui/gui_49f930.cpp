// Decompiled by Opus. Names are provisional.
// Finds the GUI entry called `name` (the lookup of 0x49ff10, inlined) and
// copies `text` (at most 16 characters) into its name.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_0049f930 {                // 0x15b bytes
    short unknown_0;
    char name[16];                     // +0x2
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6 (used in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

struct Data_0049f930 {
    int unknown_0;
    Entry_0049f930* entries;           // +0x4
};

struct Object_0049f930 {
    char unknown_0[0x18];
    Data_0049f930* data;               // +0x18
};

static inline int FindEntry(Entry_0049f930* entries, char* name)
{
    for (int i = 1; i < entries[0].count + 1; i++) {
        if (strncmp(entries[i].name, name, 16) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x49f930
void __stdcall SetGadgetName(Object_0049f930* obj, char* name, char* text)
{
    if (obj->data) {
        Entry_0049f930* entries = obj->data->entries;
        int index = FindEntry(entries, name);
        if (index != -1)
            lstrcpynA(entries[index].name, text, 0x11);
    }
}
