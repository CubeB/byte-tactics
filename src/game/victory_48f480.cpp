// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f330: reads the "unit type passes X" victory condition's
// state from a section.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

class VictoryUnitTypePassesX {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f480
void VictoryUnitTypePassesX::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesX");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
