// Decompiled by Opus. Names are provisional.
// Writes the "capture unit type" victory condition's state to a section;
// the reading counterpart is 0x48ef40, compare 0x48f840.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4630 {
public:
    void SetIntegerItem(const char* name, int value);
};

class VictoryCaptureUnitType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48ef00
void VictoryCaptureUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_CaptureUnitType");
    ((Class_004b4630*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((Class_004b4630*)obj)->SetIntegerItem("Celebrated", celebrated);
}
