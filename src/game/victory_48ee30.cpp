// Decompiled by Opus. Names are provisional.
// Same shape as 0x48eb80: writes the "build unit type" victory condition's
// state to a section.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

class VictoryBuildUnitType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48ee30
void VictoryBuildUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_BuildUnitType");
    ((HapiBank*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((HapiBank*)obj)->SetIntegerItem("Celebrated", celebrated);
}
