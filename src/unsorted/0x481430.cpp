// Decompiled by Opus. Names are provisional.
// Returns whether the list at +0x8a of the object two links down holds an
// item with the given id.

#pragma pack(push, 1)
struct Item_00481430 {
    char unknown_0[0x8e];
    Item_00481430* next;               // +0x8e
    char unknown_92[0xa8 - 0x92];
    unsigned short id;                 // +0xa8
};

struct Owner_00481430 {
    char unknown_0[0x8a];
    Item_00481430* items;              // +0x8a
};
#pragma pack(pop)

struct Link_00481430 {
    char unknown_0[0xc];
    Owner_00481430* owner;             // +0xc
};

class Class_00481430 {
public:
    char unknown_0[0x540];
    Link_00481430* link;               // +0x540

    int FUN_00481430(int id);
};

// FUNCTION: 0x481430
int Class_00481430::FUN_00481430(int id)
{
    Item_00481430* p = link->owner->items;
    while (p) {
        if (p->id == id)
            return 1;
        p = p->next;
    }
    return 0;
}
