// Decompiled by deepseek-v4.1-flash. Names are provisional.

void FUN_004d85a0(int* param_1);

extern int* DAT_0051f2e0;
extern int* DAT_0051f2e4;
extern int* DAT_0051f2e8;
extern int* DAT_0051f2ec;

// FUNCTION: 0x491e50
void FUN_00491e50()
{
    if (DAT_0051f2e0 != 0) {
        FUN_004d85a0(DAT_0051f2e0);
    }
    if (DAT_0051f2e4 != 0) {
        FUN_004d85a0(DAT_0051f2e4);
    }
    if (DAT_0051f2e8 != 0) {
        FUN_004d85a0(DAT_0051f2e8);
    }
    DAT_0051f2e8 = 0;
    DAT_0051f2e4 = 0;
    DAT_0051f2e0 = 0;
    if (DAT_0051f2ec != 0) {
        FUN_004d85a0(DAT_0051f2ec);
    }
    DAT_0051f2ec = 0;
}
