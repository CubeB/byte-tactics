// Decompiled by Opus. Names are provisional.
// Reads the "any unit passes Z" defeat condition's state from a section
// (same shape as 0x48f480).

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class Class_0048fd10 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048fd10(Class_004b4560* obj);
};

// FUNCTION: 0x48fd10
void Class_0048fd10::FUN_0048fd10(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_AnyUnitPassesZ");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
