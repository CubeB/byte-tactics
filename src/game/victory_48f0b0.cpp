// Decompiled by Opus. Names are provisional.
// Same shape as 0x48ef40.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

// The "kill all of type" victory condition; reads its state from a section.
class VictoryKillAllOfType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f0b0
void VictoryKillAllOfType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllOfType");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
