// Decompiled by Opus. Names are provisional.
// Reads the "build unit type" victory condition's state from a section; the
// writing counterpart is 0x48ee30, same shape as 0x48eb00.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

extern char DAT_00508fb4[]; // "VictoryCondition_BuildUnitType"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryBuildUnitType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48ee70
void VictoryBuildUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508fb4);
    satisfied = ((HapiBank*)obj)->GetIntegerItem(DAT_00508f30, 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem(DAT_00508f24, 0);
}
