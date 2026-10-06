// Decompiled by Haiku. Names are provisional.

struct Unit;

class MissionCondition {
public:
    virtual int IsSatisfied();           // IsSatisfied
    virtual void OnUnitDied(Unit* unit);     // Slot1
    virtual void OnUnitCaptured(Unit* unit);  // Slot2
    virtual void OnUnitCreated(Unit* unit);  // Slot3

    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
};

// The victory/defeat condition base's IsSatisfied (slot 0 of the condition
// vtables whose class does not override it).
// FUNCTION: 0x48ea00
int MissionCondition::IsSatisfied()
{
    return satisfied;
}

// The victory/defeat condition base's slot 1, called with a unit: does
// nothing unless a condition overrides it.
// FUNCTION: 0x48ea10
void MissionCondition::OnUnitDied(Unit*)
{
}

// The victory/defeat condition base's slot 2, called with a unit: does
// nothing unless a condition overrides it.
// FUNCTION: 0x48ea20
void MissionCondition::OnUnitCaptured(Unit*)
{
}

// The victory/defeat condition base's slot 3, which takes one pointer (taken
// to be a unit, like slots 1 and 2): no condition overrides it.
// FUNCTION: 0x48ea30
void MissionCondition::OnUnitCreated(Unit*)
{
}
