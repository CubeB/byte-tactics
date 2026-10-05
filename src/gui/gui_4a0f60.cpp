// Decompiled by Opus. Names are provisional.
// Looks up the gadget entry by name (the lookup of 0x49fdf0, inlined) and
// returns its field 0x137 when the entry's state is 1, otherwise -1 (compare
// 0x4a0ff0).
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0f60 {
    char state;                        // +0x0
    char unknown_1[1];
    char name[0x10];                   // +0x2
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0x137 - 0xb8];
    unsigned char field_137;           // +0x137
    char unknown_138[0x15b - 0x138];
};
#pragma pack(pop)

struct Table_004a0f60 {
    char unknown_0[4];
    Entry_004a0f60* entries;           // +0x4
};

struct Obj_004a0f60 {
    char unknown_0[0x18];
    Table_004a0f60* table;             // +0x18
};

static inline int FindEntry(Entry_004a0f60* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a0f60
int __stdcall GetButtonStageByName(Obj_004a0f60* obj, char* name)
{
    Entry_004a0f60* entries = obj->table->entries;
    int i = FindEntry(entries, name);
    if (i != -1 && entries[i].state == 1) {
        return entries[i].field_137;
    }
    return -1;
}
