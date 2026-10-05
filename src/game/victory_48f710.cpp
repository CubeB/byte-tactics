// Decompiled by Opus. Names are provisional.
// Writes the "commander killed" defeat condition's state to a section; the
// reading counterpart is 0x48f750, compare 0x48ef00.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

class DefeatCommanderKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f710
void DefeatCommanderKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_CommanderKilled");
    ((HapiBank*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((HapiBank*)obj)->SetIntegerItem("Celebrated", celebrated);
}
