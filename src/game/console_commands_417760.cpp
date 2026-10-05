// Decompiled by Opus. Names are provisional.
// Console command: with an argument, calls EnableMeteors or DisableMeteors
// depending on it; without one, calls StartMeteorShower.

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

void EnableMeteors();
void DisableMeteors();
void StartMeteorShower();

// FUNCTION: 0x417760
void __stdcall CmdMeteor(Class_004b73e0* args)
{
    if (args->count > 1) {
        if (args->FUN_004b73e0(1, 0))
            EnableMeteors();
        else
            DisableMeteors();
    } else {
        StartMeteorShower();
    }
}
