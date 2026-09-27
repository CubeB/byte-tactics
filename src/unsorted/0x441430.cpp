// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

// FUNCTION: 0x441430
int __cdecl FUN_00441430()
{
    int ecx = DAT_00511de8;
    unsigned int edx = *(unsigned char*)(ecx + 0x2a42);
    int eax = edx;
    ecx = ecx + edx;
    eax = eax << 5;
    eax = eax + edx;
    eax = eax + eax * 4;
    eax = *(int*)(ecx + eax * 2 + 0x1b8a);
    return eax + 0x80;
}
