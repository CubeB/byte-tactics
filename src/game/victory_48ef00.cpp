// Decompiled by Opus. Names are provisional.
// Writes the "capture unit type" victory condition's state to a section;
// the reading counterpart is 0x48ef40, compare 0x48f840.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

class VictoryCaptureUnitType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(Class_004b4560* obj);
};

// FUNCTION: 0x48ef00
void VictoryCaptureUnitType::SaveState(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_CaptureUnitType");
    ((Class_004b4630*)obj)->FUN_004b4630("Satisfied", satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630("Celebrated", celebrated);
}
