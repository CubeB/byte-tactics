// Decompiled by Opus. Names are provisional.
// A method of the global DAT_00513000 (its one caller, 0x451df0, sets ecx to
// it) that ignores `this` and forwards to 0x462710 on the object it is
// given, which that caller passes as &DAT_00513000 + 8.

class Class_00462710 {
public:
    void* FUN_00462710(int param_1, void* param_2, unsigned int param_3);
};

class Class_00461990 {
public:
    void FUN_00461990(int param_1, Class_00462710* param_2, int param_3, int param_4);
};

// FUNCTION: 0x461990
void Class_00461990::FUN_00461990(int param_1, Class_00462710* param_2, int param_3, int param_4)
{
    int a = param_4;
    int b = param_3;
    int c = param_1;
    param_2->FUN_00462710(c, (void*)b, a);
}
