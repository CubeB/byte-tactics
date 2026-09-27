// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x40d4f0
void __stdcall FUN_0040d4f0(char* param_1, char* param_2, char* param_3)
{
    char* ecx = param_1;
    char* eax = param_3;
    char* esi = param_2;

    for (; ecx != esi; ) {
        if (eax != 0) {
            *eax = *ecx;
        }
        eax = eax ? eax + 1 : eax;
        ecx++;
    }
}
