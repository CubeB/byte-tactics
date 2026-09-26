// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_41ac90 {
    char unknown_0[0x28];
    unsigned char flags;         // +0x28
    char unknown_29[0x15b - 0x29];
};

struct Table_41ac90 {
    char unknown_0[0xb6];
    short count;                 // +0xb6
};
#pragma pack(pop)

struct Inner_41ac90 {
    char unknown_0[4];
    Table_41ac90* table;         // +4
};

struct Obj_41ac90 {
    char unknown_0[0x18];
    Inner_41ac90* inner;         // +0x18
};

short __stdcall FUN_00488b10(Entry_41ac90* entry);
void __stdcall FUN_004a1200(Obj_41ac90* obj, int index, int flag);

static inline Entry_41ac90* Entries_41ac90(Table_41ac90* t)
{
    return (Entry_41ac90*)((char*)t + 2);
}

// FUNCTION: 0x41ac90
void __stdcall FUN_0041ac90(Obj_41ac90* obj)
{
    Table_41ac90* t = obj->inner->table;
    int n = t->count;
    for (int i = 0; i < n; i++) {
        if (Entries_41ac90(t)[i].flags & 4) {
            FUN_004a1200(obj, i, FUN_00488b10(&Entries_41ac90(t)[i]) == 0);
        }
    }
}
