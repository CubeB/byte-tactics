// Decompiled by Sonnet and Opus. Names are provisional.

class UnitRef;

#pragma pack(push, 2)
struct Owner_00489540 {
    char unknown_0[0xa2];
    UnitRef* head;         // +0xa2
    short flag;            // +0xa6
};
#pragma pack(pop)

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

class Listener_004896f0 {
public:
    virtual void Notify(int code);
};

#pragma pack(push, 2)
#pragma pack(pop)

class UnitRef {
public:
    char unknown_0[4];
    Owner_00489540* owner;          // +0x4
    UnitRef* next;                  // +0x8
    Listener_004896f0* listener;    // +0xc
    void ClearRef(void);
    void LinkToUnit(Owner_00489540* o);
    void UnlinkFromUnit();
    void FUN_00489650();
};

// FUNCTION: 0x489540
void UnitRef::LinkToUnit(Owner_00489540* o)
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

// Unlinks the link from its owner's list (head at +0xa2); the counterpart of
// 0x489540, which links it in.
// FUNCTION: 0x489580
void UnitRef::UnlinkFromUnit()
{
    if (owner != 0) {
        UnitRef** link = &owner->head;
        while (*link != this)
            link = &(*link)->next;
        *link = next;
        owner = 0;
        next = 0;
    }
}

// The out-of-line destructor of Class_004895c0 (vtable 0x4fd754): unlinks
// the object from its owner's list (head at +0xa2) and clears the link.
// Callers already call it as UnitRef::FUN_00489650, so it is written as that
// method, which runs the real destructor non-virtually.
// FUNCTION: 0x489650
void UnitRef::FUN_00489650()
{
    ((Class_004895c0*)this)->Class_004895c0::~Class_004895c0();
}

// FUNCTION: 0x4896f0
void UnitRef::ClearRef(void)
{
    Owner_00489540* saved = owner;
    if (listener)
        listener->Notify(8);
    if (owner == saved && owner) {
        UnitRef** pp = &owner->head;
        while (*pp != this)
            pp = &(*pp)->next;
        *pp = next;
        owner = 0;
        next = 0;
    }
}
