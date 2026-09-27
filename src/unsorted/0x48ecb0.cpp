// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

extern char DAT_00508f90[]; // "VictoryCondition_KillAllMobileUnits"
extern char DAT_00508f84[]; // "NumUnits"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class Class_0048ecb0 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[4];
    int numUnits;                        // +0x10

    void FUN_0048ecb0(Class_004b4560* obj);
};

// FUNCTION: 0x48ecb0
void Class_0048ecb0::FUN_0048ecb0(Class_004b4560* obj)
{
    obj->FUN_004b4560(DAT_00508f90);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f84, numUnits);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f30, satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f24, celebrated);
}
