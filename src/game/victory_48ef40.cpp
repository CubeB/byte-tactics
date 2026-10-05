// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

// The "capture unit type" victory condition; reads its state from a section.
class VictoryCaptureUnitType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48ef40
void VictoryCaptureUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_CaptureUnitType");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
