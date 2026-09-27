// Decompiled by Opus. Names are provisional.
// Same shape as 0x48eb80: writes the "build unit type" victory condition's
// state to a section.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

class Class_0048ee30 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048ee30(Class_004b4560* obj);
};

// FUNCTION: 0x48ee30
void Class_0048ee30::FUN_0048ee30(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_BuildUnitType");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
