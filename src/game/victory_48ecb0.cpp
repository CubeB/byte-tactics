// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4630 {
public:
    void SetIntegerItem(const char* name, int value);
};

extern char DAT_00508f90[]; // "VictoryCondition_KillAllMobileUnits"
extern char DAT_00508f84[]; // "NumUnits"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillAllMobileUnits {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[4];                   // +0xc, the listener's vtable
    int numUnits;                        // +0x10

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48ecb0
void VictoryKillAllMobileUnits::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f90);
    ((Class_004b4630*)obj)->SetIntegerItem(DAT_00508f84, numUnits);
    ((Class_004b4630*)obj)->SetIntegerItem(DAT_00508f30, satisfied);
    ((Class_004b4630*)obj)->SetIntegerItem(DAT_00508f24, celebrated);
}
