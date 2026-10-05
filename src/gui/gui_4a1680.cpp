// Decompiled by Sonnet. Names are provisional.

struct Out_4a1680 {
    int x0;
    int y0;
    int x1;
    int y1;
};

// FUNCTION: 0x4a1680
void __stdcall FUN_004a1680(char* param_1, int param_2, Out_4a1680* param_3)
{
    char* e = param_1 + param_2 * 0x15b;
    param_3->x0 = *(short*)(e + 0x13);
    param_3->y0 = *(short*)(e + 0x15);
    if (*e != 0) {
        param_3->x0 += *(short*)(param_1 + 0x13);
        param_3->y0 += *(short*)(param_1 + 0x15);
    }
    param_3->x1 = *(short*)(e + 0x17) - 1 + param_3->x0;
    param_3->y1 = *(short*)(e + 0x19) - 1 + param_3->y0;
}
