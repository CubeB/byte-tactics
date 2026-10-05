// Decompiled by Opus. Names are provisional.
// Finds a gadget by name and sets its flag bit (see 0x4a1080 and 0x4a1200).
#include <string.h>

#pragma pack(push, 1)
struct Entry_4a1250 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x13c - 0xb8];
    unsigned short flag : 1;         // +0x13c bit 0
    unsigned short unknown_13c_1 : 15;
    char unknown_13e[0x15b - 0x13e];
};
#pragma pack(pop)

struct Holder_4a1250 {
    char unknown_0[4];
    Entry_4a1250* entries;           // +0x04
};

struct Dialog {
    char unknown_0[0x18];
    Holder_4a1250* holder;           // +0x18
};

static inline int FindEntry(Entry_4a1250* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a1250
void __stdcall FUN_004a1250(Dialog* obj, char* name, int value)
{
    Entry_4a1250* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1) {
        entries[i].flag = value;
    }
}
