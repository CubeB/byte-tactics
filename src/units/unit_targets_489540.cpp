// Decompiled by Sonnet. Names are provisional.

class UnitRef;

#pragma pack(push, 2)
struct Owner_00489540 {
    char unknown_0[0xa2];
    UnitRef* head;         // +0xa2
    short flag;            // +0xa6
};
#pragma pack(pop)

class UnitRef {
public:
    char unknown_0[4];
    Owner_00489540* owner;  // +4
    UnitRef* next;          // +8

    void LinkToUnit(Owner_00489540* o);
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
