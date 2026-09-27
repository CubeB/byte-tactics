// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

void FUN_00430f00();

// FUNCTION: 0x417130
void __stdcall FUN_00417130(int unused)
{
    int eax = DAT_00511de8;
    *(int*)(eax + 0x37f02) ^= 1;
    FUN_00430f00();
}
