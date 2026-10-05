// Decompiled by Opus. Names are provisional.
// Writes the "unit type passes X" victory condition's state to a section;
// the reading counterpart is 0x48f480 (compare 0x48ee30).

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

class VictoryUnitTypePassesX {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f440
void VictoryUnitTypePassesX::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesX");
    ((HapiBank*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((HapiBank*)obj)->SetIntegerItem("Celebrated", celebrated);
}
