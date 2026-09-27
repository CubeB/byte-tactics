// Decompiled by Opus. Names are provisional.
// Sums the amounts of the nodes flagged 0x100 with the given index over both
// of the owner's node lists (+0x5c and +0x60).

#pragma pack(push, 1)
struct Node_439d80 {
    char unknown_0[0x36];
    int index;                       // +0x36
    int amount;                      // +0x3a
    char unknown_3e[4];
    unsigned int flags;              // +0x42
    char unknown_46[4];
    Node_439d80* next;               // +0x4a
};
#pragma pack(pop)

struct Owner_439d80 {
    char unknown_0[0x5c];
    Node_439d80* nodesA;             // +0x5c
    Node_439d80* nodesB;             // +0x60
};

// FUNCTION: 0x439d80
int __stdcall FUN_00439d80(Owner_439d80* owner, int index)
{
    int total = 0;
    Node_439d80* p;
    for (p = owner->nodesA; p; p = p->next) {
        if ((p->flags & 0x100) && p->index == index)
            total += p->amount;
    }
    for (p = owner->nodesB; p; p = p->next) {
        if ((p->flags & 0x100) && p->index == index)
            total += p->amount;
    }
    return total;
}
