// Decompiled by Sonnet. Names are provisional.

struct Obj {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;
    unsigned short flag : 1;  // +0xf0, mask 2
};

extern Obj* DAT_0051fbd0;
extern void __stdcall FUN_004b5510(int);

// FUNCTION: 0x4b5910
void FUN_004b5910()
{
    if (DAT_0051fbd0->flag) {
        FUN_004b5510(0);
    } else {
        FUN_004b5510(1);
    }
}
