// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Finds the GUI layout entry whose name matches `name`. The entry table holds
// 0x15b-byte entries whose entry 0 stores the entry count as a short at +0xb6;
// entries are searched from 1. A missing entry is reported and then treated as
// a null entry. The caller supplies an array of item pointers and its count:
// the entry gets the array, the count, and the index of the first item that
// still fits in the entry's field +0x19 when the item heights are summed from
// the bottom of the array. Item heights are the unsigned short at offset +2 of
// the object found through each item's field +0x28.
//
// The `field_c0` store must be written before the `field_c6` store: that makes
// MSVC load the count into ecx before the item array into edx, and keeps the
// rest of the tail's stores in the original's order. The loop is written with
// the counter and pointer decrements inside the body (and no for-increment):
// with `for (...; j--, p--)` MSVC computes the initial field_be in the wrong
// register, and with `for (j = count - 1; j >= 0; j--)` it rotates the loop
// test and keeps the sign flag instead of comparing with -1.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a36a0 {                // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short field_19;                    // +0x19
    int flags_1b;                      // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int field_c6;                      // +0xc6
    char unknown_ca[0x15b - 0xca];
};
#pragma pack(pop)

struct Table_004a36a0 {
    char unknown_0[4];
    Entry_004a36a0* entries;           // +0x4
};

struct Item_004a36a0 {
    char unknown_0[0x28];
    int field_28;                      // +0x28
};

void __stdcall FatalError(char* path);

static inline int FindEntry(Entry_004a36a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a36a0
void __stdcall SetGadgetItems(Table_004a36a0* table, char* name, int* items, int count)
{
    Entry_004a36a0* entries = table->entries;
    int i = FindEntry(entries, name);
    Entry_004a36a0* e;
    if (i != -1) {
        e = &entries[i];
    } else {
        FatalError("Error in GUI layout");
        e = 0;
    }
    e->field_c0 = (short)count;
    e->field_c6 = (int)items;
    int v = e->field_19;
    e->field_bc = 0;
    e->field_ba = 0;
    e->flags_1b |= 0x20;
    int* p = &items[count - 1];
    e->field_be = (short)(count - 1);
    for (int j = count - 1; j > -1; ) {
        Item_004a36a0* item = (Item_004a36a0*)*p;
        unsigned short w = *(unsigned short*)(item->field_28 + 2);
        v -= w;
        if (v < 0) {
            break;
        }
        e->field_be = (short)j;
        j--;
        p--;
    }
}
