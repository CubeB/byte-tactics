// Decompiled by Opus. Names are provisional.
// Reads the "any unit passes X" defeat condition's state from a section
// (same shape as 0x48f0b0).

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

class DefeatAnyUnitPassesX {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48fc00
void DefeatAnyUnitPassesX::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesX");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
