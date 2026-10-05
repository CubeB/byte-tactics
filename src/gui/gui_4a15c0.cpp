// Decompiled by Opus. Names are provisional.

struct Out_4a15c0 {
    int x0;
    int y0;
    int x1;
    int y1;
};

// FUNCTION: 0x4a15c0
void __stdcall FUN_004a15c0(char* param_1, int param_2, Out_4a15c0* param_3)
{
    char* e = param_1 + param_2 * 0x15b;
    if (*e == 0) {
        param_3->x0 = 0;
        param_3->y0 = 0;
    } else {
        param_3->x0 = *(short*)(e + 0x13);
        param_3->y0 = *(short*)(e + 0x15);
    }
    param_3->x1 = *(short*)(e + 0x17) - 1 + param_3->x0;
    param_3->y1 = *(short*)(e + 0x19) - 1 + param_3->y0;
}
