// Decompiled by Opus. Names are provisional.
// Load counterpart of 0x48f160 (the "kill unit type" victory condition).

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

extern char DAT_00509028[]; // "VictoryCondition_KillUnitType"
extern char DAT_00509018[]; // "NumLeftToKill"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class Class_0048f0f0 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[0x2c - 0xc];
    int numLeftToKill;                   // +0x2c

    virtual void FUN_0048f880(Class_004b4560* obj);
};

// FUNCTION: 0x48f1b0
void Class_0048f0f0::FUN_0048f880(Class_004b4560* obj)
{
    obj->FUN_004b4560(DAT_00509028);
    numLeftToKill = ((Class_004b4800*)obj)->FUN_004b4800(DAT_00509018, 0);
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800(DAT_00508f30, 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800(DAT_00508f24, 0);
}
