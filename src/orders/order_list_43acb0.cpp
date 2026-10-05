// Decompiled by Opus. Names are provisional.

struct Owner_0043acb0;

#pragma pack(push, 1)
struct Node_0043acb0 {
    char unknown_0[0xe];
    Owner_0043acb0* owner;          // +0x0e
    char unknown_12[0x30];
    unsigned int flags;             // +0x42
    char unknown_46[4];
    Node_0043acb0* next;            // +0x4a
};

struct Owner_0043acb0 {
    char unknown_0[0x5c];
    Node_0043acb0* list_a;          // +0x5c
    Node_0043acb0* list_b;          // +0x60
};
#pragma pack(pop)

// FUNCTION: 0x43acb0
void __stdcall AppendOrder(Owner_0043acb0* owner, Node_0043acb0* node)
{
    unsigned int which = node->flags & 0x40000;
    Node_0043acb0* before = which ? owner->list_b : owner->list_a;
    Node_0043acb0** link = which ? &owner->list_b : &owner->list_a;
    while (*link != before) {
        link = &(*link)->next;
    }
    *link = node;
    node->owner = owner;
    node->next = before;
    if (before != 0) {
        node->flags |= before->flags & 0x4000;
    }
}
