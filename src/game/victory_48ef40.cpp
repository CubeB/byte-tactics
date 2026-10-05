// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

// The "capture unit type" victory condition; reads its state from a section.
class VictoryCaptureUnitType {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(Class_004b4560* obj);
};

// FUNCTION: 0x48ef40
void VictoryCaptureUnitType::LoadState(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_CaptureUnitType");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
