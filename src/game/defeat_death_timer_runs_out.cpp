// Decompiled by Sonnet and Opus. Names are provisional.

extern char* g_game;

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
    int GetIntegerItem(char* name, int def);
};

class DefeatDeathTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    int field_c;                         // +0xc

    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 0 (IsSatisfied) of the "death timer runs out" defeat condition
// (vtable 0x4fd7a8): met once the game's tick count reaches the limit.
// FUNCTION: 0x48fd50
int DefeatDeathTimerRunsOut::IsSatisfied()
{
    unsigned int game_val = *(unsigned int*)(g_game + 0x38a47);
    return game_val >= (unsigned int)field_c;
}

// FUNCTION: 0x48fd70
void DefeatDeathTimerRunsOut::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_DeathTimerRunsOut");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f670: reads the "death timer runs out" defeat
// condition's state from a section (its writer is 0x48fd70).
// FUNCTION: 0x48fdb0
void DefeatDeathTimerRunsOut::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_DeathTimerRunsOut");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
