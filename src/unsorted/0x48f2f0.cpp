// Decompiled by Opus. Names are provisional.
// Writes the "move unit to radius" victory condition's state to a section
// (same shape as 0x48f070).

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

class Class_0048f2f0 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048f2f0(Class_004b4560* obj);
};

// FUNCTION: 0x48f2f0
void Class_0048f2f0::FUN_0048f2f0(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_MoveUnitToRadius");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
