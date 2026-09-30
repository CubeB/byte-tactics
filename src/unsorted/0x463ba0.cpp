// Decompiled by Haiku. Names are provisional.

extern char DAT_0052a4e4;
extern void __cdecl atexit(void*);
extern void __cdecl FUN_00463bd0();

// FUNCTION: 0x463ba0
void FUN_00463ba0()
{
    if ((DAT_0052a4e4 & 1) == 0) {
        DAT_0052a4e4 = DAT_0052a4e4 | 1;
    }
    atexit(FUN_00463bd0);
}
