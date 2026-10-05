// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a0bf0 {
    char name[0x10];                   // +0x0
    char unknown_10[0x15b - 0x10];
};

struct Table_004a0bf0 {
    char unknown_0[0xb6];
    short count;                       // +0xb6
};
#pragma pack(pop)

struct Holder_004a0bf0 {
    int unknown_0;
    Table_004a0bf0* table;             // +0x4
};

struct Object_004a0bf0 {
    char unknown_0[0x18];
    Holder_004a0bf0* holder;           // +0x18
};

void __stdcall FUN_004a09c0(Object_004a0bf0* obj, int index, int param_3, int param_4);

static inline Entry_004a0bf0* Entries(Table_004a0bf0* t)
{
    return (Entry_004a0bf0*)((char*)t + 2);
}

static inline int FindEntry(Table_004a0bf0* t, char* name)
{
    for (int i = 1; i < t->count + 1; i++) {
        if (strncmp(Entries(t)[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a0bf0
void __stdcall FUN_004a0bf0(Object_004a0bf0* obj, char* name, int param_3, int param_4)
{
    if (obj->holder != 0) {
        int index = FindEntry(obj->holder->table, name);
        if (index != -1) {
            FUN_004a09c0(obj, index, param_3, param_4);
        }
    }
}
