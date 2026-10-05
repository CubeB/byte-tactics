// Decompiled by Opus. Names are provisional.
// std::copy_backward<Elem*, Elem*>(first, last, dest) from MSVC 5's
// <xutility>, used by std::vector<TdfField>::insert (0x4c59d0): it
// assigns [first, last) backwards into the range ending at dest and returns
// the start of the copies. Its neighbours (fill 0x4c5cd0, uninitialized_fill_n
// 0x4c5c20, _Construct 0x4c5d60) all end in `ret N` too, so this file of the
// original was compiled with __stdcall as the default convention; the
// template is written out here as a __stdcall function. The element holds two
// reference-counted handles (see 0x4c5bc0.cpp) assigned with 0x4c93b0.

struct Class_004c93b0 {
    char* ptr;

    Class_004c93b0* Assign(Class_004c93b0* param_1);

    Class_004c93b0& operator=(const Class_004c93b0& other)
    {
        Assign((Class_004c93b0*)&other);
        return *this;
    }
};

struct TdfField {
    Class_004c93b0 a;                  // +0x0
    Class_004c93b0 b;                  // +0x4
};

// FUNCTION: 0x4c5d10
TdfField* __stdcall FUN_004c5d10(TdfField* first, TdfField* last, TdfField* dest)
{
    while (first != last)
        *--dest = *--last;
    return dest;
}
