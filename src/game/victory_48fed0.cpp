// Decompiled by Opus. Names are provisional.
// Victory counterpart of 0x48ff40: with no victory condition set, adds the
// default "destroy all units" condition (vtable 0x4fd948), then reports
// whether every victory condition is satisfied.

struct Unit;
class Class_004b4560;

// Mission victory/defeat condition (see 0x48e010.cpp). The "destroy all
// units" victory condition's vtable names the base's defaults and the
// overrides its own files define (0x48eb40, 0x48eb80 and 0x48ebc0).
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_0048ff40() { satisfied = celebrated = 0; }
    virtual int FUN_0048ea00();                  // IsSatisfied
    virtual void FUN_0048ea10(Unit* unit);       // Slot1
    virtual void FUN_0048ea20(Unit* unit);       // Slot2
    virtual void FUN_0048ea30(Unit* unit);       // Slot3
    virtual void FUN_0048f840(Class_004b4560* file) = 0;   // Save
    virtual void FUN_0048f880(Class_004b4560* file) = 0;   // Load
};

// VictoryCondition_DestroyAllUnits.
class Class_0048eb40 : public Condition_0048ff40 {
public:
    virtual int FUN_0048ea00();
    virtual void FUN_0048f840(Class_004b4560* file);
    virtual void FUN_0048f880(Class_004b4560* file);
};

class Class_0048ff40 {
public:
    Condition_0048ff40* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_0048ff40* defeat[16];      // +0x44
    int defeatCount;                     // +0x84

    int FUN_0048fed0();
};

// FUNCTION: 0x48fed0
int Class_0048ff40::FUN_0048fed0()
{
    if (victoryCount == 0) {
        victory[victoryCount] = new Class_0048eb40;
        victoryCount++;
    }
    for (int i = 0; i < victoryCount; i++) {
        if (!victory[i]->FUN_0048ea00())
            return 0;
    }
    return 1;
}
