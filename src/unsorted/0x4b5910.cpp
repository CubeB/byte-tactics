// Decompiled by Haiku. Names are provisional.

extern void* DAT_0051fbd0;
extern void __stdcall FUN_004b5510(int);

// FUNCTION: 0x4b5910
void FUN_004b5910()
{
    void* eax = DAT_0051fbd0;
    unsigned char cl = *(unsigned char*)((char*)eax + 0xf0);
    cl >>= 1;
    if ((cl & 1) != 0) {
        FUN_004b5510(0);
    } else {
        FUN_004b5510(1);
    }
}
