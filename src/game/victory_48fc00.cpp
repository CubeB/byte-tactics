// Decompiled by Opus. Names are provisional.
// Reads the "any unit passes X" defeat condition's state from a section
// (same shape as 0x48f0b0).

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class DefeatAnyUnitPassesX {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(Class_004b4560* obj);
};

// FUNCTION: 0x48fc00
void DefeatAnyUnitPassesX::LoadState(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_AnyUnitPassesX");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
