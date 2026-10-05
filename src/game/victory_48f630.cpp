// Decompiled by Opus. Names are provisional.
// Same shape as 0x48fd70: writes the "victory timer runs out" victory
// condition's state to a section (its reader is 0x48f670).

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4630 {
public:
    void SetIntegerItem(const char* name, int value);
};

class VictoryTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f630
void VictoryTimerRunsOut::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_VictoryTimerRunsOut");
    ((Class_004b4630*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((Class_004b4630*)obj)->SetIntegerItem("Celebrated", celebrated);
}
