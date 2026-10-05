// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

// The "unit type killed" defeat condition; writes its state to a section.
class DefeatUnitTypeKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    char unknown_c[0x2c - 0xc];
    int numLeftToKill;                   // +0x2c

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f900
void DefeatUnitTypeKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_UnitTypeKilled");
    ((HapiBank*)obj)->SetIntegerItem("NumLeftToKill", numLeftToKill);
    ((HapiBank*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((HapiBank*)obj)->SetIntegerItem("Celebrated", celebrated);
}
