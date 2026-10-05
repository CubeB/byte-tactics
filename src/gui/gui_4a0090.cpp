// Decompiled by space-bunny-free. Names are provisional.
// Localises the key of the currently selected GUI entry and writes the result
// into the help text field of the entry called "HELPTEXT", then marks the
// object changed.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0090 {              // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0x33 - 0x12];
    char text[0xb6 - 0x33];          // +0x33, the string to localise
    short count;                     // +0xb6, entry count in entry 0, and the
                                     // start of the help text in the others
    char unknown_b8[0x15b - 0xb8];
};

struct Object_004a0090 {
    char unknown_0[0x18];
    struct Data_004a0090* data;      // +0x18
    char unknown_1c[0x68 - 0x1c];
    int index;                       // +0x68
    int used;                        // +0x6c
    char unknown_70[0xcca - 0x70];
    int changed;                     // +0xcca
};
#pragma pack(pop)

struct Data_004a0090 {
    int unknown_0;
    Entry_004a0090* entries;         // +0x4
};

extern char DAT_005119b8[];

char* __stdcall FUN_004c5740(char* text);

static inline int FindEntry(Entry_004a0090* entries, char* name)
{
    for (int i = 1; i < entries[0].count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a0090
void __stdcall UpdateHelpText(Object_004a0090* obj)
{
    char* text = DAT_005119b8;
    if (obj->index != -1) {
        obj->used = obj->index;
        text = obj->data->entries[obj->index].text;
    }
    int found = FindEntry(obj->data->entries, "HELPTEXT");
    if (found != -1) {
        // re-read the entry pointer here: the original reloads it after the call
        strcpy((char*)&obj->data->entries[found].count, FUN_004c5740(text));
        obj->changed = 1;
    }
}
