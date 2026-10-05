// Decompiled by Haiku. Names are provisional.

extern void FUN_004e1410();
extern char* __cdecl FindCommandLineSwitch(char*);
extern void ShowMemoryStatus();

extern char DAT_0050d220[];

// FUNCTION: 0x4e1460
void __cdecl StartMemoryStatus()
{
    FUN_004e1410();
    char* result = FindCommandLineSwitch(DAT_0050d220);
    if (result != 0) {
        ShowMemoryStatus();
    }
}
