// Decompiled by Haiku. Names are provisional.
// Slot 0 of the unit visitor at +0xc of the "kill all mobile units" victory
// condition (visitor vtable 0x4fd920, driven by 0x48ec20): counts the units
// whose first field is set and returns whether at most one has been seen.

struct Unit {
    int field_0;                         // +0x0
};

class Condition_0048ec20 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048ec20 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

// This method overrides the visitor's slot, so `this` is the visitor
// subobject (+0xc) and count sits at +0x4 from it.
class VictoryKillAllMobileUnits : public Condition_0048ec20, public UnitVisitor_0048ec20 {
public:
    int count;                           // +0x10
    virtual int VisitUnit(Unit* unit);
};

// FUNCTION: 0x48ec00
int VictoryKillAllMobileUnits::VisitUnit(Unit* unit)
{
    if (unit->field_0 != 0) {
        count++;
    }
    return count <= 1;
}
