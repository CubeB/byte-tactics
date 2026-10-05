// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
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
    numUnits = ((HapiBank*)obj)->GetIntegerItem("NumUnits", 0);
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
