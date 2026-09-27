// Decompiled by Haiku. Names are provisional.

extern unsigned char DAT_0051e634;
extern int DAT_0051e610;

void FUN_00470b80(int*);

// FUNCTION: 0x471ca0
void FUN_00471ca0()
{
    unsigned char cl = DAT_0051e634;
    unsigned char al = 1;

    if ((al & cl) == 0) {
        cl |= al;
        DAT_0051e634 = cl;
        FUN_00470b80((int*)&DAT_0051e610);
    }
}
