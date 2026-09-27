// Decompiled by Opus. Names are provisional.

struct Parent_0043b730;

struct Vec_0043a020 {
    int x, y, z;
};

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char type;                // +0x4
    char unknown_5[0xe - 0x5];
    Parent_0043b730* parent;           // +0xe
    char unknown_12[0x42 - 0x12];
    unsigned int flags;                // +0x42
    int field_46;                      // +0x46
    Class_0043a1f0* next;              // +0x4a
    char unknown_4e[0x56 - 0x4e];

    Class_0043a1f0(unsigned char type, int a, Vec_0043a020* b, int c, int d, int e);
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Parent_0043b730 {
    char unknown_0[0x5c];
    Class_0043a1f0* first;             // +0x5c
    Class_0043a1f0* firstTop;          // +0x60, for children with flag 0x40000
    char unknown_64[0x6a - 0x64];
    Vec_0043a020 field_6a;             // +0x6a
};
#pragma pack(pop)

// Links `child` into the parent's list in front of `before` (see 0x43b730.cpp).
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

// FUNCTION: 0x43a020
void __stdcall FUN_0043a020(Parent_0043b730* p, Class_0043a1f0* item)
{
    int add = 1;
    for (Class_0043a1f0* c = p->first; c != 0; c = c->next) {
        if (c->flags & 0x8000) {
            add = 0;
            break;
        }
    }
    if (add) {
        Class_0043a1f0* child = new Class_0043a1f0(item->type, 0, &p->field_6a, 0, 0, 0);
        InsertBefore(p, child, 0);
    }
    item->flags |= 0x8000;
}
