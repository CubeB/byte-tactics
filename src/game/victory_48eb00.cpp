// Decompiled by Opus. Names are provisional.
// Reads the "kill enemy commander" victory condition's state from a section;
// the writing counterpart is 0x48eac0, compare 0x48f880.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

extern char DAT_00508f3c[]; // "VictoryCondition_KillEnemyCommander"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillEnemyCommander {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(Class_004b4560* obj);
};

// FUNCTION: 0x48eb00
void VictoryKillEnemyCommander::LoadState(Class_004b4560* obj)
{
    obj->FUN_004b4560(DAT_00508f3c);
    satisfied = ((Class_004b4800*)obj)->FUN_004b4800(DAT_00508f30, 0);
    celebrated = ((Class_004b4800*)obj)->FUN_004b4800(DAT_00508f24, 0);
}
