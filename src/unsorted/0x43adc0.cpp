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
// 0x43ad50 does.  All three helpers are inlined.
//
// NOT MATCHING (80.7%, 488 of 500 bytes).  What is left is 12 bytes in the two
// prune loops, and both halves of it come from one dead compare.
//
// The original's second loop keeps the compare of the shared unlink helper even
// though its two arguments are the same node: 0x43aea5 is `cmp esi, esi`, and
// the `or dword ptr [esi+0x42], 0x10000` behind it (0x43aeab) can never run.
// Our source has the same two arguments (`RemoveFromList(lk, node, n)`), but the
// front end folds `node != n` away, so the compare, the branch and the `or` are
// all missing (11 bytes).  A compare that survives needs two distinct symbols
// that still land in one register: `Class_0043a1f0* f = *q; if (f != n)` does
// survive, but it reloads `*q` and costs a register, and with that extra
// register the mask registers go away again.  Every other spelling tried
// (`node = n` before the call, `*lk` as either argument, a local copy, the
// unlink written out twice) is folded.
//
// That dead compare is also what makes the rest of the second block come out
// right: because 0x10000 has a second use in the second loop, MSVC rematerialises
// it at both uses instead of giving it a register, which frees ebx in the first
// loop for the mask 4 (`mov bl, 4; test byte ptr [esi+0x42], bl` and
// `or dword ptr [esi+0x42], 0x10000`, 0x43ae25 onwards).  With the compare
// folded, 0x10000 has one short use, wins ebx, and the first loop differs in
// three instructions.  The `Class_0043a1f0* node = n;` copy in PruneUsed is
// what puts 0x40000 and 0x4000 into ebx and ebp (0x43ae63), and without it both
// stay immediates, so keep it.
//
// Two smaller points, both confirmed: the constructor's second argument is this
// function's fourth parameter (the `Unit*` target the callers pass), not
// `owner`, which is what keeps `owner` in its stack slot and re-read at all
// seven uses; and the original loads `owner->list` twice in the first loop's
// pre-header (0x43ae18 and 0x43ae1b) where we load it once and copy, which no
// spelling tried here defeats (declaration order, an inline accessor, an extra
// argument, an uninitialised local: all merge the two loads).

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
        Class_0043a1f0* node = n;
        RemoveFromList((node->flags & 0x40000) ? &owner->list2 : base, node, n);
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
