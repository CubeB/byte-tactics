// Decompiled by Opus. Names are provisional.
// Writes the "unit type passes X" victory condition's state to a section;
// the reading counterpart is 0x48f480 (compare 0x48ee30).

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

class Class_0048f3e0 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void FUN_0048f840(Class_004b4560* obj);
};

// FUNCTION: 0x48f440
void Class_0048f3e0::FUN_0048f840(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_UnitTypePassesX");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
