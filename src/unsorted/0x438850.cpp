// Decompiled by Haiku. Names are provisional.

extern int DAT_00512344;

// FUNCTION: 0x438850
int __fastcall FUN_00438850(unsigned char* param_1)
{
    unsigned int eax = 0;
    eax = *param_1;
    eax = eax + eax * 4;
    int edx = DAT_00512344 + eax * 4;
    eax = eax + edx;
    return eax;
}
