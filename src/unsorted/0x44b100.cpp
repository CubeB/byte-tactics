// Decompiled by Opus. Names are provisional.

extern int* DAT_005129ac;
extern int* DAT_005129b0;

void FUN_004d85a0(int* param_1);

// FUNCTION: 0x44b100
void FUN_0044b100()
{
    if (DAT_005129ac) {
        FUN_004d85a0(DAT_005129ac);
    }
    if (DAT_005129b0) {
        FUN_004d85a0(DAT_005129b0);
    }
    DAT_005129ac = DAT_005129b0 = 0;
}
