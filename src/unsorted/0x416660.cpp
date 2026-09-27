// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

// FUNCTION: 0x416660
void __stdcall FUN_00416660(int unused)
{
    int ecx = DAT_00511de8;
    int eax = *(unsigned short*)(ecx + 0x37f06);
    int edx = eax;
    edx = ~edx;
    edx ^= eax;
    edx &= 0x10;
    edx ^= eax;
    *(unsigned short*)(ecx + 0x37f06) = (unsigned short)edx;
}
