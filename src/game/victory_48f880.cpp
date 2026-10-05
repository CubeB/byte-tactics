// Decompiled by Opus. Names are provisional.
// Reads the "all units killed" defeat condition's state from a section;
// the writing counterpart is 0x48f840, compare 0x48ef40.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

class DefeatAllUnitsKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f880
void DefeatAllUnitsKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilled");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
