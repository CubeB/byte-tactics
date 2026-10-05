// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f160: writes the "kill enemy commander" victory
// condition's state to a section.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

extern char DAT_00508f3c[]; // "VictoryCondition_KillEnemyCommander"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillEnemyCommander {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(Class_004b4560* obj);
};

// FUNCTION: 0x48eac0
void VictoryKillEnemyCommander::SaveState(Class_004b4560* obj)
{
    obj->FUN_004b4560(DAT_00508f3c);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f30, satisfied);
    ((Class_004b4630*)obj)->FUN_004b4630(DAT_00508f24, celebrated);
}
