// Decompiled by Opus. Names are provisional.
// std::_Construct(TdfField*, const TdfField&): placement-new copy of the pair
// of string handles, called without ecx from the copy loops of vector insert
// (0x4c59d0).
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

// __stdcall: the original file's default convention (see 0x4c5d10.cpp).
// FUNCTION: 0x4c5d60
void __stdcall FUN_004c5d60(TdfField* p, const TdfField& value)
{
    new ((void*)p) TdfField(value);
}
