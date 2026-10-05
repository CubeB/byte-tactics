// Decompiled by Opus. Names are provisional.
// Writes the "kill all of type" victory condition's state to a section;
// the reading counterpart is 0x48f0b0, compare 0x48ef00.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

class VictoryKillAllOfType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f070
void VictoryKillAllOfType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllOfType");
    ((HapiBank*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((HapiBank*)obj)->SetIntegerItem("Celebrated", celebrated);
}
