// Decompiled by space-bunny-free. Names are provisional.
// Creates a new Class_0043a1f0 node of the given kind and links it into the
// owner's two lists (+0x60 when the node has flag 0x40000, otherwise +0x5c),
// after pruning them.  With `remove` clear and the new node lacking flag 0x40,
// every node of the +0x5c list that has no flag 4 is deleted, all but the
// first getting flag 0x10000 so that its destructor does not unlink it again.
// Then, while the new node has no flag 0x40000, the leading run of nodes
// carrying flag 0x4000 is deleted, each from the list its own flag 0x40000
// selects.  The new node is marked in use (flag 1, plus flag 0x2000 when
// `remove` is clear) and linked in: in front of the old list head, taking over
// its flag 0x4000, through the code of 0x43acb0 when the new node has flag
// 0x20 or 0x40000, otherwise right after the node that carries flag 0x1000, as
// 0x43ad50 does.  All three helpers are inlined, and the dead `node != first`
// compare in the second loop is the original passing the node itself as the
// list head to the shared unlink helper.
//
// NOT MATCHING (56.6%, 471 of 500 bytes).  The whole difference is one
// register-allocation decision: the original keeps the `owner` parameter in
// its stack slot and re-reads it at every use (six `mov reg, [esp+0x1c]`:
// 0x43ade2, 0x43ae14, 0x43ae5f, 0x43aec8, 0x43af07, 0x43af10, 0x43af8d), so
// all four callee-saved registers are free for other things: bl holds the mask
// 4 of the first loop's test (0x43ae25) and ebx/ebp hold 0x40000/0x4000 from
// 0x43ae63 on, which is why the original tests those masks with `test reg, reg`
// and stores 0x10000 with an immediate.  Every source spelling tried here
// (one flat function, `static inline` helpers, owner member functions, the
// owner passed by address or through a wrapper struct, the real 0x43acb0 and
// 0x43ad50 defined above this function and called) makes MSVC 5 enregister
// `owner` in ebx, which cascades into the immediates, the byte tests, the
// operand order of the two flag copies and the branch polarity of the second
// loop's list selection ternary.  The structure of the code itself is confirmed
// correct: the blocks, the loops, the delete calls and all three helper bodies
// line up one for one.

#pragma pack(push, 1)

struct Owner_0043adc0;

struct Vec3_0043adc0 {
    int x, y, z;
};

class Class_0043a1f0 {
public:
    char unknown_0[0x4];
    unsigned char kind;                  // +0x4
    char unknown_5[0xe - 0x5];
    Owner_0043adc0* owner;               // +0xe
    char unknown_12[0x42 - 0x12];
    unsigned int flags;                  // +0x42
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;                // +0x4a
    char unknown_4e[0x56 - 0x4e];

    Class_0043a1f0(int k, Owner_0043adc0* o, Vec3_0043adc0* p, int a, int b, int c);
    ~Class_0043a1f0();
};

struct Owner_0043adc0 {
    char unknown_0[0x5c];
    Class_0043a1f0* list;                // +0x5c
    Class_0043a1f0* list2;               // +0x60
};
#pragma pack(pop)

// Unlinks the node `link` points at and deletes it, marking it 0x10000 (so its
// destructor does not unlink it again) unless it is the list head.
static inline void UnlinkNode(Class_0043a1f0** link, Class_0043a1f0* node, Class_0043a1f0* first)
{
    *link = node->next;
    if (node != first)
        node->flags |= 0x10000;
    delete node;
}

// Unlinks `node` from the list starting at `list` and deletes it.
static inline void RemoveFromList(Class_0043a1f0** list, Class_0043a1f0* node, Class_0043a1f0* first)
{
    Class_0043a1f0** link = list;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
        if (n == node) {
            UnlinkNode(link, node, first);
            return;
        }
        link = &n->next;
    }
}

// Drops every node of the owner's +0x5c list that has no flag 4.
static inline void PruneLoose(Owner_0043adc0* owner)
{
    Class_0043a1f0* first = owner->list;
    Class_0043a1f0* n = owner->list;
    Class_0043a1f0** link = &owner->list;
    while (n != 0) {
        if (n->flags & 4)
            link = &n->next;
        else
            UnlinkNode(link, n, first);
        n = *link;
    }
}

// Drops the leading run of nodes carrying flag 0x4000, each from the list its
// own flag 0x40000 selects.
static inline void PruneUsed(Owner_0043adc0* owner)
{
    Class_0043a1f0** base = &owner->list;
    for (Class_0043a1f0* n = owner->list; n != 0; n = owner->list) {
        if (!(n->flags & 0x4000))
            break;
        RemoveFromList((n->flags & 0x40000) ? &owner->list2 : base, n, n);
    }
}

// Puts `node` in front of the list head its flag 0x40000 selects (0x43acb0).
static inline void AddFront(Owner_0043adc0* owner, Class_0043a1f0* node)
{
    Class_0043a1f0* before = (node->flags & 0x40000) ? owner->list2 : owner->list;
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->list2 : &owner->list;
    while (*link != before)
        link = &(*link)->next;
    *link = node;
    node->owner = owner;
    node->next = before;
    if (before != 0) {
        node->flags |= before->flags & 0x4000;
        return;
    }
}

// Puts `node` right after the node carrying flag 0x1000 (0x43ad50).
static inline void AddMarked(Owner_0043adc0* owner, Class_0043a1f0* node)
{
    Class_0043a1f0** link = &owner->list;
    node->owner = owner;
    node->flags |= 0x1000;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
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

// FUNCTION: 0x43adc0
void __stdcall FUN_0043adc0(int kind, int remove, Owner_0043adc0* owner, int id,
                            Vec3_0043adc0* pos, int param_6, int param_7)
{
    Class_0043a1f0* obj = new Class_0043a1f0(kind, owner, pos, param_6, param_7, 0);

    if (remove == 0 && !(obj->flags & 0x40))
        PruneLoose(owner);

    if (!(obj->flags & 0x40000))
        PruneUsed(owner);

    obj->flags |= 1;
    if (remove == 0)
        obj->flags |= 0x2000;

    if (obj->flags & 0x40020)
        AddFront(owner, obj);
    else
        AddMarked(owner, obj);
}
