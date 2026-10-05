// Decompiled by Opus. Names are provisional.
// Command handler (sibling of 0x417430): looks up the unit type named by the
// first argument and, if it exists, applies KillUnitsOfType and ReloadUnitType
// to its id.

extern char DAT_005119b8[];

// Command arguments.
class Class_004b73c0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    char* FUN_004b73c0(int index, char* fallback);
};

short __stdcall FindUnitTypeId(char* name);
void __stdcall KillUnitsOfType(short id);
void __stdcall ReloadUnitType(unsigned short id);

// FUNCTION: 0x417490
void __stdcall FUN_00417490(Class_004b73c0* args)
{
    if (args->count > 1) {
        short id = FindUnitTypeId(args->FUN_004b73c0(1, DAT_005119b8));
        if (id) {
            KillUnitsOfType(id);
            ReloadUnitType(id);
        }
    }
}
