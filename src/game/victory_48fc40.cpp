// Decompiled by Sonnet. Names are provisional.
// Slot 0 of the unit visitor at +0xc of the "any unit passes Z" defeat
// condition (visitor vtable 0x4fd768): marks the condition met when a unit
// is within 2 of the target Z, and returns whether to keep visiting.
#include <stdlib.h>

struct Unit {
    char unknown_0[0x78];
    short value;   // +0x78
};

class Condition_0048fc70 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048fc70 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

// This method overrides the visitor's slot, so `this` is the visitor
// subobject (+0xc) and satisfied sits at -8.
class DefeatAnyUnitPassesZ : public Condition_0048fc70, public UnitVisitor_0048fc70 {
public:
    int field_10;                        // +0x10
    virtual int VisitUnit(Unit* unit);
};

// FUNCTION: 0x48fc40
int DefeatAnyUnitPassesZ::VisitUnit(Unit* unit)
{
    int diff = abs((int)unit->value - field_10);

    if (diff <= 2) {
        satisfied = 1;
    }
    return satisfied == 0;
}
