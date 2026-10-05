// Decompiled by Opus. Names are provisional.
// The out-of-line destructor body of the global at 0x51e6a0 (see 0x4814c0.cpp
// and 0x4814f0.cpp): it is a std::vector of std::vector<Elem_00434360>,
// where Elem_00434360 is a struct holding one std::vector<Elem_00434020>,
// destroyed by the inlined ~vector; the Elem_00434360 elements go through
// allocator::destroy out of line (0x434400).
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Middle_004330b0;
typedef std::vector<Middle_004330b0> Outer_004330b0;

class Class_004330b0 {
public:
    void FUN_004330b0();
};

// FUNCTION: 0x4330b0
void Class_004330b0::FUN_004330b0()
{
    ((Outer_004330b0*)this)->~vector();
}
