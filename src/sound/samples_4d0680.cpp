// Decompiled by Sonnet. Names are provisional.

void __stdcall RemoveTimer(int param_1);

class Class_004d02a0 {
public:
    char unknown_0[0x288];
    int field288;                 // +0x288
    void FUN_004d02a0(char* param_1, int param_2, int param_3, int param_4);
};

extern Class_004d02a0* DAT_0051ff14;
extern int DAT_0051ff58;
extern char DAT_0051ff60[];

// FUNCTION: 0x4d0680
void __stdcall FUN_004d0680(int unused1)
{
    RemoveTimer(DAT_0051ff14->field288);
    DAT_0051ff14->field288 = -1;
    DAT_0051ff14->FUN_004d02a0(DAT_0051ff60, 2, DAT_0051ff58, 0);
}
