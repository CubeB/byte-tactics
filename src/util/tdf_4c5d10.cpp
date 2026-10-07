// Decompiled by Opus. Names are provisional.
// std::copy_backward<Elem*, Elem*>(first, last, dest), used by
// std::vector<TdfField>::insert (0x4c59d0): it assigns [first, last) backwards
// into the range ending at dest and returns the start of the copies. The
// element holds two reference-counted handles (see 0x4c5bc0.cpp) assigned with
// 0x4c93b0.

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

// __stdcall: the original file's default convention (neighbours end in ret N).
// FUNCTION: 0x4c5d10
TdfField* __stdcall FUN_004c5d10(TdfField* first, TdfField* last, TdfField* dest)
{
    while (first != last)
        *--dest = *--last;
    return dest;
}
