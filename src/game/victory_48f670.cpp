// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f480: reads the "victory timer runs out" victory
// condition's state from a section.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

class VictoryTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f670
void VictoryTimerRunsOut::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_VictoryTimerRunsOut");
    satisfied = ((Class_004b4800*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->GetIntegerItem("Celebrated", 0);
}
