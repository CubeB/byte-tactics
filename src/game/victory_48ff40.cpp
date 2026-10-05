// Decompiled by Opus. Names are provisional.

struct Unit;
class Class_004b4560;

// Mission victory/defeat condition (6 virtual slots).
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_0048ff40() { satisfied = celebrated = 0; }
    virtual int FUN_0048ea00();          // IsSatisfied
    virtual void FUN_0048ea10(Unit* unit);   // Slot1
    virtual void FUN_0048ea20(Unit* unit);   // Slot2
    virtual void FUN_0048ea30(Unit* unit);   // Slot3
    virtual void FUN_0048f840(Class_004b4560* file) = 0;   // Save
    virtual void FUN_0048f880(Class_004b4560* file) = 0;   // Load
};

// Secondary interface of a condition that visits units.
class Listener_0048ff40 {
public:
    virtual int FUN_0048f790(Unit* unit) = 0;
};

// DefeatCondition_AllUnitsKilled.
class Class_0048f840 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual int FUN_0048ea00();
    virtual void FUN_0048f840(Class_004b4560* file);   // Save
    virtual void FUN_0048f880(Class_004b4560* file);   // Load
    virtual int FUN_0048f790(Unit* unit);
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
        if (defeat[i]->FUN_0048ea00())
            return 1;
    }
    return 0;
}
