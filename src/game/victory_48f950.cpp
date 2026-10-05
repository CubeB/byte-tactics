// Decompiled by Opus. Names are provisional.
// Reads the "unit type killed" defeat condition's state from a section;
// the writing counterpart is 0x48f900, compare 0x48f880.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class DefeatUnitTypeKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[0x2c - 0xc];
    int numLeftToKill;                   // +0x2c

    virtual void LoadState(Class_004b4560* obj);
};

// FUNCTION: 0x48f950
void DefeatUnitTypeKilled::LoadState(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_UnitTypeKilled");
    numLeftToKill = ((Class_004b4800*)obj)->FUN_004b4800("NumLeftToKill", 0);
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
