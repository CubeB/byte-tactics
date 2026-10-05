// Decompiled by Opus. Names are provisional.
// Console command: with an argument, calls EnableMeteors or DisableMeteors
// depending on it; without one, calls StartMeteorShower.

// Command arguments.
class CommandArgs {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int GetIntArg(int index, int fallback);
};

void EnableMeteors();
void DisableMeteors();
void StartMeteorShower();

// FUNCTION: 0x417760
void __stdcall CmdMeteor(CommandArgs* args)
{
    if (args->count > 1) {
        if (args->GetIntArg(1, 0))
            EnableMeteors();
        else
            DisableMeteors();
    } else {
        StartMeteorShower();
    }
}
