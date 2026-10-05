// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

// The "unit type passes Z" victory condition; reads its state from a section.
class VictoryUnitTypePassesZ {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f5d0
void VictoryUnitTypePassesZ::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesZ");
    satisfied = ((Class_004b4800*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->GetIntegerItem("Celebrated", 0);
}
