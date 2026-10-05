// Decompiled by space-bunny-free. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a07d0 {                // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0xb6 - 0x12];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0x15b - 0xb6];       // +0xb6
    } u;
};
struct Data_004a07d0 {
    int unknown_0;
    Entry_004a07d0* entries;           // +0x04
};

struct Object_004a07d0 {
    char unknown_0[0x18];
    Data_004a07d0* data;               // +0x18
    char unknown_1c[0xcca - 0x1c];
    int changed;                       // +0xcca
};
#pragma pack(pop)

void __stdcall FUN_004a05e0(Object_004a07d0* obj, int index);

static inline int FindEntry(Entry_004a07d0* entries, char* name)
{
    for (int i = 1; i < entries[0].u.count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a07d0
void __stdcall FUN_004a07d0(Object_004a07d0* obj, char* name, char* text)
{
    if (obj->data) {
        Entry_004a07d0* entries = obj->data->entries;
        int index = FindEntry(entries, name);
        if (index != -1) {
            strncpy(entries[index].u.text, text, 0x80);
            obj->changed = 1;
            FUN_004a05e0(obj, index);
        }
    }
}
