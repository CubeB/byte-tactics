// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4b4bf0
int __fastcall FUN_004b4bf0(int** param)
{
    int* pstruct = param[0];
    int ecx_val = pstruct[2];
    int edx_val = pstruct[1];

    ecx_val = ecx_val * 3;
    int eax_val = edx_val + ecx_val * 8;

    ecx_val = *(int*)(edx_val + ecx_val * 8 + 0xc);
    edx_val = *(int*)(eax_val + 0x14);

    ecx_val = ecx_val * 5;
    return *(int*)(edx_val + ecx_val * 4 + 8);
}
