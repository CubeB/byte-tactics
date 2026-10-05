// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

// FUNCTION: 0x417030
void __stdcall CmdDoubleShot(int unused)
{
    int ecx = DAT_00511de8;
    unsigned short* ptr = (unsigned short*)((char*)ecx + 0x37f2f);
    unsigned short ax = *ptr;
    unsigned short edx = ax;
    edx = (unsigned short)(~edx);
    edx ^= ax;
    edx &= 0x80;
    edx ^= ax;
    *ptr = edx;
}
