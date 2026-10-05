// Decompiled by Opus. Names are provisional.
// Victory counterpart of 0x48ff40: with no victory condition set, adds the
// default "destroy all units" condition (vtable 0x4fd948), then reports
// whether every victory condition is satisfied.

struct Unit;
class HapiBank;

// Mission victory/defeat condition (see 0x48e010.cpp). The "destroy all
// units" victory condition's vtable names the base's defaults and the
// overrides its own files define (0x48eb40, 0x48eb80 and 0x48ebc0).
class MissionCondition {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    MissionCondition() { satisfied = celebrated = 0; }
    virtual int IsSatisfied();                   // IsSatisfied
    virtual void OnUnitDied(Unit* unit);         // Slot1
    virtual void OnUnitCaptured(Unit* unit);     // Slot2
    virtual void OnUnitCreated(Unit* unit);      // Slot3
    virtual void SaveState(HapiBank* file) = 0;            // Save
    virtual void LoadState(HapiBank* file) = 0;            // Load
};

// VictoryCondition_DestroyAllUnits.
class VictoryDestroyAllUnits : public MissionCondition {
public:
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

class MissionConditions {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84

    int AllVictoryConditionsMet();
};

// FUNCTION: 0x48fed0
int MissionConditions::AllVictoryConditionsMet()
{
    if (victoryCount == 0) {
        victory[victoryCount] = new VictoryDestroyAllUnits;
        victoryCount++;
    }
    for (int i = 0; i < victoryCount; i++) {
        if (!victory[i]->IsSatisfied())
            return 0;
    }
    return 1;
}
