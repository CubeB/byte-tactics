// Decompiled by Opus. Names are provisional.
// Finds the object's list node of the given kind; the kind table's flag
// 0x40000 selects which of the object's two lists (+0x60 or +0x5c) to search.

#pragma pack(push, 1)
struct KindEntry_00439e30 {
    char unknown_0[0x11];
    unsigned int flags;                // +0x11
    char unknown_15[0x19 - 0x15];
};

struct Node_00439e30 {
    char unknown_0[4];
    unsigned char kind;                // +0x4
    char unknown_5[0x4a - 0x5];
    Node_00439e30* next;               // +0x4a
};
#pragma pack(pop)

struct Obj_00439e30 {
    char unknown_0[0x5c];
    Node_00439e30* list;               // +0x5c
    Node_00439e30* list2;              // +0x60
};

extern KindEntry_00439e30* DAT_00512344;

// FUNCTION: 0x439e30
Node_00439e30* __stdcall FUN_00439e30(Obj_00439e30* obj, unsigned char kind)
{
    Node_00439e30* n;
    if (DAT_00512344[kind].flags & 0x40000)
        n = obj->list2;
    else
        n = obj->list;
    while (n) {
        if (n->kind == kind)
            return n;
        n = n->next;
    }
    return 0;
}
