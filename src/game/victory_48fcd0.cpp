// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

// The "any unit passes Z" defeat condition; writes its state to a section.
class DefeatAnyUnitPassesZ {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(Class_004b4560* obj);
};

// FUNCTION: 0x48fcd0
void DefeatAnyUnitPassesZ::SaveState(Class_004b4560* obj)
{
    obj->FUN_004b4560("DefeatCondition_AnyUnitPassesZ");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
