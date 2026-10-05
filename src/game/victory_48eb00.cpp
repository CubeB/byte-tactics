// Decompiled by Opus. Names are provisional.
// Reads the "kill enemy commander" victory condition's state from a section;
// the writing counterpart is 0x48eac0, compare 0x48f880.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

extern char DAT_00508f3c[]; // "VictoryCondition_KillEnemyCommander"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillEnemyCommander {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48eb00
void VictoryKillEnemyCommander::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f3c);
    satisfied = ((HapiBank*)obj)->GetIntegerItem(DAT_00508f30, 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem(DAT_00508f24, 0);
}
