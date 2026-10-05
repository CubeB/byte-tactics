// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4630 {
public:
    void SetIntegerItem(const char* name, int value);
};

// The "death timer runs out" defeat condition; writes its state to a section.
class DefeatDeathTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48fd70
void DefeatDeathTimerRunsOut::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_DeathTimerRunsOut");
    ((Class_004b4630*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((Class_004b4630*)obj)->SetIntegerItem("Celebrated", celebrated);
}
