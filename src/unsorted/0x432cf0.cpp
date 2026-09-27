// Decompiled by Haiku. Names are provisional.

class Class_004c91a0 {
public:
    void init(int* param_1);
};

// FUNCTION: 0x432cf0
void __stdcall FUN_00432cf0(int* param_1, int* param_2)
{
    if (param_1 != 0) {
        ((Class_004c91a0*)param_1)->init(param_2);
        *(int*)((unsigned char*)param_1 + 4) = param_2[1];
    }
}
