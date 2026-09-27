// Decompiled by Sonnet. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

extern char DAT_005090fc[]; // "DefeatCondition_AllUnitsKilled"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class Class_0048f840 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048f840(Class_004b4560* obj);
};

// FUNCTION: 0x48f840
void Class_0048f840::FUN_0048f840(Class_004b4560* obj)
{
    obj->FUN_004b4560(DAT_005090fc);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f30, satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f24, celebrated);
}
