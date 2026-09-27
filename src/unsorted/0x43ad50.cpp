// Decompiled by Opus. Names are provisional.
// Marks a node with flag 0x1000 and links it into the owner's list at +0x5c
// just after the node currently carrying that mark (moving the mark to the
// new node), or at the end of the list when no node has it; compare 0x43ad10.

struct Owner_0043ad50;

#pragma pack(push, 1)
struct Node_0043ad50 {
    char unknown_0[0xe];
    Owner_0043ad50* owner;          // +0x0e
    char unknown_12[0x30];
    unsigned int flags;             // +0x42
    char unknown_46[4];
    Node_0043ad50* next;            // +0x4a
};

struct Owner_0043ad50 {
    char unknown_0[0x5c];
    Node_0043ad50* list_a;          // +0x5c
    Node_0043ad50* list_b;          // +0x60
};
#pragma pack(pop)

// FUNCTION: 0x43ad50
void __stdcall FUN_0043ad50(Owner_0043ad50* owner, Node_0043ad50* node)
{
    Node_0043ad50** link = &owner->list_a;
    node->owner = owner;
    node->flags |= 0x1000;
    for (Node_0043ad50* n = *link; n != 0; n = n->next) {
        if (n->flags & 0x1000) {
            n->flags &= ~0x1000;
            node->owner = owner;
            node->next = n->next;
            n->next = node;
            return;
        }
        link = &n->next;
    }
    node->next = 0;
    *link = node;
}
