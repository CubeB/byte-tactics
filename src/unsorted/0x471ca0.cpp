// Decompiled by Sonnet. Names are provisional.

extern unsigned char DAT_0051e634;

class Class_00470b80 {
public:
    void FUN_00470b80();
};

extern Class_00470b80 DAT_0051e610;

// FUNCTION: 0x471ca0
void FUN_00471ca0()
{
    if ((DAT_0051e634 & 1) == 0) {
        DAT_0051e634 |= 1;
        DAT_0051e610.FUN_00470b80();
    }
}
