// Decompiled by Sonnet. Names are provisional.

struct Obj2_4d6b70 {
    char unknown_0[0x24];
    void (__stdcall *fn)(int, int);   // +0x24
    int field28;                       // +0x28
};

void __stdcall FUN_004d5e00(int* param_1, Obj2_4d6b70* param_2, void* param_3);

// FUNCTION: 0x4d6b70
int __stdcall FUN_004d6b70(int* param_1, Obj2_4d6b70* param_2, void* param_3)
{
    FUN_004d5e00(param_1, param_2, param_3);
    param_2->fn(param_2->field28, param_1[9]);
    param_2->fn(param_2->field28, (int)param_1);
    return 0;
}
