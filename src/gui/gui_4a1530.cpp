// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_4a1530 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x13a - 0xb8];
    char f_13a;                      // +0x13a
    char unknown_13b[0x15b - 0x13b];
};
#pragma pack(pop)

struct Holder_4a1530 {
    char unknown_0[4];
    Entry_4a1530* entries;           // +0x04
};

#pragma pack(push, 1)
struct Dialog {
    char unknown_0[0x18];
    Holder_4a1530* holder;           // +0x18
    char unknown_1c[0xcca - 0x1c];
    int f_cca;                       // +0xcca
};
#pragma pack(pop)

static inline int FindEntry(Entry_4a1530* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a1530
void __stdcall FUN_004a1530(Dialog* obj, char* name, char value)
{
    int i = FindEntry(obj->holder->entries, name);
    obj->holder->entries[i].f_13a = value;
    obj->f_cca = 1;
}
