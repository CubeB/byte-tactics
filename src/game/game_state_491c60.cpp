// Decompiled by Sonnet. Names are provisional.

extern void BlankScreen();
extern void RemoveLocalPlayers();
extern void FUN_00491b60();
extern void __stdcall QuitApp(const char*);

// FUNCTION: 0x491c60
void FUN_00491c60()
{
    BlankScreen();
    RemoveLocalPlayers();
    FUN_00491b60();
    QuitApp(0);
}
