// Decompiled by Haiku. Names are provisional.

class Class_00480c30 {
public:
    char unknown_0[0x540];
    int* field_540;                           // +0x540

    int FUN_00480c30(int param_1, int param_2);
};

// FUNCTION: 0x480c30
int Class_00480c30::FUN_00480c30(int param_1, int param_2)
{
    int eax = param_1 + param_1 * 2;
    int edx = eax + eax * 8;
    return *(int*)((unsigned char*)field_540 + edx * 2 + 0x22 + param_2 * 4 + 4);
}
