// Decompiled by Opus. Names are provisional.
// Reads the "any unit passes Z" defeat condition's state from a section
// (same shape as 0x48f480).

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

class DefeatAnyUnitPassesZ {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48fd10
void DefeatAnyUnitPassesZ::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesZ");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
