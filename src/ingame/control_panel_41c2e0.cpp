// Decompiled by Haiku. Names are provisional.

extern void* g_game;
extern int __stdcall FindNextSelectedUnit(int, int);

// FUNCTION: 0x41c2e0
void __stdcall FUN_0041c2e0(int param)
{
    int val = *(int*)((char*)g_game + 0x142f3);
    int result = FindNextSelectedUnit(val, param);
    *(int*)((char*)g_game + 0x142f3) = result;
}
