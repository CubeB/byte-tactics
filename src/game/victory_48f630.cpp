// Decompiled by Opus. Names are provisional.
// Same shape as 0x48fd70: writes the "victory timer runs out" victory
// condition's state to a section (its reader is 0x48f670).

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

class Class_0048f610 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void FUN_0048f840(Class_004b4560* obj);
};

// FUNCTION: 0x48f630
void Class_0048f610::FUN_0048f840(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_VictoryTimerRunsOut");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
