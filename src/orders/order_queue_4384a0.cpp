// Decompiled by space-bunny-free. Names are provisional.
// If the owner has a count at +0x86, drop every object without flag 4 from its
// list at +0x5c (marking all but the head 0x10000 first, as DeleteOrders does),
// then build a "BECARRIED" object and insert it at the head of the list its
// own flag 0x40000 selects.

struct Parent_004384a0;

class Class_00438760 {
public:
    unsigned char index;
    char unknown_1[3];
    Class_00438760(const char*);
};

#pragma pack(push, 1)
class Class_0043a1f0 {
public:
    void* vtable;                      // +0x0
    unsigned char kind;                // +0x4
    char unknown_5[0xe - 5];
    Parent_004384a0* parent;           // +0xe
    char unknown_12[0x42 - 0x12];
    unsigned int flags;                // +0x42
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;              // +0x4a
    char unknown_4e[0x56 - 0x4e];

    Class_0043a1f0(Class_00438760, int, void*, int, int, int);
    ~Class_0043a1f0();
};

struct Parent_004384a0 {
    char unknown_0[0x5c];
    Class_0043a1f0* first;             // +0x5c
    Class_0043a1f0* firstTop;          // +0x60, for children with flag 0x40000
    char unknown_64[0x86 - 0x64];
    int field_86;                      // +0x86
};
#pragma pack(pop)

// Links `child` into the owner's list in front of `before`.
static inline void InsertBefore(Parent_004384a0* p, Class_0043a1f0* child, Class_0043a1f0* before)
{
    Class_0043a1f0** link = (child->flags & 0x40000) ? &p->firstTop : &p->first;
    while (*link != before) {
        link = &(*link)->next;
    }
    *link = child;
    child->parent = p;
    child->next = before;
    if (before != 0) {
        child->flags |= before->flags & 0x4000;
    }
}

// FUNCTION: 0x4384a0
void __stdcall AddBeCarriedOrder(Parent_004384a0* p)
{
    if (p->field_86) {
        Class_0043a1f0* first = p->first;
        Class_0043a1f0** pp = &p->first;
        Class_0043a1f0* node;
        while ((node = *pp) != 0) {
            if (node->flags & 4) {
                pp = &node->next;
            } else {
                *pp = node->next;
                if (node != first) {
                    node->flags |= 0x10000;
                }
                delete node;
            }
        }
        Class_0043a1f0* child =
            new Class_0043a1f0("BECARRIED", p->field_86, 0, 0, 0, 0);
        InsertBefore(p, child, (child->flags & 0x40000) ? p->firstTop : p->first);
    }
}
