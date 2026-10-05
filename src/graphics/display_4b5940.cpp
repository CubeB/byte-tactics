// Decompiled by Opus. Names are provisional.

struct Obj_004b5940 {
    char unknown_0[0xd4];
    int x;                             // +0xd4
    int y;                             // +0xd8
    char unknown_dc[0xf0 - 0xdc];
    unsigned char bit0 : 1;            // +0xf0
    unsigned char flag : 1;            // +0xf0, mask 2
};

extern Obj_004b5940* DAT_0051fbd0;
extern void __stdcall FUN_004b5510(int);

// FUNCTION: 0x4b5940
void __stdcall FUN_004b5940(int x, int y)
{
    DAT_0051fbd0->x = x;
    DAT_0051fbd0->y = y;
    FUN_004b5510(DAT_0051fbd0->flag);
}
