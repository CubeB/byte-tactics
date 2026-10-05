// Decompiled by Opus. Names are provisional.

struct Parent_0043b730;

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0xe];
    Parent_0043b730* parent;           // +0xe
    char unknown_12[0x42 - 0x12];
    unsigned int flags;                // +0x42
    int field_46;                      // +0x46
    Class_0043a1f0* next;              // +0x4a
    char unknown_4e[0x56 - 0x4e];

    Class_0043a1f0(unsigned char type, int a, int b, int c, int d, int e);
};
#pragma pack(pop)

struct Parent_0043b730 {
    char unknown_0[0x5c];
    Class_0043a1f0* first;             // +0x5c
    Class_0043a1f0* firstTop;          // +0x60, for children with flag 0x40000
};

// Links `child` into the parent's list in front of `before`.
static inline void InsertBefore(Parent_0043b730* p, Class_0043a1f0* child, Class_0043a1f0* before)
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

// FUNCTION: 0x43b730
void __stdcall FUN_0043b730(Parent_0043b730* p, unsigned char type)
{
    Class_0043a1f0* child = new Class_0043a1f0(type, 0, 0, 0, 0, 0);
    child->flags |= 0x4000;
    InsertBefore(p, child, (child->flags & 0x40000) ? p->firstTop : p->first);
}
