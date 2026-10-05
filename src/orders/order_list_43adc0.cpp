// Decompiled by space-bunny-free, finished by Claude Sonnet 5.5. Names are provisional.
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
// 0x43ad50 does.  All three helpers are inlined.
//
// MATCH (500 of 500 bytes, Claude Sonnet 5.5 #702).  Two source changes fixed
// the two things the earlier 80.7% version got wrong; compiler state is not
// involved (the declaration-count sweep 0 to 400 and all 128 header sets stay at
// 80.7% for the old source).
//  1. The dead compare of the second loop (`cmp esi, esi` at 0x43aea5, and the
//     `or dword ptr [esi+0x42], 0x10000` behind it).  MSVC 5 folds `node != first`
//     when both are copies of the same value, but keeps it when `first` is a fresh
//     read of the list head made at the call: `Class_0043a1f0* node = n;
//     RemoveFromList(sel, node, owner->list);`.  The read is merged into the same
//     register afterwards, so no extra load or register appears.  (The `node = n`
//     copy is still needed: it is what puts 0x40000 and 0x4000 into ebx and ebp.)
//     Passing the fresh read without the copy keeps the compare but loses ebx and
//     ebp (72.4%).
//  2. The first loop's double load of owner->list (0x43ae18 and 0x43ae1b).  With
//     the helper taking the link and the head as arguments, called as
//     `PruneLoose(&owner->list, owner->list)`, and starting from `n = *link`, the
//     two loads stay separate (`mov esi, [eax+0x5c]; mov ebp, [eax+0x5c]`).  Taking
//     only the owner and reading `owner->list` twice inside, in either order, or
//     declaring the locals in any order, gives one load and a copy (`mov ebp, ...;
//     mov esi, ebp`), one byte shorter.
//  The constructor's second argument is this function's fourth parameter (the
//  `Unit*` target the callers pass), not `owner`, which is what keeps `owner` in
//  its stack slot and re-read at every use.

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

    Class_0043a1f0(int k, void* o, Vec3_0043adc0* p, int a, int b, int c);
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
static inline void PruneLoose(Class_0043a1f0** link, Class_0043a1f0* first)
{
    Class_0043a1f0* n = *link;
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
        Class_0043a1f0* node = n;
        RemoveFromList((node->flags & 0x40000) ? &owner->list2 : base, node, owner->list);
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
void __stdcall FUN_0043adc0(int kind, int remove, Owner_0043adc0* owner, void* id,
                            Vec3_0043adc0* pos, int param_6, int param_7)
{
    Class_0043a1f0* obj = new Class_0043a1f0(kind, id, pos, param_6, param_7, 0);

    if (remove == 0 && !(obj->flags & 0x40))
        PruneLoose(&owner->list, owner->list);

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
