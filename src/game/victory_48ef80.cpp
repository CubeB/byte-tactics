// Decompiled by Haiku. Names are provisional.
// Slot 0 of the unit visitor at +0xc of the "kill all of type" victory
// condition (visitor vtable 0x4fd8c8, driven by 0x48efb0): counts the units
// of the condition's type and returns whether at most one has been seen.

struct Unit {
    char unknown_0[0xa6];
    short field_a6;                      // +0xa6
};

class Condition_0048efb0 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048efb0 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

// This method overrides the visitor's slot, so `this` is the visitor
// subobject (+0xc) and id and count sit at +0x24 and +0x26 from it.
#pragma pack(push, 2)
class VictoryKillAllOfType : public Condition_0048efb0, public UnitVisitor_0048efb0 {
public:
    char name[0x20];                     // +0x10
    short id;                            // +0x30
    int count;                           // +0x32
    virtual int VisitUnit(Unit* unit);
};
#pragma pack(pop)

// FUNCTION: 0x48ef80
int VictoryKillAllOfType::VisitUnit(Unit* unit)
{
    if (unit->field_a6 == id) {
        count++;
    }
    return count <= 1;
}
