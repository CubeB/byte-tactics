// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

// The "unit type passes Z" victory condition; reads its state from a section.
class Class_0048f5d0 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048f5d0(Class_004b4560* obj);
};

// FUNCTION: 0x48f5d0
void Class_0048f5d0::FUN_0048f5d0(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_UnitTypePassesZ");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
