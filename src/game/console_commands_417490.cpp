// Decompiled by Opus. Names are provisional.
// Command handler (sibling of 0x417430): looks up the unit type named by the
// first argument and, if it exists, applies FUN_00486e80 and FUN_0042d1f0
// to its id.

extern char DAT_005119b8[];

// Command arguments.
class Class_004b73c0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    char* FUN_004b73c0(int index, char* fallback);
};

short __stdcall FUN_00488b10(char* name);
void __stdcall FUN_00486e80(short id);
void __stdcall FUN_0042d1f0(unsigned short id);

// FUNCTION: 0x417490
void __stdcall FUN_00417490(Class_004b73c0* args)
{
    if (args->count > 1) {
        short id = FUN_00488b10(args->FUN_004b73c0(1, DAT_005119b8));
        if (id) {
            FUN_00486e80(id);
            FUN_0042d1f0(id);
        }
    }
}
