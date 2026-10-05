// Decompiled by Opus. Names are provisional.
// std::fill over an array of {string handle, int} pairs (compare the
// copy_backward at 0x432cb0): assigns the handle through its assignment
// operator 0x4c93b0 and copies the int. The file was compiled with
// __stdcall as the default, hence `ret 0xc`.

class Class_004c93b0 {
public:
    char* ptr;
    Class_004c93b0* Assign(Class_004c93b0* param_1);
};

struct Elem_432cb0 {
    Class_004c93b0 handle;
    int field4;
};

// FUNCTION: 0x432c80
void __stdcall FUN_00432c80(Elem_432cb0* first, Elem_432cb0* last, Elem_432cb0* value)
{
    for (; first != last; ++first) {
        first->handle.Assign(&value->handle);
        first->field4 = value->field4;
    }
}
