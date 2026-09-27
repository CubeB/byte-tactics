// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Owner_0043ac60;

#pragma pack(push, 1)
struct Node_0043ac60 {
    char unknown_0[0xe];
    Owner_0043ac60* owner;          // +0x0e
    char unknown_12[0x30];
    unsigned int flags;             // +0x42
    char unknown_46[4];
    Node_0043ac60* next;            // +0x4a
};

struct Owner_0043ac60 {
    char unknown_0[0x5c];
    Node_0043ac60* list_a;          // +0x5c
    Node_0043ac60* list_b;          // +0x60
};
#pragma pack(pop)

// FUNCTION: 0x43ac60
void __stdcall FUN_0043ac60(Owner_0043ac60* owner, Node_0043ac60* node,
                            Node_0043ac60* before)
{
    Node_0043ac60** link = (node->flags & 0x40000) ? &owner->list_b
                                                   : &owner->list_a;
    while (*link != before)
        link = &(*link)->next;
    *link = node;
    node->owner = owner;
    node->next = before;
    if (before != 0)
        node->flags |= before->flags & 0x4000;
}
