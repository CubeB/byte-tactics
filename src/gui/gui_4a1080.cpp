// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_4a1080 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x137 - 0xb8];
    char f_137;                      // +0x137
    char unknown_138[0x15b - 0x138];
};
#pragma pack(pop)

struct Holder_4a1080 {
    char unknown_0[4];
    Entry_4a1080* entries;           // +0x04
};

struct Class_004a1080 {
    char unknown_0[0x18];
    Holder_4a1080* holder;           // +0x18
};

static inline int FindEntry(Entry_4a1080* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a1080
int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, char value)
{
    Entry_4a1080* entries = obj->holder->entries;
    int i = FindEntry(entries, name);
    if (i != -1) {
        entries[i].f_137 = value;
        return 1;
    }
    return 0;
}
