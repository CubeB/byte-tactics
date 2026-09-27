// Decompiled by Haiku. Names are provisional.

extern void FUN_004c9740(const char*);
extern int DAT_0050a780;

// FUNCTION: 0x4c9790
int FUN_004c9790(int param_1) {
    FUN_004c9740("HAPINET_guaranteepackets\n");
    int result = DAT_0050a780;
    DAT_0050a780 = param_1;
    return result;
}
