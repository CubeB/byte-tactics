// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

// The "unit type killed" defeat condition; writes its state to a section.
class Class_0048f900 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[0x2c - 0xc];
    int numLeftToKill;                   // +0x2c

    void FUN_0048f900(Class_004b4560* obj);
};

// FUNCTION: 0x48f900
void Class_0048f900::FUN_0048f900(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_UnitTypeKilled");
    ((Class_004b4630*)obj)->FUN_004b4630("NumLeftToKill", numLeftToKill);
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
