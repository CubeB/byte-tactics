// Decompiled by Sonnet. Names are provisional.

// FUNCTION: 0x48a060
void __stdcall SetWeaponTargetUnit(char* param_1, char* param_2, int param_3)
{
    char* elem = param_1 + param_3 * 0x1c;
    *(unsigned short*)(elem + 4) = *(unsigned short*)(param_2 + 0xa8);
    *(unsigned short*)(elem + 6) = 0x8000;
    *(unsigned short*)(param_1 + 0xba) &= 0x83ff;
}
