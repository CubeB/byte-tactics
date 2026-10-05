// Decompiled by Opus. Names are provisional.
// Console command: with an argument, calls FUN_00437d40 or FUN_00437d50
// depending on it; without one, calls FUN_00438070.

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

void FUN_00437d40();
void FUN_00437d50();
void FUN_00438070();

// FUNCTION: 0x417760
void __stdcall CmdMeteor(Class_004b73e0* args)
{
    if (args->count > 1) {
        if (args->FUN_004b73e0(1, 0))
            FUN_00437d40();
        else
            FUN_00437d50();
    } else {
        FUN_00438070();
    }
}
