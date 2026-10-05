// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

// The "any unit passes Z" defeat condition; writes its state to a section.
class DefeatAnyUnitPassesZ {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48fcd0
void DefeatAnyUnitPassesZ::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AnyUnitPassesZ");
    ((HapiBank*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((HapiBank*)obj)->SetIntegerItem("Celebrated", celebrated);
}
