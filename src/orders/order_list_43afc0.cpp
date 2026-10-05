// Decompiled by Opus. Names are provisional.
// With `remove` set, looks in the owner's list at +0x5c for an object of the
// given kind (and id, and within 0x100000 of `pos` in x and z, when those are
// given) and deletes it; otherwise, or when nothing matches, hands all the
// arguments on to FUN_0043adc0.
// The match test and the removal are inlined helpers; written in place,
// MSVC assigns the parameters other registers and lays out the blocks
// differently.

#pragma pack(push, 1)
class Class_0043a1f0 {
public:
    char unknown_0[0x4];
    unsigned char kind;                // +0x4
    char unknown_5[0x16 - 0x5];
    int id;                            // +0x16
    char unknown_1a[0x22 - 0x1a];
    int x;                             // +0x22
    int y;                             // +0x26
    int z;                             // +0x2a
    char unknown_2e[0x42 - 0x2e];
    unsigned int flags;                // +0x42
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;              // +0x4a

    ~Class_0043a1f0();
};

struct Owner_0043afc0 {
    char unknown_0[0x5c];
    Class_0043a1f0* list;              // +0x5c
    Class_0043a1f0* list2;             // +0x60
};
#pragma pack(pop)

void __stdcall FUN_0043adc0(unsigned char kind, int remove, Owner_0043afc0* owner, int id, int* pos, int param_6, int param_7);

// |d| <= 0x100000
static inline int InRange(int d)
{
    return (unsigned int)(d + 0x100000) <= 0x200000;
}

static inline int Matches(Class_0043a1f0* node, unsigned char kind, int id, int* pos)
{
    return kind == node->kind && (id == 0 || id == node->id) &&
           (!pos || (InRange(pos[0] - node->x) && InRange(pos[2] - node->z)));
}

// Unlinks `node` from the list its flag 0x40000 selects and deletes it.
static inline void RemoveNode(Owner_0043afc0* owner, Class_0043a1f0* node, Class_0043a1f0* first)
{
    Class_0043a1f0** link = &owner->list;
    if (node->flags & 0x40000) {
        link = &owner->list2;
    }
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

// FUNCTION: 0x43afc0
void __stdcall FUN_0043afc0(unsigned char kind, int remove, Owner_0043afc0* owner, int id, int* pos, int param_6, int param_7)
{
    if (remove) {
        Class_0043a1f0* first = owner->list;
        for (Class_0043a1f0* node = first; node != 0; node = node->next) {
            if (Matches(node, kind, id, pos)) {
                RemoveNode(owner, node, first);
                return;
            }
        }
    }
    FUN_0043adc0(kind, remove, owner, id, pos, param_6, param_7);
}
