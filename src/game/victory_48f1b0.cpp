// Decompiled by Opus. Names are provisional.
// Load counterpart of 0x48f160 (the "kill unit type" victory condition).

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

extern char DAT_00509028[]; // "VictoryCondition_KillUnitType"
extern char DAT_00509018[]; // "NumLeftToKill"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillUnitType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[0x2c - 0xc];
    int numLeftToKill;                   // +0x2c

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f1b0
void VictoryKillUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00509028);
    numLeftToKill = ((HapiBank*)obj)->GetIntegerItem(DAT_00509018, 0);
    satisfied = ((HapiBank*)obj)->GetIntegerItem(DAT_00508f30, 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem(DAT_00508f24, 0);
}
