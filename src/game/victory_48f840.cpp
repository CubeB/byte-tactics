// Decompiled by Sonnet. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

extern char DAT_005090fc[]; // "DefeatCondition_AllUnitsKilled"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class DefeatAllUnitsKilled {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f840
void DefeatAllUnitsKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_005090fc);
    ((HapiBank*)obj)->SetIntegerItem(DAT_00508f30, satisfied);
    ((HapiBank*)obj)->SetIntegerItem(DAT_00508f24, celebrated);
}
