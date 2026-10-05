// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f480: reads the "victory timer runs out" victory
// condition's state from a section.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class VictoryTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(Class_004b4560* obj);
};

// FUNCTION: 0x48f670
void VictoryTimerRunsOut::LoadState(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_VictoryTimerRunsOut");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
