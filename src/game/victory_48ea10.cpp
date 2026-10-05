// Decompiled by Haiku. Names are provisional.
// The victory/defeat condition base's slot 1, called with a unit: does
// nothing unless a condition overrides it.

struct Unit;

class MissionCondition {
public:
    virtual void OnUnitDied(Unit* unit);     // Slot1
};

// FUNCTION: 0x48ea10
void MissionCondition::OnUnitDied(Unit*)
{
}
