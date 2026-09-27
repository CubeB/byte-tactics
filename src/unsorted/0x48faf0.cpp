// Decompiled by Opus. Names are provisional.
// Same shape as 0x48ef40.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

// The "all units killed of type" defeat condition; reads its state from a
// section.
class Class_0048faf0 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048faf0(Class_004b4560* obj);
};

// FUNCTION: 0x48faf0
void Class_0048faf0::FUN_0048faf0(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_AllUnitsKilledOfType");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
