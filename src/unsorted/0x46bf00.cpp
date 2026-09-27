// Decompiled by Haiku. Names are provisional.

extern int(__stdcall* DAT_0051e588)(int);

// FUNCTION: 0x46bf00
int __stdcall FUN_0046bf00(int param_1)
{
    if (DAT_0051e588 != 0) {
        return DAT_0051e588(param_1);
    }
    return 1;
}
