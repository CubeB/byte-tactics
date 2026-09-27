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
class Class_0048ef40 {
public:
    char unknown_0[4];
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    void FUN_0048ef40(Class_004b4560* obj);
};

// FUNCTION: 0x48ef40
void Class_0048ef40::FUN_0048ef40(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_CaptureUnitType");
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
