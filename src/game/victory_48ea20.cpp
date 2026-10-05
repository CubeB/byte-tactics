// Decompiled by Haiku. Names are provisional.
// The victory/defeat condition base's slot 2, called with a unit: does
// nothing unless a condition overrides it.

struct Unit;

class MissionCondition {
public:
    virtual void OnUnitCaptured(Unit* unit);  // Slot2
};

// FUNCTION: 0x48ea20
void MissionCondition::OnUnitCaptured(Unit*)
{
}
