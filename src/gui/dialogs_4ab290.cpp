// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4ab290
int __stdcall FUN_004ab290(int param_1, int param_2)
{
    int eax = *(int*)(param_1 + 0x18);
    if (eax != 0) {
        *(int*)(eax + 0x24) = param_2;
    }
    return 1;
}
