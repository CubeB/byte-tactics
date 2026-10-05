// Decompiled by Opus. Names are provisional.
// The out-of-line destructor of Class_004895c0 (vtable 0x4fd754): unlinks
// the object from its owner's list (head at +0xa2) and clears the link.
// Its ??_G (0x489600) inlines the same body. Callers already call it as
// UnitRef::FUN_00489650, so it is written as that method, which runs
// the real destructor non-virtually.

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

class UnitRef {
public:
    void FUN_00489650();
};

// FUNCTION: 0x489650
void UnitRef::FUN_00489650()
{
    ((Class_004895c0*)this)->Class_004895c0::~Class_004895c0();
}
