// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f670: reads the "death timer runs out" defeat
// condition's state from a section (its writer is 0x48fd70).

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

class DefeatDeathTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48fdb0
void DefeatDeathTimerRunsOut::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_DeathTimerRunsOut");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
