// Decompiled by Opus. Names are provisional.
// Unloads the debug-help DLL: calls its cleanup entry point (SymCleanup-like,
// taking the process handle), frees the library and clears every loaded
// function pointer.
#include <windows.h>

extern char DAT_00528ad8;
extern HMODULE DAT_00528ae0;
extern int DAT_00528ad0;
extern void* DAT_00528ab8;
extern BOOL (__stdcall* DAT_00528abc)(HANDLE);
extern void* DAT_00528ac0;
extern void* DAT_00528ac4;
extern void* DAT_00528ac8;
extern void* DAT_00528acc;
extern void* DAT_00528ab4;
extern void* DAT_00528ad4;

// FUNCTION: 0x4de110
void FUN_004de110()
{
    DAT_00528ad8 = 0;
    if (DAT_00528ae0) {
        DAT_00528abc(GetCurrentProcess());
        FreeLibrary(DAT_00528ae0);
    }
    DAT_00528ae0 = 0;
    DAT_00528ad0 = 0;
    DAT_00528ab8 = 0;
    DAT_00528abc = 0;
    DAT_00528ac0 = 0;
    DAT_00528ac4 = 0;
    DAT_00528ac8 = 0;
    DAT_00528acc = 0;
    DAT_00528ab4 = 0;
    DAT_00528ad4 = 0;
}
