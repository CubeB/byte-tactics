// Decompiled by Opus. Names are provisional.
// When the list has a handler at +0x10, forwards everything to
// FUN_00459200; otherwise calls FUN_004584d0 for every flagged 0x36-byte
// entry, last to first.

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Owner_00458430 {
    char unknown_0[0xff];
    char kind;                         // +0xff
};

#pragma pack(push, 1)
struct Entry_00458430 {
    int field_0;                       // +0x0
    char unknown_4[0x22 - 0x4];
    void* field_22;                    // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned char flags;               // +0x28
    char unknown_29[0x36 - 0x29];
};

struct List_00458430 {
    int count;                         // +0x0
    char unknown_4[0xc - 0x4];
    Owner_00458430* owner;             // +0xc
    int field_10;                      // +0x10
    char unknown_14[0x22 - 0x14];
    Entry_00458430 entries[1];         // +0x22
};
#pragma pack(pop)

class Class_00459200 {
public:
    void FUN_00459200(int param_1, List_00458430* list, Vec3 v, int param_6);
};

class Class_004584d0 {
public:
    void FUN_004584d0(List_00458430* list, int param_1, Vec3* v, int field_0, void* field_22, char kind, int param_6);
};

class Class_00458430 {
public:
    void FUN_00458430(int param_1, List_00458430* list, Vec3 v, int param_6);
};

// FUNCTION: 0x458430
void Class_00458430::FUN_00458430(int param_1, List_00458430* list, Vec3 v, int param_6)
{
    if (list->field_10 != 0) {
        ((Class_00459200*)this)->FUN_00459200(param_1, list, v, param_6);
        return;
    }
    for (int i = list->count - 1; i >= 0; i--) {
        if (list->entries[i].flags & 1) {
            ((Class_004584d0*)this)->FUN_004584d0(list, param_1, &v, list->entries[i].field_0,
                                                  list->entries[i].field_22, list->owner->kind, param_6);
        }
    }
}
