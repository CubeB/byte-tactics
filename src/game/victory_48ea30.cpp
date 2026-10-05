// Decompiled by Haiku. Names are provisional.
// The victory/defeat condition base's slot 3, which takes one pointer (taken
// to be a unit, like slots 1 and 2): no condition overrides it.

struct Unit;

class MissionCondition {
public:
    virtual void OnUnitCreated(Unit* unit);  // Slot3
};

// FUNCTION: 0x48ea30
void MissionCondition::OnUnitCreated(Unit*)
{
}
