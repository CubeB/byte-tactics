// Decompiled by Opus. Names are provisional.
// A link that registers itself in its owner's list (head at +0xa2) when the
// owner is set and its flag at +0xa6 is non-zero. The only vtable slot is
// the scalar deleting destructor (0x489600), which unlinks it again.

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
    virtual ~Class_004895c0();
};

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
