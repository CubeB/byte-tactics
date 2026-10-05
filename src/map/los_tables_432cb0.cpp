// Decompiled by Sonnet. Names are provisional.

class Class_004c93b0 {
public:
    char* ptr;
    Class_004c93b0* Assign(Class_004c93b0* param_1);
};

struct Elem_432cb0 {
    Class_004c93b0 handle;
    int field4;
};

// FUNCTION: 0x432cb0
Elem_432cb0* __stdcall FUN_00432cb0(Elem_432cb0* param_1, Elem_432cb0* param_2, Elem_432cb0* param_3)
{
    while (param_1 != param_2) {
        --param_2;
        --param_3;
        param_3->handle.Assign(&param_2->handle);
        param_3->field4 = param_2->field4;
    }
    return param_3;
}
