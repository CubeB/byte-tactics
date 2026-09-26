// Decompiled by Opus. Names are provisional.

// GLOBAL: 0x51fe48
extern int DAT_0051fe48[];
// GLOBAL: 0x51fea0
extern int DAT_0051fea0[];

// FUNCTION: 0x4c7080
void __stdcall FUN_004c7080(int count)
{
    for (int i = 0; i < count; i++) {
        if (i == 0) {
            DAT_0051fe48[0] = count - 1;
        } else {
            DAT_0051fe48[i] = i - 1;
        }
        if (i == count - 1) {
            DAT_0051fea0[i] = 0;
        } else {
            DAT_0051fea0[i] = i + 1;
        }
    }
}
