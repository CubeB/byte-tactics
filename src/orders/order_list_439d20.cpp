// Decompiled by Opus. Names are provisional.

struct Target_439d20 {
    char unknown_0[0xe4];
    unsigned short field_e4;         // +0xe4
};

struct Entry_439d20 {
    Target_439d20* target;           // +0x0
    char unknown_4[0x18];
};

#pragma pack(push, 1)
struct Node_439d20 {
    char unknown_0[0x36];
    int index;                       // +0x36
    char unknown_3a[4];
    int amount;                      // +0x3e
    unsigned int flags;              // +0x42
    char unknown_46[4];
    Node_439d20* next;               // +0x4a
};
#pragma pack(pop)


struct Owner_439d20 {
    char unknown_0[0x10];
    Entry_439d20 entries[2];         // +0x10
    char unknown_48[0x18];
    Node_439d20* nodes;              // +0x60
};

// FUNCTION: 0x439d20
int __stdcall FUN_00439d20(Owner_439d20* owner)
{
    Node_439d20* p;
    for (p = owner->nodes; p; p = p->next) {
        if (p->flags & 0x80000)
            return p->amount * 100 / owner->entries[p->index].target->field_e4;
    }
    return 0;
}
