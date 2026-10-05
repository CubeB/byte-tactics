// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
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

    virtual void SaveState(Class_004b4560* obj);
};

// FUNCTION: 0x48f160
void VictoryKillUnitType::SaveState(Class_004b4560* obj)
{
    obj->FUN_004b4560(DAT_00509028);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00509018, numLeftToKill);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f30, satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f24, celebrated);
}
