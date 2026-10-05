// Decompiled by Opus. Names are provisional.
// std::_Construct(TdfField*, const TdfField&) from MSVC 5's
// <xmemory>: placement-new copy of the pair of string handles, called
// without ecx from the inlined copy loops of vector insert (0x4c59d0). Like
// its neighbours (see 0x4c5d10.cpp) the original file used __stdcall as the
// default convention, so the template is written as a __stdcall function.
#include <new>

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

struct TdfField {
    Class_004c91a0 a;                  // +0x0
    Class_004c91a0 b;                  // +0x4
};

// FUNCTION: 0x4c5d60
void __stdcall FUN_004c5d60(TdfField* p, const TdfField& value)
{
    new ((void*)p) TdfField(value);
}
