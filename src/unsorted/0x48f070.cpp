// Decompiled by Opus. Names are provisional.
// Writes the "kill all of type" victory condition's state to a section;
// the reading counterpart is 0x48f0b0, compare 0x48ef00.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

class Class_0048efb0 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void FUN_0048f840(Class_004b4560* obj);
};

// FUNCTION: 0x48f070
void Class_0048efb0::FUN_0048f840(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_KillAllOfType");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
