// Decompiled by Opus. Names are provisional.

// Mission victory/defeat condition (6 virtual slots).
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_0048ff40() { satisfied = celebrated = 0; }
    virtual int FUN_0048f7e0() = 0;      // IsSatisfied
    virtual void FUN_0048ea10();         // Slot1
    virtual void FUN_0048ea20();         // Slot2
    virtual void FUN_0048ea30();         // Slot3
    virtual void FUN_0048f840(void* file) = 0;   // Save
    virtual void FUN_0048f880(void* file) = 0;   // Load
};

// Secondary interface of a condition that watches events.
class Listener_0048ff40 {
public:
    virtual void FUN_0048f790(void* event) = 0;
};

// DefeatCondition_AllUnitsKilled.
class Class_0048f840 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual int FUN_0048f7e0();
    virtual void FUN_0048f840(void* file);   // Save
    virtual void FUN_0048f880(void* file);   // Load
    virtual void FUN_0048f790(void* event);
};

class Class_0048ff40 {
public:
    Condition_0048ff40* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_0048ff40* defeat[16];      // +0x44
    int defeatCount;                     // +0x84

    int FUN_0048ff40();
};

// FUNCTION: 0x48ff40
int Class_0048ff40::FUN_0048ff40()
{
    if (defeatCount == 0) {
        defeat[defeatCount] = new Class_0048f840;
        defeatCount++;
    }
    for (int i = 0; i < defeatCount; i++) {
        if (defeat[i]->FUN_0048f7e0())
            return 1;
    }
    return 0;
}
