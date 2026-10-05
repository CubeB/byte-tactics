// Decompiled by Opus. Names are provisional.
// Appends a node to the end of the owner's list that its flag 0x40000
// selects (+0x60 when set, +0x5c otherwise); compare 0x43acb0.

struct Owner_0043ad10;

#pragma pack(push, 1)
struct Node_0043ad10 {
    char unknown_0[0xe];
    Owner_0043ad10* owner;          // +0x0e
    char unknown_12[0x30];
    unsigned int flags;             // +0x42
    char unknown_46[4];
    Node_0043ad10* next;            // +0x4a
};

struct Owner_0043ad10 {
    char unknown_0[0x5c];
    Node_0043ad10* list_a;          // +0x5c
    Node_0043ad10* list_b;          // +0x60
};
#pragma pack(pop)

// FUNCTION: 0x43ad10
void __stdcall FUN_0043ad10(Owner_0043ad10* owner, Node_0043ad10* node)
{
    Node_0043ad10** link = (node->flags & 0x40000) ? &owner->list_b : &owner->list_a;
    for (Node_0043ad10* n = *link; n != 0; n = n->next)
        link = &n->next;
    *link = node;
    node->owner = owner;
    node->next = 0;
}
