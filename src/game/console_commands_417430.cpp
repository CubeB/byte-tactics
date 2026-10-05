// Decompiled by Opus. Names are provisional.
#include <stdio.h>

extern char DAT_005119b8[];

// Command arguments.
class Class_004b73c0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    char* FUN_004b73c0(int index, char* fallback);
};

void __stdcall FUN_004bcf00(char* dir);
void __stdcall SaveGameFile(char* path, char* description, int param_3);

// FUNCTION: 0x417430
void __stdcall CmdSave(Class_004b73c0* args)
{
    char path[256];
    if (args->count > 1) {
        sprintf(path, "savegame\\%s.sav", args->FUN_004b73c0(1, DAT_005119b8));
        FUN_004bcf00("savegame");
        SaveGameFile(path, "Generic Game Description", 0x29a);
    }
}
