// Decompiled by Haiku. Names are provisional.

extern void* FUN_004b6220();

// FUNCTION: 0x4c13a0
void __stdcall FUN_004c13a0(int param_1, int param_2)
{
    void* eax = FUN_004b6220();
    if (param_1 != -1) {
        *(int*)((unsigned char*)eax + 0x208) = param_1;
    }
    if (param_2 != -1) {
        *(int*)((unsigned char*)eax + 0x20c) = param_2;
    }
}
