// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f0b0.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

// The "move unit to radius" victory condition; reads its state from a section.
class Class_0048f250 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void FUN_0048f880(Class_004b4560* obj);
};

// FUNCTION: 0x48f330
void Class_0048f250::FUN_0048f880(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_MoveUnitToRadius");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
