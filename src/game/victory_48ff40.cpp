// Decompiled by Opus. Names are provisional.

struct Unit;
class Class_004b4560;

// Mission victory/defeat condition (6 virtual slots).
class MissionCondition {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    MissionCondition() { satisfied = celebrated = 0; }
    virtual int IsSatisfied();           // IsSatisfied
    virtual void OnUnitDied(Unit* unit);     // Slot1
    virtual void OnUnitCaptured(Unit* unit);  // Slot2
    virtual void OnUnitCreated(Unit* unit);  // Slot3
    virtual void SaveState(Class_004b4560* file) = 0;      // Save
    virtual void LoadState(Class_004b4560* file) = 0;      // Load
};

// Secondary interface of a condition that visits units.
class Listener_0048ff40 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

// DefeatCondition_AllUnitsKilled.
class DefeatAllUnitsKilled : public MissionCondition, public Listener_0048ff40 {
public:
    virtual int IsSatisfied();
    virtual void SaveState(Class_004b4560* file);      // Save
    virtual void LoadState(Class_004b4560* file);      // Load
    virtual int VisitUnit(Unit* unit);
};

class MissionConditions {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84

    int AnyDefeatConditionMet();
};

// FUNCTION: 0x48ff40
int MissionConditions::AnyDefeatConditionMet()
{
    if (defeatCount == 0) {
        defeat[defeatCount] = new DefeatAllUnitsKilled;
        defeatCount++;
    }
    for (int i = 0; i < defeatCount; i++) {
        if (defeat[i]->IsSatisfied())
            return 1;
    }
    return 0;
}
