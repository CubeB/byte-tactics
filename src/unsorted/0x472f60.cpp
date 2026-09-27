// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

static inline int Compare(int eax, int edx, int ecx)
{
    if (eax > edx) return 0;
    if ((unsigned int)eax > (unsigned int)*(int*)(ecx + 0x38a47)) return 0;
    return 1;
}

// FUNCTION: 0x472f60
int __fastcall FUN_00472f60(int param_1)
{
    int eax = *(int*)(param_1 + 8);
    int edx = *(int*)(param_1 + 4);
    int ecx = DAT_00511de8;
    return Compare(eax, edx, ecx);
}
