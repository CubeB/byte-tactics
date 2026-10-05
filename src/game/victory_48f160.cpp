// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4630 {
public:
    void SetIntegerItem(const char* name, int value);
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

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f160
void VictoryKillUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00509028);
    ((Class_004b4630*)obj)->SetIntegerItem(DAT_00509018, numLeftToKill);
    ((Class_004b4630*)obj)->SetIntegerItem(DAT_00508f30, satisfied);
    ((Class_004b4630*)obj)->SetIntegerItem(DAT_00508f24, celebrated);
}
