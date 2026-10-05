// Decompiled by Opus. Names are provisional.
// Same shape as 0x48faf0: reads the "destroy all units" victory condition's
// state from a section.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

class VictoryDestroyAllUnits {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48ebc0
void VictoryDestroyAllUnits::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_DestroyAllUnits");
    satisfied = ((Class_004b4800*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->GetIntegerItem("Celebrated", 0);
}
