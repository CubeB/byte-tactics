// Decompiled by space-bunny-free. Names are provisional.
// Finds a gadget entry by name (the lookup of 0x4a1080, inlined), stores the
// given value in the entry's field 0x138, flags the list as changed and, when
// the value is not zero, tells the list to lay the entry out (0x4a0340).
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a1110 {               // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x138 - 0xb8];
    short f_138;                     // +0x138
    char unknown_13a[0x15b - 0x13a];
};
#pragma pack(pop)

struct Holder_004a1110 {
    char unknown_0[4];
    Entry_004a1110* entries;         // +0x04
};

#pragma pack(push, 1)
struct Dialog {
    char unknown_0[0x18];
    Holder_004a1110* holder;         // +0x18
    char unknown_1c[0xcca - 0x1c];
    int f_cca;                       // +0xcca
};
#pragma pack(pop)

static inline int FindEntry(Entry_004a1110* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

void __stdcall FUN_004a0340(Dialog* obj, int index);

// FUNCTION: 0x4a1110
int __stdcall SetGadgetStatusByName(Dialog* obj, char* name, int value)
{
    Entry_004a1110* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1) {
        entries[i].f_138 = value;
        obj->f_cca = 1;
        if (value) {
            FUN_004a0340(obj, i);
        }
        return 1;
    }
    return 0;
}
