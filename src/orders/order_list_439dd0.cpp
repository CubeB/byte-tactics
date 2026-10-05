// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x439dd0
int __stdcall FUN_00439dd0(int param_1)
{
    int eax = param_1;
    if (eax != 0) {
        eax = *(int*)(eax + 0x5c);
        if (eax != 0) {
            eax = *(int*)(eax + 0x16);
            return eax;
        }
    }
    return 0;
}
