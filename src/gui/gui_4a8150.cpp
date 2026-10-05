// Decompiled by Opus. Names are provisional.
// Appends a cleared entry of the given type to the GUI entry list (entry 0
// holds the count) and returns its index.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a8150 {                // 0x15b bytes
    unsigned char type;                // +0x0
    char unknown_1[0x29 - 0x1];
    unsigned char field_29;            // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

struct Holder_004a8150 {
    char unknown_0[4];
    Entry_004a8150* entries;           // +0x4
};

struct Class_004a8150 {
    char unknown_0[0x18];
    Holder_004a8150* holder;           // +0x18
};

// FUNCTION: 0x4a8150
int __stdcall AddGadgetEntry(Class_004a8150* obj, unsigned char type)
{
    Entry_004a8150* entries = obj->holder->entries;
    entries->count++;
    Entry_004a8150* e = &entries[entries->count];
    memset(e, 0, sizeof(Entry_004a8150));
    e->field_29 = 1;
    e->type = type;
    return entries->count;
}
