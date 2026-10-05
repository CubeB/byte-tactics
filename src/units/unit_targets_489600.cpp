// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_004895c0
// (vtable 0x4fd754, constructor 0x4895c0). The destructor (out of line at
// 0x489650) is inlined here: it unlinks the object from its owner's list
// (head at +0xa2) and clears the link.
//
// The static object below exists only to make the compiler emit the vtable
// (and with it this COMDAT) here.

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
};

// FUNCTION: 0x489600 ??_GClass_004895c0@@UAEPAXI@Z
static Class_004895c0 s_obj(0, 0);
