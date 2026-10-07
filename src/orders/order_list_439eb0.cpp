// Decompiled by Opus. Names are provisional.
// Deletes the objects in the owner's list at +0x5c (all of them, or only
// those without flag 4 when `all` is 0). With `all` set, also deletes every
// object of the list at +0x60, unlinking each from the list its flag 0x40000
// selects. Every deleted object except the list's head is marked 0x10000
// first.

#pragma pack(push, 1)
class Class_0043a1f0 {
public:
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;              // +0x4a

    ~Class_0043a1f0();
};

struct Owner_00439eb0 {
    char unknown_0[0x5c];
    Class_0043a1f0* list;              // +0x5c
    Class_0043a1f0* list2;             // +0x60
};
#pragma pack(pop)

// FUNCTION: 0x439eb0
void __stdcall DeleteOrders(Owner_00439eb0* owner, int all)
{
    // Read through the owner, loop through a separate link pointer: one
    // shared local merges the two loads.
    Class_0043a1f0* first = owner->list;
    Class_0043a1f0** pp = &owner->list;
    Class_0043a1f0* node;
    while ((node = *pp) != 0) {
        if (!all && (node->flags & 4)) {
            pp = &node->next;
        } else {
            *pp = node->next;
            if (node != first) {
                node->flags |= 0x10000;
            }
            delete node;
        }
    }
    if (all) {
        Class_0043a1f0** head2 = &owner->list2;
        while ((node = *head2) != 0) {
            Class_0043a1f0* first2 = owner->list;
            Class_0043a1f0** link = (node->flags & 0x40000) ? head2 : &owner->list;
            for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
                if (n == node) {
                    *link = node->next;
                    if (node != first2) {
                        node->flags |= 0x10000;
                    }
                    delete node;
                    break;
                }
                link = &n->next;
            }
        }
    }
}
