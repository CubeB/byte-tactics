// Decompiled by Opus. Names are provisional.
// Victory counterpart of 0x48ff40: with no victory condition set, adds the
// default "destroy all units" condition (vtable 0x4fd948), then reports
// whether every victory condition is satisfied.

// Mission victory/defeat condition (see 0x48ff40.cpp). The slots carry the
// names their functions already have for the "destroy all units" victory
// condition, so its vtable matches the original's names.
class Condition_0048fed0 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_0048fed0() { satisfied = celebrated = 0; }
    virtual int FUN_0048eb40() = 0;              // IsSatisfied
    virtual void FUN_0048ea10();                 // Slot1
    virtual void FUN_0048ea20();                 // Slot2
    virtual void FUN_0048ea30();                 // Slot3
    virtual void FUN_0048eb80(void* file) = 0;   // Save
    virtual void FUN_0048ebc0(void* file) = 0;   // Load
};

// VictoryCondition_DestroyAllUnits.
class Class_0048eb40 : public Condition_0048fed0 {
public:
    virtual int FUN_0048eb40();
    virtual void FUN_0048eb80(void* file);
    virtual void FUN_0048ebc0(void* file);
};

class Class_0048ff40 {
public:
    Condition_0048fed0* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_0048fed0* defeat[16];      // +0x44
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
        if (!victory[i]->FUN_0048eb40())
            return 0;
    }
    return 1;
}
