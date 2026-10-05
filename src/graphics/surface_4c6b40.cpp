// Decompiled by Haiku. Names are provisional.

extern void* FUN_004b6220();

// FUNCTION: 0x4c6b40
void __stdcall FUN_004c6b40(int param_1)
{
    void* ptr = FUN_004b6220();
    *(int*)((unsigned char*)ptr + 0xe4) = param_1;
}
