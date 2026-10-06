// Decompiled by Opus. Names are provisional.

class Class_004895c0;

#pragma pack(push, 2)
struct Owner_004895c0 {
    char unknown_0[0xa2];
    Class_004895c0* head;              // +0xa2
    short flag;                        // +0xa6
};
#pragma pack(pop)

class Class_004895c0 {
public:
    Owner_004895c0* owner;             // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    Class_004895c0(Owner_004895c0* o, int v);
    virtual ~Class_004895c0()
    {
        if (owner != 0) {
            Class_004895c0** pp = &owner->head;
            while (*pp != this)
                pp = &(*pp)->next;
            *pp = next;
            owner = 0;
            next = 0;
        }
    }
    void SetUnit(Owner_004895c0* o);
};

// A link that registers itself in its owner's list (head at +0xa2) when the
// owner is set and its flag at +0xa6 is non-zero. The only vtable slot is
// the scalar deleting destructor (0x489600), which unlinks it again.
// FUNCTION: 0x4895c0
Class_004895c0::Class_004895c0(Owner_004895c0* o, int v)
    : value(v)
{
    if (o != 0 && o->flag != 0) {
        owner = o;
        next = o->head;
        o->head = this;
        return;
    }
    owner = 0;
    next = 0;
}

// The compiler-generated scalar deleting destructor of Class_004895c0
// (vtable 0x4fd754, constructor 0x4895c0). The destructor (out of line at
// 0x489650) is inlined here: it unlinks the object from its owner's list
// (head at +0xa2) and clears the link.
// FUNCTION: 0x489600 ??_GClass_004895c0@@UAEPAXI@Z

// Moves the link to a new owner: unlinks it from the current owner's list
// (as the destructor 0x489650 does), then links it into `o`'s list (as the
// constructor 0x4895c0 does) when `o` is set and its flag is non-zero.
// FUNCTION: 0x489690
void Class_004895c0::SetUnit(Owner_004895c0* o)
{
    if (owner != 0) {
        Class_004895c0** pp = &owner->head;
        while (*pp != this)
            pp = &(*pp)->next;
        *pp = next;
        owner = 0;
        next = 0;
    }
    if (o != 0 && o->flag != 0) {
        owner = o;
        next = o->head;
        o->head = this;
        return;
    }
    owner = 0;
    next = 0;
}
