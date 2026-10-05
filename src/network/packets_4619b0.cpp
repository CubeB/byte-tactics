// Decompiled by Sonnet. Names are provisional.

class Class_00461630 {
public:
    void* FUN_00461630(int param_1, int param_2);
};

class Class_00462710 {
public:
    void* FUN_00462710(int param_1, void* param_2, unsigned int param_3);
};

class Class_004619b0 {
public:
    void* FUN_004619b0(int param_1, int param_2, void* param_3, unsigned int param_4);
};

// FUNCTION: 0x4619b0
void* Class_004619b0::FUN_004619b0(int param_1, int param_2, void* param_3, unsigned int param_4)
{
    void* obj = ((Class_00461630*)this)->FUN_00461630(param_2, 1);
    if (obj != 0) {
        return ((Class_00462710*)obj)->FUN_00462710(param_1, param_3, param_4);
    }
    return 0;
}
