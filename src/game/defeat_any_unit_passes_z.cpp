// Decompiled by Sonnet and Opus. Names are provisional.

#include <stdlib.h>

struct Unit {
    char unknown_0[0x78];
    short value;                       // +0x78
    char unknown_7a[0xa6 - 0x7a];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048fc70 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

struct UnitList_0048fc70 {
    Unit* first;                       // +0x0
    Unit* last;                        // +0x4 (inclusive)

    void ForEach(UnitVisitor_0048fc70* visitor)
    {
        for (Unit* unit = first; unit <= last; unit++) {
            if (unit->field_a6 != 0) {
                int result = visitor->VisitUnit(unit);
                if (!result) {
                    break;
                }
            }
        }
    }
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1d15];
    UnitList_0048fc70 units;           // +0x1d15
};
#pragma pack(pop)

extern Game* g_game;

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
    void SetIntegerItem(const char* name, int value);
};

class Condition_0048fc70 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
};

// VisitUnit overrides the visitor's slot, so its `this` is the visitor
// subobject (+0xc) and satisfied sits at -8.
class DefeatAnyUnitPassesZ : public Condition_0048fc70, public UnitVisitor_0048fc70 {
public:
    int field_10;                        // +0x10
    virtual int IsSatisfied();
    virtual int VisitUnit(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 0 of the unit visitor at +0xc of the "any unit passes Z" defeat
// condition (visitor vtable 0x4fd768): marks the condition met when a unit
// is within 2 of the target Z, and returns whether to keep visiting.
// FUNCTION: 0x48fc40
int DefeatAnyUnitPassesZ::VisitUnit(Unit* unit)
{
    int diff = abs((int)unit->value - field_10);

    if (diff <= 2) {
        satisfied = 1;
    }
    return satisfied == 0;
}

// Slot 0 of the "any unit passes Z" defeat condition (vtable 0x4fd770,
// visitor vtable 0x4fd768 holding 0x48fc40; state saved by 0x48fcd0): until
// the condition is met, visits every live unit and returns whether it is.
// FUNCTION: 0x48fc70
int DefeatAnyUnitPassesZ::IsSatisfied()
{
    if (satisfied == 0) {
        g_game->units.ForEach(this);
    }
    return satisfied;
}

// Same shape as 0x48fbc0: writes the "any unit passes Z" defeat condition's
// state to a section.
// FUNCTION: 0x48fcd0
void DefeatAnyUnitPassesZ::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesZ");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Reads the "any unit passes Z" defeat condition's state from a section
// (same shape as 0x48f480).
// FUNCTION: 0x48fd10
void DefeatAnyUnitPassesZ::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesZ");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
