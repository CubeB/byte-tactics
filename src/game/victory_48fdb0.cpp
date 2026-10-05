// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f670: reads the "death timer runs out" defeat
// condition's state from a section (its writer is 0x48fd70).

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class DefeatDeathTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(Class_004b4560* obj);
};

// FUNCTION: 0x48fdb0
void DefeatDeathTimerRunsOut::LoadState(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_DeathTimerRunsOut");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
