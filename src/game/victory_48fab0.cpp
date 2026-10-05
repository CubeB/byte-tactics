// Decompiled by Opus. Names are provisional.
// Same shape as 0x48eb80: writes the "all units killed of type" defeat
// condition's state to a section.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

class DefeatAllUnitsKilledOfType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48fab0
void DefeatAllUnitsKilledOfType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilledOfType");
    ((HapiBank*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((HapiBank*)obj)->SetIntegerItem("Celebrated", celebrated);
}
