// Decompiled by Opus. Names are provisional.
// Writes the "kill all of type" victory condition's state to a section;
// the reading counterpart is 0x48f0b0, compare 0x48ef00.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4630 {
public:
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
    ((Class_004b4630*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((Class_004b4630*)obj)->SetIntegerItem("Celebrated", celebrated);
}
