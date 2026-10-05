// Decompiled by Opus. Names are provisional.
// std::fill<Elem*, Elem>(first, last, x) from MSVC 5's <xutility>, used by
// std::vector<Elem_004c5bc0>::insert; like its neighbour copy_backward
// (0x4c5d10) it ends in `ret N`, so it is written as a __stdcall function.
// The element holds two reference-counted handles assigned with 0x4c93b0.

struct Class_004c93b0 {
    char* ptr;

    Class_004c93b0* Assign(Class_004c93b0* param_1);

    Class_004c93b0& operator=(const Class_004c93b0& other)
    {
        Assign((Class_004c93b0*)&other);
        return *this;
    }
};

struct Elem_004c5bc0 {
    Class_004c93b0 a;                  // +0x0
    Class_004c93b0 b;                  // +0x4
};

// FUNCTION: 0x4c5cd0
void __stdcall FUN_004c5cd0(Elem_004c5bc0* first, Elem_004c5bc0* last, const Elem_004c5bc0& x)
{
    for (; first != last; ++first)
        *first = x;
}
