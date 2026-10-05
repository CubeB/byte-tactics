// Decompiled by Sonnet. Names are provisional.

struct Class_004404c0 {
    char unknown_0[0x10];
    int stride;           // +0x10
    char unknown_14[4];
    unsigned int* base;   // +0x18

    void FUN_004404c0(int param_1, int param_2, int param_3);
};

// FUNCTION: 0x4404c0
void Class_004404c0::FUN_004404c0(int param_1, int param_2, int param_3)
{
    int shift = (param_2 & 0xf) << 1;
    int row = (param_2 >> 4) * stride + param_1;
    unsigned int* p = base + row;
    *p = (param_3 << shift) | (~(3 << shift) & *p);
}
