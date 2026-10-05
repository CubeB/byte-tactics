// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

// The "kill all mobile units" victory condition; reads its state from a
// section (the load counterpart of 0x48ecb0).
class Class_0048ec20 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[4];                   // +0xc, the listener's vtable
    int numUnits;                        // +0x10

    virtual void FUN_0048f880(Class_004b4560* obj);
};

// FUNCTION: 0x48ed00
void Class_0048ec20::FUN_0048f880(Class_004b4560* obj)
{
    obj->FUN_004b4560("VictoryCondition_KillAllMobileUnits");
    numUnits = ((Class_004b4800*)obj)->FUN_004b4800("NumUnits", 0);
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800("Celebrated", 0);
}
