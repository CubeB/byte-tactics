// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

// The "kill all mobile units" victory condition; reads its state from a
// section (the load counterpart of 0x48ecb0).
class VictoryKillAllMobileUnits {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[4];                   // +0xc, the listener's vtable
    int numUnits;                        // +0x10

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48ed00
void VictoryKillAllMobileUnits::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllMobileUnits");
    numUnits = ((Class_004b4800*)obj)->GetIntegerItem("NumUnits", 0);
    satisfied = ((Class_004b4800*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->GetIntegerItem("Celebrated", 0);
}
