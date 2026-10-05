// Decompiled by Opus. Names are provisional.
// Looks up a menu entry by name and, when found, passes its index to
// FUN_004a12e0; compare 0x4a0570 and 0x4a1530.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a14c0 {                // 0x15b bytes
    short unknown_0;
    char name[16];                     // +0x2
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6 (used in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

struct Data_004a14c0 {
    int unknown_0;
    Entry_004a14c0* entries;           // +0x4
};

struct Object_004a14c0 {
    char unknown_0[0x18];
    Data_004a14c0* data;               // +0x18
};

void __stdcall FUN_004a12e0(Object_004a14c0* obj, int index, int param_3);

static inline int FindEntry(Entry_004a14c0* entries, char* name)
{
    for (int i = 1; i < entries[0].count + 1; i++) {
        if (strncmp(entries[i].name, name, 16) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a14c0
void __stdcall FUN_004a14c0(Object_004a14c0* obj, char* name, int param_3)
{
    int index = FindEntry(obj->data->entries, name);
    if (index != -1)
        FUN_004a12e0(obj, index, param_3);
}
