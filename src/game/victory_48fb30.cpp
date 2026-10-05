// Decompiled by Sonnet. Names are provisional.
// Slot 0 of the unit visitor at +0xc of the "any unit passes X" defeat
// condition (visitor vtable 0x4fd788): marks the condition met when a unit
// is within 2 of the target X, and returns whether to keep visiting.
#include <stdlib.h>

struct Unit {
    char unknown_0[0x76];
    short value;   // +0x76
};

class Condition_0048fb60 {
public:
    virtual int FUN_0048ea00();          // IsSatisfied
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048fb60 {
public:
    virtual int FUN_0048f790(Unit* unit) = 0;
};

// This method overrides the visitor's slot, so `this` is the visitor
// subobject (+0xc) and satisfied sits at -8.
class Class_0048fb60 : public Condition_0048fb60, public UnitVisitor_0048fb60 {
public:
    int field_10;                        // +0x10
    virtual int FUN_0048f790(Unit* unit);
};

// FUNCTION: 0x48fb30
int Class_0048fb60::FUN_0048f790(Unit* unit)
{
    int diff = abs((int)unit->value - field_10);

    if (diff <= 2) {
        satisfied = 1;
    }
    return satisfied == 0;
}
