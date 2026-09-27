// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

// FUNCTION: 0x48fd50
int __fastcall FUN_0048fd50(int param_1)
{
    int eax = DAT_00511de8;
    int edx = *(int*)(eax + 0x38a47);
    eax = *(int*)(param_1 + 0xc);
    if (edx < eax) return 0;
    return 1;
}
