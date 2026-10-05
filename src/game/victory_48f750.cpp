// Decompiled by Opus. Names are provisional.
// Same shape as 0x48ef40: reads the "commander killed" defeat condition's
// state from a section.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

class DefeatCommanderKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f750
void DefeatCommanderKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_CommanderKilled");
    satisfied = ((Class_004b4800*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((Class_004b4800*)obj)->GetIntegerItem("Celebrated", 0);
}
