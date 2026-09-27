// Decompiled by Haiku. Names are provisional.

extern void* g_game;

extern void FUN_004c22f0(int param_1, int param_2);
extern void FUN_004c2870();

// FUNCTION: 0x41cd20
void __cdecl FUN_0041cd20()
{
    int eax = (int)g_game;
    *(int*)(eax + 0x2cdf) = 0;
    eax = eax + 0x2cc7;
    int ecx = *(int*)(eax + 4);
    int edx = *(int*)(eax);
    FUN_004c22f0(edx, ecx);
    FUN_004c2870();
}
