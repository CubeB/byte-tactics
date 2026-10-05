// Decompiled by Opus. Names are provisional.
// Unlinks `node` from the owner's list at +0x5c (or +0x60 when the node has
// flag 0x40000) and deletes it; every node except the head of the +0x5c list
// is marked 0x10000 first. 0x439eb0 inlines the same code.

#pragma pack(push, 1)
class Class_0043a1f0 {
public:
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;              // +0x4a

    ~Class_0043a1f0();
};

struct Owner_00439f80 {
    char unknown_0[0x5c];
    Class_0043a1f0* list;              // +0x5c
    Class_0043a1f0* list2;             // +0x60
};
#pragma pack(pop)

// FUNCTION: 0x439f80
void __stdcall DeleteOrder(Owner_00439f80* owner, Class_0043a1f0* node)
{
    Class_0043a1f0* first = owner->list;
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->list2 : &owner->list;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
        if (n == node) {
            *link = node->next;
            if (node != first) {
                node->flags |= 0x10000;
            }
            delete node;
            return;
        }
        link = &n->next;
    }
}
