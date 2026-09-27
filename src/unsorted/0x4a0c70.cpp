// Decompiled by Opus. Names are provisional.
// Finds a gadget by name (as in 0x4a0570), stores a value into it and marks
// the object as changed.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0c70 {                // 0x15b bytes
    short unknown_0;
    char name[16];                     // +0x2
    char unknown_12[0x1f - 0x12];
    int value;                         // +0x1f
    char unknown_23[0xb6 - 0x23];
    short count;                       // +0xb6 (used in entry 0)
    char unknown_b8[0x15b - 0xb8];
};

struct Object_004a0c70 {
    char unknown_0[0x18];
    struct Data_004a0c70* data;        // +0x18
    char unknown_1c[0xcca - 0x1c];
    int changed;                       // +0xcca
};
#pragma pack(pop)

struct Data_004a0c70 {
    int unknown_0;
    Entry_004a0c70* entries;           // +0x4
};

static inline int FindEntry(Entry_004a0c70* entries, char* name)
{
    for (int i = 1; i < entries[0].count + 1; i++) {
        if (strncmp(entries[i].name, name, 16) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a0c70
void __stdcall FUN_004a0c70(Object_004a0c70* obj, char* name, int value)
{
    if (obj->data) {
        Entry_004a0c70* entries = obj->data->entries;
        int index = FindEntry(entries, name);
        if (index != -1) {
            entries[index].value = value;
            obj->changed = 1;
        }
    }
}
