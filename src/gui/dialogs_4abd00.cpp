// Decompiled by Haiku. Names are provisional.

extern void __stdcall IsGadgetNamed(int, int, int);
extern char DAT_00502ae8;

// FUNCTION: 0x4abd00
void __stdcall MessageBoxHandler(int param_1)
{
    int edx = *(int*)(param_1 + 0x18);
    IsGadgetNamed(*(int*)(edx + 4), *(int*)(param_1 + 0x60), (int)&DAT_00502ae8);
}
