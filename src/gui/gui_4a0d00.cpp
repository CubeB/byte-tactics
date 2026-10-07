// Decompiled by space-bunny-free. Names are provisional.
// Looks a gadget entry up by name (the FindEntry of 0x4a0c70 and 0x4a0bf0) and
// returns the text at entry + 0xb6, but only for entry types 1, 3 and 5 (the
// type byte at +0, read through a switch). With a non-zero third argument the
// text is copied there as well; the return value is the text either way, 0 when
// the entry is missing or of another type.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0d00 {                // 0x15b bytes
    unsigned char type;               // +0x0
    char unknown_1[0x2 - 0x1];
    char name[0x10];                  // +0x2
    char unknown_12[0xb6 - 0x12];
    union {
        short count;                  // +0xb6 (entry 0 only)
        char desc[0x15b - 0xb6];      // the string returned for entries 1..n
    } u;
};
#pragma pack(pop)

struct Data_004a0d00 {
    int unknown_0;
    Entry_004a0d00* entries;           // +0x4
};

struct Object_004a0d00 {
    char unknown_0[0x18];
    Data_004a0d00* data;               // +0x18
};

void __stdcall FatalError(char* path);

static inline int FindEntry(Entry_004a0d00* entries, char* name)
{
    for (int i = 1; i < entries[0].u.count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a0d00
char* __stdcall GetGadgetText(Object_004a0d00* obj, char* name, char* buf)
{
    char* desc = 0;
    if (obj->data == 0) {
        FatalError("Internal error");
    }
    Entry_004a0d00* entries = obj->data->entries;
    int index = FindEntry(entries, name);
    if (index != -1) {
        switch (entries[index].type) {
        case 1:
        case 3:
        case 5:
            desc = entries[index].u.desc;
            break;
        }
        if (desc != 0 && buf != 0) {
            strcpy(buf, desc);
        }
    }
    // Single return at the end: keeps the local in memory.
    return desc;
}
