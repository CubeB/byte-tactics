// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f160: writes the "kill enemy commander" victory
// condition's state to a section.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

extern char DAT_00508f3c[]; // "VictoryCondition_KillEnemyCommander"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillEnemyCommander {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48eac0
void VictoryKillEnemyCommander::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f3c);
    ((HapiBank*)obj)->SetIntegerItem(DAT_00508f30, satisfied);
    ((HapiBank*)obj)->SetIntegerItem(DAT_00508f24, celebrated);
}
