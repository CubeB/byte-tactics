// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x49fb10
void __stdcall FUN_0049fb10(int param_1, int param_2)
{
    int eax = *(int*)(param_1 + 0x18);
    if (eax != 0) {
        *(int*)(eax + 0x18) = param_2;
    }
}
