// Decompiled by Haiku. Names are provisional.

extern int FUN_004b6220();

// FUNCTION: 0x4c1420
void __stdcall FUN_004c1420(int param_1)
{
    if (param_1 != 0) {
        int eax = FUN_004b6220();
        *(int*)(eax + 0x204) = param_1;
    }
}
