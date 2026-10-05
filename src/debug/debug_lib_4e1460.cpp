// Decompiled by Haiku. Names are provisional.

extern void FUN_004e1410();
extern char* __cdecl FUN_004d9f60(char*);
extern void FUN_004e1400();

extern char DAT_0050d220[];

// FUNCTION: 0x4e1460
void __cdecl FUN_004e1460()
{
    FUN_004e1410();
    char* result = FUN_004d9f60(DAT_0050d220);
    if (result != 0) {
        FUN_004e1400();
    }
}
