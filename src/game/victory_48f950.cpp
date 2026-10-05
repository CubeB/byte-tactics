// Decompiled by Opus. Names are provisional.
// Reads the "unit type killed" defeat condition's state from a section;
// the writing counterpart is 0x48f900, compare 0x48f880.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

class DefeatUnitTypeKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[0x2c - 0xc];
    int numLeftToKill;                   // +0x2c

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f950
void DefeatUnitTypeKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_UnitTypeKilled");
    numLeftToKill = ((Class_004b4800*)obj)->GetIntegerItem("NumLeftToKill", 0);
    satisfied = ((Class_004b4800*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->GetIntegerItem("Celebrated", 0);
}
